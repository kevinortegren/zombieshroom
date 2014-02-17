#ifndef COMPILE_LEVEL_EDITOR
#pragma once

#include <RakNet/RakPeerInterface.h>
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
	class ActionSystem : public ECS::EntitySystem
	{
	public:
		ActionSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext)
			, m_serverPeer(nullptr)
			, m_clientPeer(nullptr)
		{
			SetUsage<RootForce::PlayerActionComponent>();
			SetUsage<RootForce::Network::NetworkComponent>();
			SetUsage<RootForce::Animation>();
			SetUsage<RootForce::Collision>();
			SetUsage<RootForce::Transform>();
			SetUsage<RootForce::StateComponent>();
			SetUsage<RootForce::PlayerPhysics>();
			SetUsage<RootForce::PlayerComponent>();
			SetUsage<RootForce::HealthComponent>();

		}
		void Init();

		void ProcessEntity(ECS::Entity* p_entity);
        
		void UpdateAimingDevice(bool m_inMenu);
		void SetServerPeerInterface(RakNet::RakPeerInterface* p_serverPeer);
		void SetClientPeerInterface(RakNet::RakPeerInterface* p_clientPeer);
	private:

		RootEngine::GameSharedContext* m_engineContext;
		RakNet::RakPeerInterface* m_serverPeer;
		RakNet::RakPeerInterface* m_clientPeer;

		ECS::ComponentMapper<RootForce::PlayerActionComponent> m_action;
		ECS::ComponentMapper<RootForce::Network::NetworkComponent> m_network;
		ECS::ComponentMapper<RootForce::Animation> m_animation;
		ECS::ComponentMapper<RootForce::Collision> m_collision;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::StateComponent> m_state;
		ECS::ComponentMapper<RootForce::PlayerPhysics> m_physic;
		ECS::ComponentMapper<RootForce::PlayerComponent> m_player;
		ECS::ComponentMapper<RootForce::HealthComponent> m_health;

		void AbilitySwitch(ECS::Entity* p_entity, bool p_isPush);
	};

}
#endif
