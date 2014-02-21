#include <RenderingSystem.h>
#include <RootEngine/Render/Include/Renderer.h>

namespace RootForce
{
	Renderable::~Renderable()
	{
		std::cout << "Renderable destruct." << std::endl;
	}

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

		m_matrices[p_entity].m_model = glm::translate(glm::mat4(1.0f), transform->m_position);
		m_matrices[p_entity].m_model = glm::rotate(m_matrices[p_entity].m_model, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
		m_matrices[p_entity].m_model = glm::scale(m_matrices[p_entity].m_model, transform->m_scale);
		m_matrices[p_entity].m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_matrices[p_entity].m_model))));

		Render::RenderJob job;	
		job.m_mesh = renderable->m_model->m_meshes[0];
		job.m_shadowMesh = renderable->m_model->m_meshes[0];
		job.m_material = renderable->m_material;	
		job.m_params = renderable->m_params;
		job.m_params[Render::Semantic::MODEL] = &m_matrices[p_entity].m_model;
		job.m_params[Render::Semantic::NORMAL] = &m_matrices[p_entity].m_normal;
		job.m_flags = renderable->m_renderFlags;
		job.m_renderPass = renderable->m_pass;
		m_renderer->AddRenderJob(job);
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