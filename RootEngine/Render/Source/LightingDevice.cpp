#include <RootEngine\Render\Include\LightingDevice.h>
#include <RootEngine\Render\Include\RenderExtern.h>
#include <RootEngine\Include\ResourceManager\ResourceManager.h>
#include <ctime>

namespace Render
{
	LightingDevice::LightingDevice()
	: m_numDirectionalLights(0),
		m_numPointLights(0) {}

	void LightingDevice::Init(GLRenderer* p_renderer, int p_width, int p_height, GeometryBuffer* p_gbuffer, Mesh* p_fullscreenQuad)
	{
		// Load programs from the lighting effect file.
		Render::EffectInterface* lightingEffect = g_context.m_resourceManager->LoadEffect("Renderer/Lighting");
		m_ambient = lightingEffect->GetTechniques()[0]->GetPrograms()[0];
		m_directional = lightingEffect->GetTechniques()[0]->GetPrograms()[1];
		m_pointLight = lightingEffect->GetTechniques()[0]->GetPrograms()[2];
		m_pointLightStencil = lightingEffect->GetTechniques()[0]->GetPrograms()[3];
		m_backgroundAlphaBlend = lightingEffect->GetTechniques()[0]->GetPrograms()[4];
		m_backgroundAddative = lightingEffect->GetTechniques()[0]->GetPrograms()[5];
		m_pointLightFSQ = lightingEffect->GetTechniques()[0]->GetPrograms()[6];

		Render::EffectInterface* ssaoEffect = g_context.m_resourceManager->LoadEffect("Renderer/SSAO");
		m_ssaoTech = ssaoEffect->GetTechniques()[0];

		m_fullscreenQuad = p_fullscreenQuad;

		// Load unit sphere mesh.
		//m_unitSphere = g_context.m_resourceManager->LoadCollada("Primitives/sphere")->m_meshes[0];

		// TODO: Init triangle.

		// Light uniforms.
		m_lights = p_renderer->CreateBuffer(GL_UNIFORM_BUFFER);
		m_lights->BufferData(1, sizeof(m_lightVars), &m_lightVars);
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_LIGHTS, m_lights->GetBufferId());

