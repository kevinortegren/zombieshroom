#pragma once
#ifndef COMPILE_LEVEL_EDITOR
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/Component.h>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/ParticleSystem.h>

namespace RootForce
{
	struct AbilityRespawnComponent : public ECS::Component<AbilityRespawnComponent>
	{
		float Timer;
		AbilityInfo CurrentAbility;
		Network::UserID_t Claimed;

		AbilityRespawnComponent()
			: Timer(0), Claimed(Network::ReservedUserID::NONE)
		{}
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

		void HideSpawnpoint(ECS::Entity* p_entity);
		void RevealSpawnpoint(ECS::Entity* p_entity);

		void NewCurrentAbility(ECS::Entity* p_entity);
		void CreateRenderComponent(ECS::Entity* p_entity);
		void CreateCollisionComponents(ECS::Entity* p_entity);
		void CreateParticleEmitter(ECS::Entity* p_entity);

		RootEngine::GameSharedContext* m_engineContext;

		ECS::ComponentMapper<RootForce::AbilityRespawnComponent> m_respawn;
		ECS::ComponentMapper<RootForce::Transform> m_transform;
		ECS::ComponentMapper<RootForce::Network::NetworkComponent> m_network;
		ECS::ComponentMapper<RootForce::Script> m_script;
		ECS::ComponentMapper<RootForce::ParticleEmitter> m_particle;
		std::vector<std::string> m_levelAbilities;
		std::string m_workingDir;

		RakNet::RakPeerInterface* m_serverPeer;
		RakNet::RakPeerInterface* m_clientPeer;
	};
}
#endif