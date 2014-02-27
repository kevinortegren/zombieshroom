#ifndef COMPILE_LEVEL_EDITOR
#include "TimerSystem.h"
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RakNet/GetTime.h>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

void RootForce::TimerSystem::Init()
{
	m_timers.Init(m_world->GetEntityManager());
	m_networks.Init(m_world->GetEntityManager());
	m_scripts.Init(m_world->GetEntityManager());
}

void RootForce::TimerSystem::ProcessEntity( ECS::Entity* p_entity )
{
	TimerComponent* timer = m_timers.Get(p_entity);
	Network::NetworkComponent* network = m_networks.Get(p_entity);
	Script* script = m_scripts.Get(p_entity);

	timer->TimeLeft -= m_world->GetDelta();

	if (timer->TimeLeft <= 0)
	{
		if (m_serverPeer != nullptr)
		{
			timer->TimeUp = true;

			NetworkMessage::TimeUp m;
			m.ID = network->ID;
			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID)ID_TIMESTAMP);
			bs.Write(RakNet::GetTime());
			bs.Write((RakNet::MessageID)NetworkMessage::MessageType::TimeUp);
			m.Serialize(true, &bs);

			m_serverPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}
	}

	if (timer->TimeUp)
	{
		//g_engineContext.m_script->SetFunction(script->Name, "OnDestroy");
		//g_engineContext.m_script->AddParameterUserData(p_entity, sizeof(ECS::Entity*), "Entity");
		//g_engineContext.m_script->ExecuteScript();

		Network::DeleteEntities(g_networkEntityMap, network->ID, m_world->GetEntityManager());
		//m_world->GetEntityManager()->RemoveAllComponents(p_entity);
		//m_world->GetEntityManager()->RemoveEntity(p_entity);
	}
}

void RootForce::TimerSystem::SetServerPeer( RakNet::RakPeerInterface* p_serverPeer )
{
	m_serverPeer = p_serverPeer;
}

void RootForce::TimerSystem::Begin()
{
	int asdf = 0;
}

#endif