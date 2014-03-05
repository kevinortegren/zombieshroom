#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RakNet/RakPeerInterface.h>

namespace RootForce
{
    struct StatChange : public ECS::Component<StatChange>
	{
		float SpeedChange;
		float JumpHeightChange;
		float DamageResistance;
		float KnockbackResistance;
		float SpeedChangeTime;
		float JumpHeightChangeTime;
		float DamageResistanceTime;
		float KnockbackResistanceTime;
		StatChange()
			: SpeedChange(1.0f),
			JumpHeightChange(1.0f),
			DamageResistance(1.0f),
			KnockbackResistance(1.0f),
			SpeedChangeTime(0.0f),
			JumpHeightChangeTime(0.0f),
			DamageResistanceTime(0.0f),
			KnockbackResistanceTime(0.0f)
		{}
	};

	class StatChangeSystem : public ECS::EntitySystem
	{
	public:
		StatChangeSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world), m_serverPeer(nullptr)
		{
			SetUsage<StatChange>();
			SetUsage<Network::NetworkComponent>();
		}

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void SetServerPeer(RakNet::RakPeerInterface* p_serverPeer);
		

	private:
		ECS::ComponentMapper<StatChange> m_statChange;
		ECS::ComponentMapper<Network::NetworkComponent> m_networks;

		RakNet::RakPeerInterface* m_serverPeer;

		void SendServerStuff(Network::NetworkComponent* p_network, int p_statToChange);
	};
}