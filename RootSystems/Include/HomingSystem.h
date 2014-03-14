#ifndef COMPILE_LEVEL_EDITOR
#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

namespace RootForce
{
	struct HomingComponent : public ECS::Component<HomingComponent>
	{
		Network::NetworkEntityID TargetID;
		glm::vec3 TargetPosition;
		float Controllability;
		float Speed;
		HomingComponent()
			: TargetID(),
			  TargetPosition(glm::vec3(0,0,0)),
			  Controllability(0.0f)
		{}
	};

	class HomingSystem : public ECS::EntitySystem
	{
	public:
		HomingSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{ 
			SetUsage<RootForce::HomingComponent>();
			//SetUsage<RootForce::Collision>();
			SetUsage<RootForce::Physics>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);

	private:


		RootEngine::GameSharedContext* m_engineContext;
		//ECS::ComponentMapper<RootForce::Collision> m_collision;
		ECS::ComponentMapper<RootForce::Physics> m_physics;
		ECS::ComponentMapper<RootForce::HomingComponent> m_target;
	};
}

#endif