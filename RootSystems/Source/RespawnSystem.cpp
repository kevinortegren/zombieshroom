#include "RespawnSystem.h"
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>


namespace RootSystems
{

	void RespawnSystem::Init()
	{
		m_health.Init(m_world->GetEntityManager());
		m_collision.Init(m_world->GetEntityManager());
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
				m_engineContext->m_physics->SetPosition(*(collision->m_handle), glm::vec3(0, 10, 0));

				health->Health = 100;
				health->IsDead = false;
			}
			health->WantsRespawn = false;
		}

		if(health->IsDead)
			health->RespawnDelay -= dt;
	}
}