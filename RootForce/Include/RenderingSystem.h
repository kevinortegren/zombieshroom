#pragma once

#include <RootEngine/Render/Include/Renderer.h>

#include <ECS/World.h>
#include <Transform.h>
#include <Renderable.h>

namespace RootForce
{

	struct RenderingSystem : public ECS::ComponentSystem
	{
		RenderingSystem(ECS::World* p_world, Render::RendererInterface* p_renderer)
			: ECS::ComponentSystem(p_world)
		{
			m_renderer = p_renderer;
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

		void ProcessEntity(ECS::Entity* p_entity)
		{
			Transform* transform = m_transforms.Get(p_entity);
			Renderable* renderable = m_renderables.Get(p_entity);

			Render::Uniforms uniforms;
			uniforms.m_world = glm::translate(glm::mat4(1.0f), transform->m_position);

			Render::RenderJob job;
			job.m_mesh = renderable->m_mesh;
			job.m_uniforms = &uniforms;
			//job.m_effect = m_engineContext.m_resourceManager->GetEffect("DiffuseTexture");

			m_renderer->AddRenderJob(&job);
		}
	private:
		Render::RendererInterface* m_renderer;
	};
}