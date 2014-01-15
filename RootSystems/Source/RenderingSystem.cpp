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
		Transform* transform = m_transforms.Get(p_entity);
		Renderable* renderable = m_renderables.Get(p_entity);

		if(!renderable->m_material)
		{
			m_logger->LogText(LogTag::GAME, LogLevel::MASS_DATA_PRINT, "Renderable of Entity %i has no material", p_entity->GetId());
			return;
		}
		if(renderable->m_material->m_effect == nullptr)
		{
			m_logger->LogText(LogTag::GAME, LogLevel::MASS_DATA_PRINT, "Renderable of Entity %i has no effect", p_entity->GetId());
			return;
		}
		if(renderable->m_model == nullptr)
		{
			m_logger->LogText(LogTag::GAME, LogLevel::MASS_DATA_PRINT, "Renderable of Entity %i has no model", p_entity->GetId());
			return;
		}

		glm::mat4x4 translation = glm::translate(glm::mat4(1.0f), transform->m_position);
		//glm::mat4x4 rotation = glm::inverse(renderable->m_model->m_transform) * glm::rotate(glm::mat4(1.0f), transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis()) * renderable->m_model->m_transform;
		glm::mat4x4 rotation = glm::rotate(glm::mat4(1.0f), transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
		glm::mat4x4 scale = glm::scale(transform->m_scale);

		m_matrices[p_entity].m_model = translation * rotation * scale;

		m_matrices[p_entity].m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_matrices[p_entity].m_model))));

		//m_matrices[p_entity].m_model = m_matrices[p_entity].m_model;
		m_matrices[p_entity].m_model = m_matrices[p_entity].m_model;


		for(auto itr = renderable->m_model->m_meshes.begin(); itr != renderable->m_model->m_meshes.end(); ++itr)
		{
			Render::RenderJob job;
			job.m_mesh = (*itr);

			job.m_params = renderable->m_params;
			job.m_params[Render::Semantic::MODEL] = &m_matrices[p_entity].m_model;
			job.m_params[Render::Semantic::NORMAL] = &m_matrices[p_entity].m_normal;

			job.m_material = renderable->m_material;
			job.m_renderPass = renderable->m_pass;

			m_renderer->AddRenderJob(job);
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