#ifndef COMPILE_LEVEL_EDITOR
#include "RespawnSystem.h"
#include <RakNet/GetTime.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

namespace RootSystems
{

	void RespawnSystem::Init()
	{
		m_health.Init(m_world->GetEntityManager());
		m_collision.Init(m_world->GetEntityManager());
		m_transform.Init(m_world->GetEntityManager());
		m_network.Init(m_world->GetEntityManager());
	}


	void RespawnSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		float dt = m_world->GetDelta();

		RootForce::HealthComponent* health = m_health.Get(p_entity);
		RootForce::Collision* collision = m_collision.Get(p_entity);
		RootForce::Transform* transform = m_transform.Get(p_entity);
		RootForce::Network::NetworkComponent* network = m_network.Get(p_entity);
		

		//If the player has 0 health and is not already dead, kill him
		if(health->Health <= 0 && !health->IsDead)
		{
			health->IsDead = true;
			health->RespawnDelay = 3.0f;
		}

		//Check if the player wants to respawn, only allow him to respawn after a 3 second delay.
		//This is to stop accidental respawn clicks when a player has just died(one might want to check the score first)
		if(health->IsDead && health->WantsRespawn)
		{
			if(health->RespawnDelay <= 0)
			{

				// TODO: Find a spawn point
				unsigned spawnpointIndex = GetRandomSpawnpoint();
				RootForce::Transform* spawnpoint = GetSpawnpointTransform(spawnpointIndex);
				if(spawnpoint != nullptr)
				{
					transform->m_position = spawnpoint->m_position;
					transform->m_orientation = spawnpoint->m_orientation;
				}
				else
					transform->m_position = glm::vec3(0,100,0);

				health->Health = 100;
				health->IsDead = false;
				m_engineContext->m_physics->DeactivateRagdoll(*(collision->m_handle));

				// If server, send a message to all clients about the respawn.
				if (m_serverPeer != nullptr)
				{
					RootForce::NetworkMessage::SpawnUser m;
					m.User = network->ID.UserID;
					m.SpawnPointIndex = spawnpointIndex;

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) ID_TIMESTAMP);
					bs.Write(RakNet::GetTime());
					bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::SpawnUser);
					m.Serialize(true, &bs);

					m_serverPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				}
			}
			health->WantsRespawn = false;
		}

		if(health->IsDead)
			health->RespawnDelay -= dt;
	}

	unsigned RespawnSystem::GetRandomSpawnpoint()
	{
		unsigned numspawns = 0;
		for(std::multimap<std::string, ECS::Entity*>::iterator itr = m_spawnPoints.first; itr != m_spawnPoints.second; ++itr, ++numspawns)
			;

		if(numspawns==0)
		{
			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "No spawnpoints found!");
			return -1;
		}

		return rand() % numspawns;
	}

	RootForce::Transform* RespawnSystem::GetSpawnpointTransform(unsigned index)
	{
		if (index == -1)
			return nullptr;

		unsigned numspawns = 0;
		for(std::multimap<std::string, ECS::Entity*>::iterator itr = m_spawnPoints.first; itr != m_spawnPoints.second; ++itr, ++numspawns)
		{
			if(numspawns == index)
			{
				float x,y,z;
				x = m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second)->m_position.x;
				y = m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second)->m_position.y;
				z = m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second)->m_position.z;
				m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Found spawnpoint number: %d position: %f %f %f",index,x,y,z );

				return m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second);
			}
		}

		return nullptr;
	}

	void RespawnSystem::LoadSpawnPoints()
	{
		m_spawnPoints = m_world->GetGroupManager()->GetEntitiesInGroup("SpawnPoint");
	}

	void RespawnSystem::SetServerPeer(RakNet::RakPeerInterface* p_serverPeer)
	{
		m_serverPeer = p_serverPeer;
	}

}
#endif
