#include <RootEngine/Render/Include/ShadowDevice.h>

namespace Render
{
	ShadowDevice::ShadowDevice()
	{
		m_depthTexture = nullptr;
	}

	ShadowDevice::~ShadowDevice()
	{
		if(m_depthTexture)
		{
			delete m_depthTexture;
		}
		glDeleteFramebuffers(1, &m_framebuffer);
	}

	void ShadowDevice::Init(GLRenderer* p_renderer, int p_width, int p_height)
	{
		m_depthTexture = new Texture();
		//m_depthTexture->
	}
}