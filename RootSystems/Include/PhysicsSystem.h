#pragma once

#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/Include/Logging/Logging.h>

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/CollisionSystem.h>

namespace RootForce
{
	struct Physics : public ECS::Component<Physics>
	{
		Physics()
			: m_mass(0) {}

		float m_mass;
		glm::vec3 m_velocity;
	};

	struct PhysicsSystem : public ECS::EntitySystem
	{
		PhysicsSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			//SetUsage<Physics>();
			SetUsage<Collision>();
			SetUsage<Transform>();
		}
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		void SetPhysicsInterface(RootEngine::Physics::PhysicsInterface* p_physics);
		void SetLoggingInterface(Logging::LoggingInterface* p_logger);

		void AddStaticEntities();

		ECS::ComponentMapper<Physics> m_physicsAccessor;
		ECS::ComponentMapper<Collision> m_collisions;
		ECS::ComponentMapper<Transform> m_transforms;
		
	private:
		RootEngine::Physics::PhysicsInterface* m_physics;
		Logging::LoggingInterface* m_logger;
	};

	// Alt. DoThisBeforePhysicsSystemSystem.
	struct PhysicsTransformCorrectionSystem : public ECS::EntitySystem
	{
		PhysicsTransformCorrectionSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			//SetUsage<Physics>();
			SetUsage<Collision>();
			SetUsage<Transform>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	private:
		ECS::ComponentMapper<Physics> m_physicsAccessor;
		ECS::ComponentMapper<Collision> m_collisions;
		ECS::ComponentMapper<Transform> m_transforms;
	};
}