#pragma once

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/NetworkTypes.h>

namespace RootForce
{
	struct AbilityRespawnComponent : public ECS::Component<AbilityRespawnComponent>
	{
		float Timer;
		AbilityInfo CurrentAbility;
		Network::UserID_t Claimed;

		AbilityRespawnComponent()
		{
			Timer = 0;
			CurrentAbility = AbilityInfo();
			Claimed = Network::ReservedUserID::NONE;
		}
	};

	class AbilityRespawnSystem : public ECS::EntitySystem
	{
	public:
		AbilityRespawnSystem(ECS::World* p_world, RootEngine::GameSharedContext* p_engineContext, std::string p_workingDir)
			: ECS::EntitySystem(p_world)
			, m_engineContext(p_engineContext), m_workingDir(p_workingDir), m_serverPeer(nullptr), m_clientPeer(nullptr)
		{ 
			SetUsage<RootForce::AbilityRespawnComponent>();
		}
		void Init();
		void ProcessEntity(ECS::Entity* p_entity);
		void LoadAbilities(std::string p_abilityPack);
		void AttatchComponentToPoints();
		void SetServerPeerInterface(RakNet::RakPeerInterface* p_serverPeer) { m_serverPeer = p_serverPeer; }
		void SetClientPeerInterface(RakNet::RakPeerInterface* p_clientPeer) { m_clientPeer = p_clientPeer; }
	private:

		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::AbilityRespawnComponent> m_respawn;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::Network::NetworkComponent> m_network;
		ECS::ComponentMapper<RootForce::Script> m_script;
		std::vector<std::string> m_levelAbilities;
		std::string m_workingDir;

		RakNet::RakPeerInterface* m_serverPeer;
		RakNet::RakPeerInterface* m_clientPeer;
	};
}