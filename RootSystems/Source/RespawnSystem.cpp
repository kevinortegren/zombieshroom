#include "RespawnSystem.h"
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>

namespace RootSystems
{
	void RespawnSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		float dt = m_world->GetDelta();

		RootForce::HealthComponent* health = m_world->GetEntityManager()->GetComponent<RootForce::HealthComponent>(p_entity);
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);

		if( !health || !transform )
			return;

		if(health->Health <= 0 && !health->IsDead)
		{
			health->IsDead = true;
			health->RespawnDelay = 10.f;
		}
		if(health->IsDead && health->WantsRespawn)
		{
			if(health->RespawnDelay <= 0)
			{
				// TODO: Find a spawn point
				transform->m_position = glm::vec3(0, 10, 0);

				health->Health = 100;
				health->IsDead = false;
			}
			health->WantsRespawn = false;
		}
		if(health->IsDead)
			health->RespawnDelay -= dt;
	}
}