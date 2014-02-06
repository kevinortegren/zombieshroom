#include <RootEngine/Render/Include/RenderToTexture.h>

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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderToTexture::BindTextureAsDepth()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture->GetHandle(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}