#pragma once

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/Logging/Logging.h>

#include <Utility\ECS\Include\World.h>
#include <Transform.h>
#include <Renderable.h>

namespace RootForce
{

	struct RenderingSystem : public ECS::EntitySystem
	{
		RenderingSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<Renderable>();
			SetUsage<Transform>();
			m_renderer = nullptr;
			m_logger = nullptr;
		}
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		void SetRendererInterface(Render::RendererInterface* p_renderer);
		void SetLoggingInterface(Logging::LoggingInterface* p_logger);

		ECS::ComponentMapper<Renderable> m_renderables;
		ECS::ComponentMapper<Transform> m_transforms;
	
		
	private:
		Render::RendererInterface* m_renderer;
		Logging::LoggingInterface* m_logger;
	};
}