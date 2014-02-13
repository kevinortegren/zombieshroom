#ifndef COMPILE_LEVEL_EDITOR
#include "RespawnSystem.h"
#include <RakNet/GetTime.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

namespace RootSystems
{
	const glm::vec3 RespawnSystem::DEFAULT_SPAWN_POINT(0, 100, 0);

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

		// Check if a spawn point has been received from the server as a response to a respawn request.
		if (m_clientPeer != nullptr && health->SpawnIndex != -1)
		{
			Respawn(health->SpawnIndex, p_entity);
			health->SpawnIndex = -1;
		}

		//Check if the player wants to respawn, only allow him to respawn after a 3 second delay.
		//This is to stop accidental respawn clicks when a player has just died(one might want to check the score first)
		if(health->IsDead && health->WantsRespawn)
		{
			if(health->RespawnDelay <= 0.0f)
			{
				// If we are a client and this is us, send a respawn request.
				if (m_clientPeer != nullptr && p_entity == m_world->GetTagManager()->GetEntityByTag("Player"))
				{
					RootForce::NetworkMessage::RespawnRequest m;
					m.User = network->ID.UserID;

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) ID_TIMESTAMP);
					bs.Write(RakNet::GetTime());
					bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::RespawnRequest);
					m.Serialize(true, &bs);

					m_clientPeer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				}
				
				// If we are a server, call on spawn function and tell every client of the respawn.
				if (m_serverPeer != nullptr)
				{
					// Call on spawn function with random spawn index
					health->SpawnIndex = GetRandomSpawnpoint();
					Respawn(health->SpawnIndex, p_entity);

					// Broadcast the player spawning
					RootForce::NetworkMessage::SpawnUser m;
					m.User = network->ID.UserID;
					m.SpawnPointIndex = health->SpawnIndex;

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

	int RespawnSystem::GetRandomSpawnpoint()
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

	RootForce::Transform* RespawnSystem::GetSpawnpointTransform(int index)
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
				//m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Found spawnpoint number: %d position: %f %f %f",index,x,y,z );

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

	void RespawnSystem::SetClientPeer(RakNet::RakPeerInterface* p_clientPeer)
	{
		m_clientPeer = p_clientPeer;
	}

	void RespawnSystem::Respawn(int index, ECS::Entity* p_player)
	{
		// Get the relevant components
		RootForce::HealthComponent* health = m_health.Get(p_player);
		RootForce::Collision* collision = m_collision.Get(p_player);
		RootForce::Transform* transform = m_transform.Get(p_player);
		RootForce::Network::NetworkComponent* network = m_network.Get(p_player);

		// Set the spawn position
		RootForce::Transform* spawnpoint = GetSpawnpointTransform(index);
		if(spawnpoint != nullptr)
		{
			transform->m_position = spawnpoint->m_position;
			transform->m_orientation = spawnpoint->m_orientation;
		}
		else
		{
			transform->m_position = DEFAULT_SPAWN_POINT;
		}

		m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Spawning player (%u) at spawn point %d: (%f, %f, %f)", network->ID.UserID, index, transform->m_position.x, transform->m_position.y, transform->m_position.z);

		// Resurrect the player
		health->Health = 100;
		health->IsDead = false;
		m_engineContext->m_physics->DeactivateRagdoll(*(collision->m_handle));
	}
}
#endif
