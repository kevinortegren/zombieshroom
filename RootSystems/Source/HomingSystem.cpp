#include "HomingSystem.h"

namespace RootForce
{
	void HomingSystem::Init()
	{
		m_physics.Init(m_world->GetEntityManager());
		m_target.Init(m_world->GetEntityManager());
	}

	void HomingSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Collision* physics = m_physics.Get(p_entity);
		HomingComponent* target = m_target.Get(p_entity);
		
		if (target->TargetPlayer != nullptr)
		{
			Transform* targetTransform = m_world->GetEntityManager()->GetComponent<Transform>(target->TargetPlayer);

			target->TargetPosition = targetTransform->m_position;
			
		}
		Transform* abilityTransform = m_world->GetEntityManager()->GetComponent<Transform>(p_entity);
		glm::vec3 currentVelocity = g_engineContext.m_physics->GetVelocity(*physics->m_handle);
		float speed = glm::length(currentVelocity);
		glm::vec3 targetVelocity = glm::normalize(target->TargetPosition - abilityTransform->m_position) * speed;

		glm::vec3 changeVel = targetVelocity - currentVelocity * target->Controllability;

		g_engineContext.m_physics->SetVelocity(*physics->m_handle, glm::normalize(currentVelocity * changeVel) * speed);
	}

}