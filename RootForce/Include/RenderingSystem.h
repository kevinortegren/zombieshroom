#pragma once

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/Logging/Logging.h>

#include <RootForce\Include\ECS/World.h>
#include <RootForce\Include\Transform.h>
#include <RootForce\Include\Renderable.h>

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
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity, float dt);
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