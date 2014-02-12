#pragma once

#include <RakNet/RakPeerInterface.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

namespace RootSystems
{
	class RespawnSystem : public ECS::EntitySystem
	{
	public:
		RespawnSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext)
			, m_serverPeer(nullptr)
			, m_clientPeer(nullptr)
		{ 
			SetUsage<RootForce::HealthComponent>();
			SetUsage<RootForce::Collision>();
			SetUsage<RootForce::Network::NetworkComponent>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);
		void LoadSpawnPoints();

		void SetServerPeer(RakNet::RakPeerInterface* p_serverPeer);
		void SetClientPeer(RakNet::RakPeerInterface* p_clientPeer);
	private:
		static const glm::vec3 DEFAULT_SPAWN_POINT;

		int GetRandomSpawnpoint();
		RootForce::Transform* GetSpawnpointTransform(int index);
		void Respawn(int index, ECS::Entity* p_player);

		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::HealthComponent> m_health;
		ECS::ComponentMapper<RootForce::Collision> m_collision;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::Network::NetworkComponent> m_network;

		ECS::GroupManager::GroupRange m_spawnPoints;

		RakNet::RakPeerInterface* m_serverPeer;
		RakNet::RakPeerInterface* m_clientPeer;
	};
}