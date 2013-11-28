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

		CreateBuffers(p_width, p_height);

		Unbind();
	}

	void GeometryBuffer::CreateBuffers(int p_width, int p_height)
	{
		glGenTextures(1, &m_depthHandle);
		glBindTexture(GL_TEXTURE_2D, m_depthHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, p_width, p_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthHandle, 0);

		glGenTextures(1, &m_diffuseHandle);
		glBindTexture(GL_TEXTURE_2D, m_diffuseHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_diffuseHandle, 0);

		glGenTextures(1, &m_normalsHandle);
		glBindTexture(GL_TEXTURE_2D, m_normalsHandle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalsHandle, 0);
	}

	void GeometryBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GeometryBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GeometryBuffer::Read()
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseHandle);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_normalsHandle);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_depthHandle);
	}

	void GeometryBuffer::Resize(int p_width, int p_height)
	{
		glDeleteTextures(1, &m_diffuseHandle);
		glDeleteTextures(1, &m_normalsHandle);
		glDeleteTextures(1, &m_depthHandle);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		CreateBuffers(p_width, p_height);

		Unbind();
	}
}