#pragma once

#include <RootEngine\Render\Include\Renderer.h>
#include <RootEngine\Include\ModelImporter.h>
#include <RootEngine\Include\Logging\Logging.h>

#include <Utility\ECS\Include\World.h>
#include <Utility\ECS\Include\Shared\Transform.h>

namespace RootForce
{
	struct Renderable : public ECS::Component<Renderable>
	{
		Renderable()
			: m_model(nullptr) {}

		RootEngine::Model* m_model;
		Render::Material m_material;
	};

	struct RenderingSystem : public ECS::EntitySystem
	{
		RenderingSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<Renderable>();
			SetUsage<Transform>();
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