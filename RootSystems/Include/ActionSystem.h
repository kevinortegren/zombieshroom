#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility\ECS\Include\Component.h>
#include <Utility\ECS\Include\EntitySystem.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/BitStream.h>

#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Orientation.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/AnimationSystem.h>

#include <glm/glm.hpp>

namespace RootForce
{
	struct NetworkContext;
}


namespace RootSystems
{

	struct ActionEvent
	{
		uint32_t UserID;
		RootForce::Network::MessageType::MessageType ActionType;
		RakNet::BitStream ParameterStream;
	};

	class ActionSystem : public ECS::EntitySystem
	{
	public:
		ActionSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext) 
		{
			SetUsage<RootForce::PlayerActionComponent>();
			SetUsage<RootForce::Animation>();
			SetUsage<RootForce::Collision>();
			SetUsage<RootForce::Transform>();
			SetUsage<RootForce::StateComponent>();
			SetUsage<RootForce::PlayerPhysics>();
			SetUsage<RootForce::UserAbility>();
			SetUsage<RootForce::HealthComponent>();

		}
		void Init();

		void ProcessEntity(ECS::Entity* p_entity);
	private:
		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::PlayerActionComponent> m_action;
		ECS::ComponentMapper<RootForce::Animation> m_animation;
		ECS::ComponentMapper<RootForce::Collision> m_collision;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::StateComponent> m_state;
		ECS::ComponentMapper<RootForce::PlayerPhysics> m_physic;
		ECS::ComponentMapper<RootForce::UserAbility> m_ability;
		ECS::ComponentMapper<RootForce::HealthComponent> m_health;
	};

}