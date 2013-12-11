#include <RenderingSystem.h>

namespace RootForce
{
	void RenderingSystem::Init()
	{
		m_renderables.Init(m_world->GetEntityManager());
		m_transforms.Init(m_world->GetEntityManager());
	}

	void RenderingSystem::Begin()
	{

	}

	void RenderingSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		if(m_renderer && m_logger)
		{
			Transform* transform = m_transforms.Get(p_entity);
			Renderable* renderable = m_renderables.Get(p_entity);

			if(renderable->m_material.m_effect == nullptr)
			{
				m_logger->LogText(LogTag::GAME, LogLevel::MASS_DATA_PRINT, "Renderable of Entity %i has no effect", p_entity->GetId());
				return;
			}
			if(renderable->m_model == nullptr)
			{
				m_logger->LogText(LogTag::GAME, LogLevel::MASS_DATA_PRINT, "Renderable of Entity %i has no model", p_entity->GetId());
				return;
			}

			Render::Uniforms uniforms;
			//uniforms.m_world = glm::translate(glm::mat4(1.0f), transform->m_position);
			//uniforms.m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(uniforms.m_world))));

			uniforms.m_world = glm::translate(glm::mat4(1.0f), transform->m_position);
			uniforms.m_world = glm::rotate(uniforms.m_world, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			uniforms.m_world = glm::scale(uniforms.m_world, transform->m_scale);
			uniforms.m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(uniforms.m_world))));

			for(auto itr = renderable->m_model->m_meshes.begin(); itr != renderable->m_model->m_meshes.end(); ++itr)
			{
				Render::RenderJob job;
				job.m_mesh = (*itr);
				job.m_uniforms = uniforms;
				job.m_material = &renderable->m_material;

				m_renderer->AddRenderJob(job);
			}
		}
	}

	void RenderingSystem::End()
	{

	}

	void RenderingSystem::SetRendererInterface(Render::RendererInterface* p_renderer)
	{
		m_renderer = p_renderer;
	}

	void RenderingSystem::SetLoggingInterface(Logging::LoggingInterface* p_logger)
	{
		m_logger = p_logger;
	}
}