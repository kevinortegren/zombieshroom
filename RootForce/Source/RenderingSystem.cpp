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

	void RenderingSystem::ProcessEntity(ECS::Entity* p_entity, float dt)
	{
		if(m_renderer && m_logger)
		{
			Transform* transform = m_transforms.Get(p_entity);
			Renderable* renderable = m_renderables.Get(p_entity);

			if(renderable->m_material.m_effect == nullptr)
			{
				m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Renderable of Entity %i has no effect", p_entity->GetId());
				return;
			}
			if(renderable->m_mesh == nullptr)
			{
				m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "Renderable of Entity %i has no mesh", p_entity->GetId());
				return;
			}

			Render::Uniforms uniforms;
			//uniforms.m_world = glm::translate(glm::mat4(1.0f), transform->m_position);
			//uniforms.m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(uniforms.m_world))));

			uniforms.m_world = glm::translate(glm::mat4(1.0f), transform->m_position);
			uniforms.m_world = glm::rotate(uniforms.m_world, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			uniforms.m_world = glm::scale(uniforms.m_world, transform->m_scale);
			uniforms.m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(uniforms.m_world))));


			//todo: add loop for multiple submeshes?
			Render::RenderJob job;
			job.m_mesh = renderable->m_mesh;
			job.m_uniforms = uniforms;
			job.m_material = &renderable->m_material;

			m_renderer->AddRenderJob(job);
		}
	}

	void RenderingSystem::End()
	{
		m_renderer->Render();
		m_renderer->RenderLines();
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