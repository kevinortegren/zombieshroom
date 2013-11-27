#pragma once

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/Logging/Logging.h>

#include <ECS/World.h>
#include <Transform.h>
#include <Renderable.h>

namespace RootForce
{

	struct RenderingSystem : public ECS::ComponentSystem
	{
		RenderingSystem(ECS::World* p_world)
			: ECS::ComponentSystem(p_world)
		{
			SetUsage<Renderable>();
			SetUsage<Transform>();
		}
	

		ECS::ComponentMapper<Renderable> m_renderables;
		ECS::ComponentMapper<Transform> m_transforms;
	
		void Init()
		{
			m_renderables.Init(m_world->GetEntityManager());
			m_transforms.Init(m_world->GetEntityManager());
		}

		void Begin()
		{

		}

		void ProcessEntity(ECS::Entity* p_entity, float dt)
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
				uniforms.m_world = glm::translate(glm::mat4(1.0f), transform->m_position);
				uniforms.m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(uniforms.m_world))));

				//todo: add loop for multiple submeshes?
				Render::RenderJob job;
				job.m_mesh = renderable->m_mesh;
				job.m_uniforms = uniforms;
				job.m_material = &renderable->m_material;

				m_renderer->AddRenderJob(job);
			}
		}

		void End()
		{

		}

		void SetRendererInterface(Render::RendererInterface* p_renderer)
		{
			m_renderer = p_renderer;
		}

		void SetLoggingInterface(Logging::LoggingInterface* p_logger)
		{
			m_logger = p_logger;
		}
	private:
		Render::RendererInterface* m_renderer;
		Logging::LoggingInterface* m_logger;
	};
}