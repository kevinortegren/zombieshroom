#include <RootEngine/Render/Include/GlowDevice.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RenderExtern.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace Render
{
	void GlowDevice::Init(GLRenderer* p_renderer, int p_width, int p_height)
	{
		m_width = p_width;
		m_height = p_height;

		m_glowEffect = g_context.m_resourceManager->LoadEffect("PostProcess/Glow");

		glGenFramebuffers(1, &m_glowFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_glowFramebuffer);

		m_glowTexture = p_renderer->CreateTexture();
		m_glowTexture->CreateEmptyTexture(p_width / 2, p_height / 2, TextureFormat::TEXTURE_RGBA);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glowTexture->GetHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		struct
		{
			int halfWidth;
			int halfHeight;
			float blurFactor;
			float blurStrength;
			float blurRadius;

		} data;

		data.halfWidth = p_width / 2;
		data.halfHeight = p_height / 2;
		data.blurRadius = 2.2f;
		data.blurFactor = 2.0f;
		data.blurStrength = 0.3f;

		m_glowEffect->GetTechniques()[0]->m_perTechniqueBuffer->BufferData(1, sizeof(data), &data);
	}

	void GlowDevice::Process(GLRenderer* p_renderer, Mesh* m_mesh)
	{
		// Apply technique.
		m_glowEffect->GetTechniques()[0]->Apply();

		// Clear glow fbo.
		glBindFramebuffer(GL_FRAMEBUFFER, m_glowFramebuffer);
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Horizontal pass.
		glViewport(0, 0, m_width / 2, m_height / 2);

		m_glowEffect->GetTechniques()[0]->GetPrograms()[0]->Apply();
		m_mesh->Draw();

		p_renderer->BindForwardFramebuffer();

		// Bind glow as input.
		m_glowTexture->Bind(5);

		// Vertical pass.
		glViewport(0, 0, m_width, m_height);

		m_glowEffect->GetTechniques()[0]->GetPrograms()[1]->Apply();
		m_mesh->Draw();
	}

	void GlowDevice::Resize(int p_width, int p_height)
	{
		//TODO: Resize.
	}

	void SetGlowFactor(float p_factor)
	{

	}

	void SetGlowStrength(float p_strength)
	{

	}

	void SetGlowRadius(float p_radius)
	{

	}

}
