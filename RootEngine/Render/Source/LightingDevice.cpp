#include <RootEngine\Render\Include\LightingDevice.h>
#include <RootEngine\Render\Include\RenderExtern.h>
#include <RootEngine\Include\ResourceManager\ResourceManager.h>

namespace Render
{
	LightingDevice::LightingDevice()
	: m_numDirectionalLights(0),
		m_numPointLights(0) {}

	void LightingDevice::Init(GLRenderer* p_renderer, int p_width, int p_height, GeometryBuffer* p_gbuffer)
	{
		// Load techniques.	
		Render::EffectInterface* lightingEffect = g_context.m_resourceManager->LoadEffect("Renderer/Lighting");
		m_deferredTech = lightingEffect->GetTechniques()[0];
		Render::EffectInterface* ssaoEffect = g_context.m_resourceManager->LoadEffect("Renderer/SSAO");
		m_ssaoTech = ssaoEffect->GetTechniques()[0];
		
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

	float LightingDevice::Random(float p_low, float p_high)
	{
		return p_low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(p_high-p_low)));
	}

	void LightingDevice::SetupSSAO()
	{
		int kernelSize = 16;
		for (int i = 0; i < kernelSize; ++i)
		{
			m_kernel[i] = glm::vec3(
				Random(-1.0f, 1.0f),
				Random(-1.0f, 1.0f),
				Random(0.0f, 1.0f));
			m_kernel[i] = glm::normalize(m_kernel[i]);
			//m_kernel[i] *= Random(0.0f, 1.0f);
			float scale = float(i) / float(kernelSize);
			scale = glm::mix<float>(0.1f, 1.0f, scale * scale);
			m_kernel[i] *= scale;
		}

		BufferInterface* ssaoBuff = m_ssaoTech->GetBufferInterface();
		ssaoBuff->BufferData((size_t)kernelSize, sizeof(glm::vec3), &m_kernel[0]);

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

		glClear(GL_COLOR_BUFFER_BIT);
	}

	void LightingDevice::Clear()
	{
		GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, buffers);

		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilMask(0x00);

		// Clear la-buffer.
		glClear(GL_COLOR_BUFFER_BIT);

	}

	void LightingDevice::Process(Mesh& p_fullscreenQuad)
	{
		p_fullscreenQuad.Bind();
		//SSAO
		BeginSSAO();
		glDisable(GL_STENCIL_TEST);
		auto ssao = m_ssaoTech->GetPrograms()[0];
		m_noiseSSAOTex->Bind(7);
		m_ssaoTech->Apply();
		ssao->Apply();
		p_fullscreenQuad.Draw();
		glEnable(GL_STENCIL_TEST);
		Clear();
		m_lights->BufferSubData(0, sizeof(m_lightVars), &m_lightVars);

		auto ambient = m_deferredTech->GetPrograms()[0];
		auto directional = m_deferredTech->GetPrograms()[1];
		auto pointlight = m_deferredTech->GetPrograms()[2];
		auto background = m_deferredTech->GetPrograms()[4];



		// Background.
		background->Apply();	
		p_fullscreenQuad.Draw();

		m_ssaoTex->Bind(7);
		// Ambient.
		ambient->Apply();
		p_fullscreenQuad.Draw();
		m_ssaoTex->Unbind(7);

		// Directional.
		directional->Apply();
		p_fullscreenQuad.DrawInstanced(m_numDirectionalLights);

		// Pointlights.
		pointlight->Apply();
		p_fullscreenQuad.DrawInstanced(m_numPointLights);

		
	
		p_fullscreenQuad.Unbind();

		// Unbind.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void LightingDevice::ClearLights()
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

}