#include <RootEngine/Render/Include/GlowDevice.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RenderExtern.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace Render
{
	GlowDevice::~GlowDevice()
	{
		glDeleteFramebuffers(1, &m_glowFramebuffer);
	}

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
			float halfWidth;
			float halfHeight;
			float blurRadius;
			//float gauss[11];
		} data;

		data.halfWidth = (float)p_width / 2.0f;
		data.halfHeight = (float)p_height / 2.0f;
		data.blurRadius = 1.4f;

		float blurFactor = 9.0f;
		float blurStrength = 0.3f;

		/*data.gauss[0] = Gaussian(blurFactor * (1.0f - blurStrength), (float)pow(blurFactor * 0.35, 2));
		for(int i = 0; i < 10; i++)
		{
			data.gauss[i+1] = Gaussian(i * (1.0f - blurStrength), (float)pow(blurFactor * 0.35, 2));
		}*/

		m_glowEffect->GetTechniques()[0]->m_perTechniqueBuffer->BufferData(1, sizeof(data), &data, GL_DYNAMIC_DRAW);

		m_display = true;
	}

	float GlowDevice::Gaussian(float x, float deviation)
	{
		return (float)((1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation))));	
	}

	void GlowDevice::SetHalfWidth(float p_halfWidth)
	{
		m_glowEffect->GetTechniques()[0]->m_perTechniqueBuffer->BufferSubData(0, sizeof(float), &p_halfWidth);
	}

	void GlowDevice::SetHalfHeight(float p_halfHeight)
	{
		m_glowEffect->GetTechniques()[0]->m_perTechniqueBuffer->BufferSubData(4, sizeof(float), &p_halfHeight);
	}

	void GlowDevice::SetGlowFactor(float p_factor)
	{
		g_context.m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Changed glow factor to %f", p_factor);
		//m_glowEffect->GetTechniques()[0]->m_perTechniqueBuffer->BufferSubData(8, sizeof(float), &p_factor);
	}

	void GlowDevice::SetGlowStrength(float p_strength)
	{
		g_context.m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Changed glow strength to %f", p_strength);
		//m_glowEffect->GetTechniques()[0]->m_perTechniqueBuffer->BufferSubData(12, sizeof(float), &p_strength);
	}

	void GlowDevice::SetGlowRadius(float p_radius)
	{
		g_context.m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Changed glow radius to %f", p_radius);
		m_glowEffect->GetTechniques()[0]->m_perTechniqueBuffer->BufferSubData(8, sizeof(float), &p_radius);
	}

	void GlowDevice::HorizontalPass(Mesh* m_mesh)
	{
		if(m_display)
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
		}
	}

	void GlowDevice::VerticalPass(Mesh* m_mesh)
	{
		if(m_display)
		{
			// Bind glow as input.
			glActiveTexture(GL_TEXTURE0 + GLRenderer::s_textureSlots[TextureSemantic::SHADER_INPUT]);
			glBindTexture(GL_TEXTURE_2D, m_glowTexture->GetHandle());

			// Vertical pass.
			glViewport(0, 0, m_width, m_height);

			m_glowEffect->GetTechniques()[0]->GetPrograms()[1]->Apply();
			m_mesh->Draw();
		}
	}

	void GlowDevice::Resize(int p_width, int p_height)
	{
		m_width = p_width;
		m_height = p_height;

		glBindFramebuffer(GL_FRAMEBUFFER, m_glowFramebuffer);
		m_glowTexture->CreateEmptyTexture(p_width / 2, p_height / 2, TextureFormat::TEXTURE_RGBA);	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		SetHalfWidth((float)p_width/2.0f);
		SetHalfHeight((float)p_height/2.0f);
	}

	void GlowDevice::Display(bool p_value)
	{
		m_display = p_value;
	}
}
