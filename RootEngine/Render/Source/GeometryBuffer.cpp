#include <RootEngine/Render/Include/GeometryBuffer.h>

namespace Render
{
	GeometryBuffer::~GeometryBuffer()
	{
		glDeleteFramebuffers(1, &m_fbo);
		glDeleteRenderbuffers(1, &m_rbo);
	}

	void GeometryBuffer::Init(int p_width, int p_height)
	{
		glGenFramebuffers(1, &m_fbo);
		glGenRenderbuffers (1, &m_rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glBindRenderbuffer (GL_RENDERBUFFER, m_rbo);

		CreateBuffers(p_width, p_height);

		Unbind();
	}

	void GeometryBuffer::CreateBuffers(int p_width, int p_height)
	{
		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT, p_width, p_height);

		m_diffuse.Load(p_width, p_height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_diffuse.GetHandle(), 0);

		m_normals.Load(p_width, p_height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normals.GetHandle(), 0);
	}

	void GeometryBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glBindRenderbuffer (GL_RENDERBUFFER, m_rbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GeometryBuffer::Unbind()
	{
		glBindRenderbuffer (GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GeometryBuffer::Read()
	{
		m_diffuse.Enable( 0 );
		m_normals.Enable( 1 );
	}

	void GeometryBuffer::Resize(int p_width, int p_height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glBindRenderbuffer (GL_RENDERBUFFER, m_rbo);

		CreateBuffers(p_width, p_height);

		Unbind();
	}
}