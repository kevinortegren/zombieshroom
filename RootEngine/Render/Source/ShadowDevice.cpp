#include <RootEngine/Render/Include/ShadowDevice.h>

#include <RootEngine/Render/Include/RenderExtern.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace Render
{
	ShadowDevice::ShadowDevice()
		: m_shadowJobAllocator(10000 * sizeof(ShadowJob))
	{
		m_depthTexture = nullptr;
		m_numberOfShadowcasters = 0;
		m_showShadows = true;
		m_depthTextureArray = 0;
	}

	ShadowDevice::~ShadowDevice()
	{
		if(m_depthTexture)
		{
			delete m_depthTexture;
		}
		for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
		{
			glDeleteFramebuffers(1, &m_framebuffers[i]);
		}
	}

	void ShadowDevice::Init(GLRenderer* p_renderer, int p_width, int p_height, BufferInterface* p_cameraBuffer, BufferInterface* p_uniforms)
	{
		m_width = p_width;
		m_height = p_height;
		m_cameraBuffer = p_cameraBuffer;
		m_uniforms = p_uniforms;

		glGenTextures(1, &m_depthTextureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_depthTextureArray);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32F, m_width, m_height, RENDER_SHADOW_CASCADES);
		
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
		{
			glGenFramebuffers(1, &m_framebuffers[i]);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[i]);
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTextureArray, 0, i);
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
		}
		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_BACK);

		m_shadowEffect = g_context.m_resourceManager->LoadEffect("Renderer/Shadow");

		glGenSamplers(1, &m_samplerObjectPCF);
		glGenSamplers(1, &m_samplerObjectFloat);

		glSamplerParameteri(m_samplerObjectPCF, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(m_samplerObjectPCF, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(m_samplerObjectPCF, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(m_samplerObjectPCF, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_samplerObjectPCF, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glSamplerParameteri(m_samplerObjectPCF, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glSamplerParameteri(m_samplerObjectFloat, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(m_samplerObjectFloat, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(m_samplerObjectFloat, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(m_samplerObjectFloat, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void ShadowDevice::AddShadowcaster( const Render::Shadowcaster& p_shadowcaster, int p_index)
	{
		if(p_index > RENDER_MAX_SHADOWCASTERS)
			return;

		m_shadowcasters[p_index] = p_shadowcaster;
		m_numberOfShadowcasters++;
	}

	static bool SortShadowJobs(ShadowJob* a, ShadowJob* b)
	{
		return a->m_technique < b->m_technique;
	};

	void ShadowDevice::Process()
	{
		m_numberOfShadowcasters = 0;

		// Clear framebuffers.
		for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[i]);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}
	
		if(m_showShadows)
		{
			std::sort(m_shadowJobs.begin(), m_shadowJobs.end(), SortShadowJobs);

			ShadowTechnique::ShadowTechnique currentTechnique = ShadowTechnique::SHADOW_NONE;

			for(auto itr = m_shadowJobs.begin(); itr != m_shadowJobs.end(); ++itr)
			{
				if((*itr)->m_technique != currentTechnique)
				{
					currentTechnique = (*itr)->m_technique;

					std::shared_ptr<Technique> tech = m_shadowEffect->GetTechniques()[currentTechnique-1];
					tech->GetPrograms()[0]->Apply();

					for(auto param = (*itr)->m_params.begin(); param != (*itr)->m_params.end(); ++param)
					{	
						m_uniforms->BufferSubData(tech->m_uniformsParams[param->first], Render::GLRenderer::s_sizes[param->first], param->second);
					}
				}

				(*itr)->m_mesh->Bind();

				for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[i]);
					glDrawBuffers(0, NULL);
					m_cameraBuffer->BufferSubData(0, sizeof(glm::mat4), &m_shadowcasters[0].m_viewProjections[i]);

					(*itr)->m_mesh->Draw();
				}
	
				(*itr)->m_mesh->Unbind();
			}
		}

		// Deconstruct the job batch.
		for(auto itr = m_shadowJobs.begin(); itr != m_shadowJobs.end(); ++itr)
		{
			(*itr)->~ShadowJob();
		}

		m_shadowJobAllocator.Clear();
		m_shadowJobs.clear();	
	}

	void ShadowDevice::Resize(int p_width, int p_height)
	{
		if(m_depthTextureArray)
		{
			glDeleteTextures(1, &m_depthTextureArray);
			m_depthTextureArray = 0;
		}
		for(int i = 0; i < RENDER_SHADOW_CASCADES; i++)
		{
			if(m_framebuffers[i])
			{
				glDeleteFramebuffers(1, &m_framebuffers[i]);
				m_framebuffers[i] = 0;
			}
		}

		glDeleteSamplers(1, &m_samplerObjectPCF);
		glDeleteSamplers(1, &m_samplerObjectFloat);

		glBindTexture(GL_TEXTURE_2D_ARRAY, m_depthTextureArray);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32F, m_width, m_height, RENDER_SHADOW_CASCADES);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
}