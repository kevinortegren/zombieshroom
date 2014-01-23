#include <RootEngine\Render\Include\LightingDevice.h>
#include <RootEngine\Render\Include\RenderExtern.h>
#include <RootEngine\Include\ResourceManager\ResourceManager.h>

namespace Render
{
	LightingDevice::LightingDevice()
	: m_numDirectionalLights(0),
		m_numPointLights(0)
	{

	}

	void LightingDevice::Init(GLRenderer* p_renderer, int p_width, int p_height)
	{
		// Load effects.	
		auto deferred = g_context.m_resourceManager->LoadEffect("Renderer/Deferred");
		m_lightingTech = deferred->GetTechniques()[0];

		// Light uniforms.
		m_lights = p_renderer->CreateBuffer(GL_UNIFORM_BUFFER);
		m_lights->BufferData(1, sizeof(m_lightVars), &m_lightVars);
		glBindBufferBase(GL_UNIFORM_BUFFER, RENDER_SLOT_LIGHTS, m_lights->GetBufferId());

		// Setup la-buffer.
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		glGenTextures(1, &m_laHandle);
		glBindTexture(GL_TEXTURE_2D, m_laHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_laHandle, 0);
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

	void LightingDevice::Process(Mesh& p_fullscreenQuad)
	{
		// Bind la-buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_lights->BufferSubData(0, sizeof(m_lightVars), &m_lightVars);

		auto ambient = m_lightingTech->GetPrograms()[0];
		auto directional = m_lightingTech->GetPrograms()[1];
		auto pointlight = m_lightingTech->GetPrograms()[2];

		p_fullscreenQuad.Bind();

		// Ambient.
		ambient->Apply();
		p_fullscreenQuad.Draw();

		// Directional.
		directional->Apply();
		p_fullscreenQuad.DrawInstanced(m_numDirectionalLights);

		// Pointlights.
		pointlight->Apply();
		p_fullscreenQuad.DrawInstanced(m_numPointLights);

		p_fullscreenQuad.Unbind();

		// Unbind.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_numPointLights = 0;
	}

	void LightingDevice::Resize(int p_width, int p_height)
	{
		//TODO
	}
}