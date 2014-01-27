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

		CreateBuffers(p_width, p_height);

		Unbind();
	}

	void GeometryBuffer::CreateBuffers(int p_width, int p_height)
	{
		m_depthTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_DEPTH_COMPONENT);
		m_depthTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_depthTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_depthTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_depthTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_diffuseTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);
		m_normalsTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);
		m_glowTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_RGBA);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_diffuseTexture->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalsTexture->GetHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_glowTexture->GetHandle(), 0);
	}

	void GeometryBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GeometryBuffer::Unbind()
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
	}

	void GeometryBuffer::Read()
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->GetHandle());

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_normalsTexture->GetHandle());

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture->GetHandle());

		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, m_glowTexture->GetHandle());
	}

	void GeometryBuffer::Resize(int p_width, int p_height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		CreateBuffers(p_width, p_height);

		Unbind();
	}
}