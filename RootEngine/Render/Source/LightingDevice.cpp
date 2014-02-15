#include <RootEngine\Render\Include\LightingDevice.h>
#include <RootEngine\Render\Include\RenderExtern.h>
#include <RootEngine\Include\ResourceManager\ResourceManager.h>

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

		m_fullscreenQuad = p_fullscreenQuad;

		// Load unit sphere mesh.
		m_unitSphere = g_context.m_resourceManager->LoadCollada("Primitives/sphere")->m_meshes[0];

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

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_la->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, p_gbuffer->m_depthTexture->GetHandle(), 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_showAmbient = true;;
		m_showDirectional = true;
		m_showPointlights = true;
		m_showBackgroundBlend = true;
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

	void LightingDevice::Ambient()
	{
		if(m_showAmbient)
		{
			m_fullscreenQuad->Bind();
			m_ambient->Apply();
			m_fullscreenQuad->Draw();
			m_fullscreenQuad->Unbind();
		}
	}

	void LightingDevice::Directional()
	{
		if(m_showDirectional)
		{
			m_fullscreenQuad->Bind();
			m_directional->Apply();
			m_fullscreenQuad->DrawInstanced(m_numDirectionalLights);
			m_fullscreenQuad->Unbind();
		}
	}

	void LightingDevice::Point()
	{
		if(m_showPointlights)
		{
			PointLightStencil();
			PointLightRender();
		}
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

	}

	void LightingDevice::BackgroundBlend(BackgroundBlend::BackgroundBlend p_mode)
	{
		if(m_showBackgroundBlend)
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
	}

	void LightingDevice::ShowAmbient(bool p_value)
	{
		m_showAmbient = p_value;
	}

	void LightingDevice::ShowDirectional(bool p_value)
	{
		m_showDirectional = p_value;
	}

	void LightingDevice::ShowPointLights(bool p_value)
	{
		m_showPointlights = p_value;
	}

	void LightingDevice::ShowBackgroundBlend(bool p_value)
	{
		m_showBackgroundBlend = p_value;
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
}