#include <RootEngine/Render/Include/GeometryBuffer.h>

#include <RootEngine/Render/Include/RenderExtern.h>
#include <RootEngine/Render/Include/Renderer.h>

namespace Render
{
	GeometryBuffer::~GeometryBuffer()
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	void GeometryBuffer::Init(GLRenderer* p_renderer, int p_width, int p_height)
	{
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_depthTexture = p_renderer->CreateTexture();
		m_diffuseTexture = p_renderer->CreateTexture();
		m_normalsTexture = p_renderer->CreateTexture();
		m_glowTexture = p_renderer->CreateTexture();
		m_backgroundTexture = p_renderer->CreateTexture();

		CreateTextures(p_width, p_height);

		UnbindTextures();
	}

	void GeometryBuffer::CreateTextures(int p_width, int p_height)
	{
		m_depthTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_DEPTH_STENCIL);
		m_depthTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_depthTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_depthTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_depthTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_diffuseTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);
		m_normalsTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RG16);
		m_glowTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);
		m_backgroundTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_diffuseTexture->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalsTexture->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_glowTexture->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_backgroundTexture->GetHandle(), 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			g_context.m_logger->LogText(LogTag::RENDER, LogLevel::SUCCESS, "Good framebuffer support.");
			break;
		default:
			g_context.m_logger->LogText(LogTag::RENDER, LogLevel::WARNING, "Bad framebuffer support!");
			break;
		}
	}

	void GeometryBuffer::Clear(GLbitfield p_mask)
	{
		glClear(p_mask);
	}

	void GeometryBuffer::Enable()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4, buffers);	

		// Enable depth for reads during geometry pass.
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture->GetHandle());
	}

	void GeometryBuffer::BindTextures()
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->GetHandle());

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_normalsTexture->GetHandle());

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture->GetHandle());

		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, m_glowTexture->GetHandle());

		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, m_backgroundTexture->GetHandle());
	}

	void GeometryBuffer::UnbindTextures()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void GeometryBuffer::Resize(int p_width, int p_height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		CreateTextures(p_width, p_height);

		UnbindTextures();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}