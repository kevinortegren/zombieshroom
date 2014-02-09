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
		
		// Load Unit sphere.
		m_unitSphere = g_context.m_resourceManager->LoadCollada("Primitives/sphere")->m_meshes[0];

		// Light uniforms.
		m_lights = p_renderer->CreateBuffer(GL_UNIFORM_BUFFER);
		m_lights->BufferData(1, sizeof(m_lightVars), &m_lightVars);
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_LIGHTS, m_lights->GetBufferId());

		// Setup la-buffer.
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_la = p_renderer->CreateTexture();
		m_la->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_la->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, p_gbuffer->m_depthTexture->GetHandle(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	void LightingDevice::Process(Mesh& p_fullscreenQuad, int p_backgroundEffect)
	{
		// TODO: Do this once per frame.
		m_lights->BufferSubData(0, sizeof(m_lightVars), &m_lightVars);

		//TODO: Set the programs in initialize.
		auto ambient = m_deferredTech->GetPrograms()[0];
		auto directional = m_deferredTech->GetPrograms()[1];
		auto pointlight = m_deferredTech->GetPrograms()[2];
		auto pointlight_stencil = m_deferredTech->GetPrograms()[3];

		/*
			Defferedpass: Will probely use alpha blending since it's more flexible.
			Waterpass: Will need addative blending since simular to glass.
		*/

		// TODO: Read flag from pass.
		auto background = m_deferredTech->GetPrograms()[4 + p_backgroundEffect];

		p_fullscreenQuad.Bind();

		// Ambient.
		ambient->Apply();
		p_fullscreenQuad.Draw();

		// Directional.
		directional->Apply();
		p_fullscreenQuad.DrawInstanced(m_numDirectionalLights);
		p_fullscreenQuad.Unbind();

		GLuint64 startTime, stopTime;
		unsigned int queryID[2];

		glGenQueries(2, queryID);
		glQueryCounter(queryID[0], GL_TIMESTAMP);

		// Disable cull face so we process both front/back polygons.
		glDisable(GL_CULL_FACE);

		// Enable writing.
		glStencilMask(0xFF);

		// If we fail depth test increase value.
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		
		// If we fail depth test decrease value.
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		
		m_unitSphere->Bind();
		pointlight_stencil->Apply();

		// Fragments inside the point light range will have its stencil value differ from 127.
		m_unitSphere->DrawInstanced(m_numPointLights);

		// Render fragments of object inside the point light range.
		glStencilFunc(GL_NOTEQUAL, 127, 0xFF);
		glStencilMask(0x00);

		// Enable front face culling incase the camera is inside the pointlight.
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		pointlight->Apply();
		m_unitSphere->DrawInstanced(m_numPointLights);

		m_unitSphere->Unbind();
		glCullFace(GL_BACK);

		glQueryCounter(queryID[1], GL_TIMESTAMP);

		GLint stopTimerAvailable = 0;
		while (!stopTimerAvailable) {
			glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
		}

		glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
		glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &stopTime);

		printf("Time spent on the GPU: %f ms\n", (stopTime - startTime) / 1000000.0);

		// Blend background pixels with lighting accumulation.
		p_fullscreenQuad.Bind();
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	
		background->Apply();	
		p_fullscreenQuad.Draw();
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