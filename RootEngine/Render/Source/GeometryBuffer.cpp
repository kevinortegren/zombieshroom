#include <RootEngine/Render/Include/GeometryBuffer.h>

namespace Render
{
	GeometryBuffer::~GeometryBuffer()
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	void GeometryBuffer::Init(int p_width, int p_height)
	{
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		m_color.Load(p_width, p_height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color.GetHandle(), 0);
	}

	void GeometryBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GeometryBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GeometryBuffer::Read()
	{
		
	}
}