		// Setup la-buffer.
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_la = p_renderer->CreateTexture();
		m_la->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);
		m_ssaoTex = p_renderer->CreateTexture();
		m_ssaoTex->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);
		m_noiseSSAOTex = p_renderer->CreateTexture();
		m_noiseSSAOTex->CreateEmptyTexture(4,4, TextureFormat::TEXTURE_RG16);
		m_noiseSSAOTex->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		m_noiseSSAOTex->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_la->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_ssaoTex->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, p_gbuffer->m_depthTexture->GetHandle(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		SetupSSAO();
	}

	void LightingDevice::SetAmbientLight(const glm::vec4& p_color)
	{
		m_lightVars.m_ambient = p_color;
	}

	void LightingDevice::AddDirectionalLight(const DirectionalLight& p_light, int index)
	{
		if(index > RENDER_MAX_DIRECTIONALLIGHTS)
			return;

		m_lightVars.m_dlights[index] = p_light;
		m_numDirectionalLights++;
	}

	void LightingDevice::AddPointLight(const PointLight& p_light, int index)
	{
		if(index > RENDER_MAX_POINTLIGHTS)
			return;

		m_lightVars.m_plights[index] = p_light;
		m_numPointLights++;
	}

	void LightingDevice::BufferLights()
	{
		m_lights->BufferSubData(0, sizeof(m_lightVars), &m_lightVars);
	}

	void LightingDevice::SSAO()
	{
		m_fullscreenQuad->Bind();
		//SSAO
		BeginSSAO();
		glDisable(GL_STENCIL_TEST);
		auto ssao = m_ssaoTech->GetPrograms()[0];
		m_noiseSSAOTex->Bind(7);
		m_ssaoTech->Apply();
		ssao->Apply();
		m_fullscreenQuad->Draw();
		glEnable(GL_STENCIL_TEST);
	}

	void LightingDevice::Ambient()
	{
		m_fullscreenQuad->Bind();
		m_ambient->Apply();
		m_ssaoTex->Bind(7);
		m_fullscreenQuad->Draw();
		m_ssaoTex->Unbind(7);
		m_fullscreenQuad->Unbind();
	}

	void LightingDevice::Directional()
	{
		m_fullscreenQuad->Bind();
		m_directional->Apply();
		m_fullscreenQuad->DrawInstanced(m_numDirectionalLights);
		m_fullscreenQuad->Unbind();
	}

	void LightingDevice::PointLightStencil()
	{
		// Disable cull face so we process both front/back polygons.
		glDisable(GL_CULL_FACE);

		// Enable writing.
		glStencilMask(0xFF);

		// If we fail depth test increase value.
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		
		// If we fail depth test decrease value.
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		
		m_unitSphere->Bind();
		m_pointLightStencil->Apply();

		// Fragments inside the point light range will have its stencil value differ from 127.
		m_unitSphere->DrawInstanced(m_numPointLights);
		m_unitSphere->Unbind();

		glEnable(GL_CULL_FACE);
	}

	void LightingDevice::PointLightRender()
	{
		// Render fragments of object inside the point light range.
		glStencilFunc(GL_NOTEQUAL, 127, 0xFF);
		glStencilMask(0x00);

		// Enable front face culling incase the camera is inside the pointlight.
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		m_unitSphere->Bind();
		m_pointLight->Apply();
		m_unitSphere->DrawInstanced(m_numPointLights);
		m_unitSphere->Unbind();

		glCullFace(GL_BACK);
	}

	void LightingDevice::PointLightFSQ()
	{
		m_fullscreenQuad->Bind();
		m_pointLightFSQ->Apply();
		m_fullscreenQuad->DrawInstanced(m_numPointLights);
		m_fullscreenQuad->Unbind();
	}

	void LightingDevice::BackgroundBlend(BackgroundBlend::BackgroundBlend p_mode)
	{
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		m_fullscreenQuad->Bind();
		
		switch(p_mode)
		{
		case BackgroundBlend::ADDATIVE:
			m_backgroundAddative->Apply();
			break;
		case BackgroundBlend::ALPHABLEND:
		default:
			m_backgroundAlphaBlend->Apply();
			break;
		}

		m_fullscreenQuad->Draw();
		m_fullscreenQuad->Unbind();
	}

	void LightingDevice::SetupSSAO()
	{
		srand (static_cast <unsigned> (time(0)));
		int kernelSize = 16;
		for (int i = 0; i < kernelSize; ++i)
		{
			m_kernel[i] = glm::vec4(
				Random(-1.0f, 1.0f),
				Random(-1.0f, 1.0f),
				Random(0.0f, 1.0f),
				0.0f);
			m_kernel[i] = glm::normalize(m_kernel[i]);
			//m_kernel[i] *= Random(0.0f, 1.0f);
			float scale = float(i) / float(kernelSize);
			scale = glm::mix<float>(0.1f, 1.0f, scale * scale);
			m_kernel[i] *= scale;
			//g_context.m_logger->LogText(LogTag::GENERAL, LogLevel::DEBUG_PRINT, "Kernel X: %f, Y: %f, Z: %f", m_kernel[i].x, m_kernel[i].y, m_kernel[i].z);
		}

		for(int i = 0; i < kernelSize; i++)
		{
			//g_context.m_logger->LogText(LogTag::GENERAL, LogLevel::DEBUG_PRINT, "Length: %f", glm::length(m_kernel[i]));
		}

		BufferInterface* ssaoBuff = m_ssaoTech->GetBufferInterface();
		ssaoBuff->BufferData((size_t)kernelSize, sizeof(glm::vec4), &m_kernel[0]);

		const int noiseSize = 16;
		glm::vec2 noise[noiseSize];
		for (int i = 0; i < noiseSize; ++i) {
			noise[i] = glm::vec2(
				Random(-1.0f, 1.0f),
				Random(-1.0f, 1.0f));
			noise[i] = glm::normalize(noise[i]);
		}

		m_noiseSSAOTex->BufferData(&noise[0]);
	};

	void LightingDevice::BeginSSAO()
	{
		// Bind la-buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT1};
		glDrawBuffers(1, buffers);

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glStencilMask(0x00);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	/*void LightingDevice::Process(Mesh& p_fullscreenQuad, int p_backgroundEffect)
	{

		GLuint64 startTime, stopTime;
		unsigned int queryID[2];

		glGenQueries(2, queryID);
		glQueryCounter(queryID[0], GL_TIMESTAMP);




		glQueryCounter(queryID[1], GL_TIMESTAMP);

		GLint stopTimerAvailable = 0;
		while (!stopTimerAvailable) {
			glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
		}

		glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
		glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &stopTime);

		printf("Time spent on the GPU: %f ms\n", (stopTime - startTime) / 1000000.0);

	} */

	void LightingDevice::Clear()
	{
		// Bind la-buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, buffers);

		// Only work with fragments modified by geometry pass.
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glStencilMask(0x00);

		// Clear la-buffer.
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void LightingDevice::ResetLights()
	{
		m_numDirectionalLights = 0;
		m_numPointLights = 0;
	}

	void LightingDevice::Resize(int p_width, int p_height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_la->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_la->GetHandle(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	float LightingDevice::Random( float p_low, float p_high )
	{
		return p_low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(p_high-p_low)));
	}

}