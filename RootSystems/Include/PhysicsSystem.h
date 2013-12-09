#pragma once

#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/Include/Logging/Logging.h>

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Physics.h>

namespace RootForce
{

	struct PhysicsSystem : public ECS::EntitySystem
	{
		PhysicsSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<PhysicsAccessor>();
			SetUsage<Transform>();
		}
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		void SetPhysicsInterface(RootEngine::Physics::PhysicsInterface* p_physics);
		void SetLoggingInterface(Logging::LoggingInterface* p_logger);

		ECS::ComponentMapper<PhysicsAccessor> m_physicsAccessor;
		ECS::ComponentMapper<Transform> m_transforms;
	
		
	private:
		RootEngine::Physics::PhysicsInterface* m_physics;
		Logging::LoggingInterface* m_logger;
	};
}