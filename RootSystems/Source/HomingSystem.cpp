#include "HomingSystem.h"

namespace RootForce
{
	void HomingSystem::Init()
	{
		//m_collision.Init(m_world->GetEntityManager());
		m_target.Init(m_world->GetEntityManager());
		m_physics.Init(m_world->GetEntityManager());
	}

	void HomingSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		//Collision* collision = m_collision.Get(p_entity);
		HomingComponent* target = m_target.Get(p_entity);
		Physics* physics = m_physics.Get(p_entity);
		
		if (target->TargetPlayer != nullptr)
		{
			Transform* targetTransform = m_world->GetEntityManager()->GetComponent<Transform>(target->TargetPlayer);

			target->TargetPosition = targetTransform->m_position;
			
		}
		Transform* abilityTransform = m_world->GetEntityManager()->GetComponent<Transform>(p_entity);
		glm::vec3 currentVelocity = physics->m_velocity;//g_engineContext.m_physics->GetVelocity(*collision->m_handle);
		//target->Speed = glm::length(currentVelocity);
		glm::vec3 targetVelocity = glm::normalize(target->TargetPosition - abilityTransform->m_position) * target->Speed;

		glm::vec3 changeVel = (targetVelocity - currentVelocity) * target->Controllability;

		physics->m_velocity = glm::normalize(currentVelocity + changeVel) * target->Speed;
	}

}