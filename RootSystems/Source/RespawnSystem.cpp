#ifndef COMPILE_LEVEL_EDITOR
#include "RespawnSystem.h"
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

namespace RootSystems
{

	void RespawnSystem::Init()
	{
		m_health.Init(m_world->GetEntityManager());
		m_collision.Init(m_world->GetEntityManager());

		m_spawnPoints = m_world->GetGroupManager()->GetEntitiesInGroup("SpawnPoint");
	}


	void RespawnSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		float dt = m_world->GetDelta();

		RootForce::HealthComponent* health = m_health.Get(p_entity);
		RootForce::Collision* collision = m_collision.Get(p_entity);
		

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
				RootForce::Transform* spawnpoint = GetRandomSpawnpoint();
				if(spawnpoint != nullptr)
					m_engineContext->m_physics->SetPosition(*(collision->m_handle), spawnpoint->m_position);
				else
					m_engineContext->m_physics->SetPosition(*(collision->m_handle), glm::vec3(0,100,0));

				health->Health = 100;
				health->IsDead = false;
			}
			health->WantsRespawn = false;
		}

		if(health->IsDead)
			health->RespawnDelay -= dt;
	}

	RootForce::Transform* RespawnSystem::GetRandomSpawnpoint()
	{
		unsigned numspawns = 0;
		for(std::multimap<std::string, ECS::Entity*>::iterator itr = m_spawnPoints.first; itr != m_spawnPoints.second; ++itr, ++numspawns)
			;
		if(numspawns==0)
		{
			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "No spawnpoints found!");
			return nullptr;
		}
		unsigned chosenspwn = rand()%numspawns;
		numspawns = 0;
		for(std::multimap<std::string, ECS::Entity*>::iterator itr = m_spawnPoints.first; itr != m_spawnPoints.second; ++itr, ++numspawns)
			if(numspawns == chosenspwn)
			{
				float x,y,z;
				x = m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second)->m_position.x;
				y = m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second)->m_position.y;
				z = m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second)->m_position.z;
				m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Found spawnpoint number: %d position: %f %f %f",chosenspwn,x,y,z );

				return m_world->GetEntityManager()->GetComponent<RootForce::Transform>((*itr).second);
			}

		m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::FATAL_ERROR, "No spawnpoint found for respawn");
		return nullptr;
	}

}
#endif
