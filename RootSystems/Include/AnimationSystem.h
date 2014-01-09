#pragma once

#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>

namespace RootForce
{
	struct Animation : public ECS::Component<Animation>
	{
		Animation(){}
	};

	struct AnimationSystem : public ECS::EntitySystem
	{
		AnimationSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world), m_time(0)
		{
			SetUsage<Animation>();
			SetUsage<Renderable>();
			m_logger = nullptr;
		}
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		void SetLoggingInterface(Logging::LoggingInterface* p_logger);

		ECS::ComponentMapper<Animation> m_animations;
		ECS::ComponentMapper<Renderable> m_renderables;

	private:
		Logging::LoggingInterface* m_logger;

		float m_time;
	};
}