#include <RootEngine/Render/Include/ShadowDevice.h>

#include <RootEngine/Render/Include/RenderExtern.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace Render
{
	ShadowDevice::ShadowDevice()
	{
		m_depthTexture = nullptr;
		m_numberOfShadowcasters = 0;
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
		m_depthTexture->CreateEmptyTexture(p_width, p_height, TextureFormat::TEXTURE_DEPTH_COMPONENT);
		m_depthTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		m_depthTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		m_depthTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_depthTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//m_depthTexture->SetParameter(GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		m_depthTexture->SetParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		m_depthTexture->SetParameter(GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture->GetHandle(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_BACK);
		
		auto effect = g_context.m_resourceManager->LoadEffect("Renderer/Shadow");
		m_technique = effect->GetTechniques()[0];
	}

	void ShadowDevice::AddShadowcaster( const Render::Shadowcaster& p_shadowcaster, int p_index)
	{
		if(p_index > RENDER_MAX_SHADOWCASTERS)
			return;

		m_shadowcasters[p_index] = p_shadowcaster;
		m_numberOfShadowcasters++;
	}

	void ShadowDevice::Process()
	{

		m_numberOfShadowcasters = 0;
	}

}