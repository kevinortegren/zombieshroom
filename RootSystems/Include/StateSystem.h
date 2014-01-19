#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\EntitySystem.h>

#include <RootSystems/Include/Orientation.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/PlayerSystem.h>

namespace RootSystems
{
	class StateSystem : public ECS::EntitySystem
	{
	public:
		StateSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext) 
		{ 
			SetUsage<RootForce::Collision>();
			SetUsage<RootForce::Transform>();
			SetUsage<RootForce::StateComponent>();
			SetUsage<RootForce::PlayerPhysics>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);
	private:
		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::Collision> m_collision;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::StateComponent> m_state;
		ECS::ComponentMapper<RootForce::PlayerPhysics> m_physic;
	};
}