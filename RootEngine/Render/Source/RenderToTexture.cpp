#include <RootEngine/Render/Include/RenderToTexture.h>
#include <fstream>

namespace Render
{
	RenderToTexture::RenderToTexture()
	{
		glGenFramebuffers(1, &m_framebuffer);
	}

	RenderToTexture::~RenderToTexture()
	{
		glDeleteFramebuffers(1, &m_framebuffer);
	}

	void RenderToTexture::BindTexture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->GetHandle(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderToTexture::BindTextureAsDepth()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture->GetHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderToTexture::Store(const char* p_filename)
	{
		m_texture->Store(p_filename);
	}

	void RenderToTexture::SetTexture(TextureInterface* p_texture)
	{
		m_texture = p_texture;
	}

	TextureInterface* RenderToTexture::GetTexture()
	{
		return m_texture;
	}

	GLuint RenderToTexture::GetFramebuffer()
	{
		return m_framebuffer;
	}


}