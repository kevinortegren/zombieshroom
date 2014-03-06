#ifndef COMPILE_LEVEL_EDITOR
#include "StatChangeSystem.h"
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RakNet/GetTime.h>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

void RootForce::StatChangeSystem::Init()
{
	m_statChange.Init(m_world->GetEntityManager());
	m_networks.Init(m_world->GetEntityManager());
}

void RootForce::StatChangeSystem::ProcessEntity( ECS::Entity* p_entity )
{
	StatChange* change = m_statChange.Get(p_entity);
	Network::NetworkComponent* network = m_networks.Get(p_entity);

	if(m_serverPeer != nullptr)
	{
		if(change->SpeedChangeTime > 0)
			change->SpeedChangeTime -= m_world->GetDelta();
		if(change->JumpHeightChangeTime > 0)
			change->JumpHeightChangeTime -= m_world->GetDelta();
		if(change->KnockbackResistanceTime > 0)
			change->KnockbackResistanceTime -= m_world->GetDelta();
		if(change->DamageResistanceTime > 0)
			change->DamageResistanceTime -= m_world->GetDelta();
	}

	if (change->SpeedChangeTime <= 0 && change->SpeedChange != 1.0f)
	{
		if (m_serverPeer != nullptr)
		{
			SendServerStuff(network, 0);
		}
		change->SpeedChangeTime = 0.0f;
		change->SpeedChange = 1.0f;
	}
	if (change->JumpHeightChangeTime <= 0 && change->JumpHeightChange != 1.0f)
	{
		if (m_serverPeer != nullptr)
		{
			SendServerStuff(network, 1);
		}
		change->JumpHeightChangeTime = 0.0f;
		change->JumpHeightChange = 1.0f;
	}
	if (change->KnockbackResistanceTime <= 0 && change->KnockbackResistance != 1.0f)
	{
		if (m_serverPeer != nullptr)
		{
			SendServerStuff(network, 2);
		}
		change->KnockbackResistanceTime = 0.0f;
		change->KnockbackResistance = 1.0f;
	}
	if (change->DamageResistanceTime <= 0 && change->DamageResistance != 1.0f)
	{
		if (m_serverPeer != nullptr)
		{
			SendServerStuff(network, 3);
		}
		change->DamageResistanceTime = 0.0f;
		change->DamageResistance = 1.0f;
	}
}

void RootForce::StatChangeSystem::SetServerPeer( RakNet::RakPeerInterface* p_serverPeer )
{
	m_serverPeer = p_serverPeer;
}

void RootForce::StatChangeSystem::Begin()
{
	int asdf = 0;
}

void RootForce::StatChangeSystem::SendServerStuff( Network::NetworkComponent* p_network, int p_statToChange )
{
	NetworkMessage::StatChangeTimeUp m;
	m.UserID = p_network->ID.UserID;
	m.StatToReset = p_statToChange;
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)ID_TIMESTAMP);
	bs.Write(RakNet::GetTime());
	bs.Write((RakNet::MessageID)NetworkMessage::MessageType::StatChangeTimeUp);
	m.Serialize(true, &bs);

	m_serverPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

#endif