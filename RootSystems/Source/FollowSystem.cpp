#include "FollowSystem.h"

namespace RootForce
{
	void FollowSystem::Init()
	{
		m_transform.Init(m_world->GetEntityManager());
		m_target.Init(m_world->GetEntityManager());
	}

	void FollowSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		Transform* transform = m_transform.Get(p_entity);
		FollowComponent* target = m_target.Get(p_entity);
		
		if (target->Target)
		{
			Transform* targetTransform = m_world->GetEntityManager()->GetComponent<Transform>(target->Target);

			transform->m_orientation = targetTransform->m_orientation;
			transform->m_position = targetTransform->m_position + target->Offset * targetTransform->m_orientation.GetFront();
		}
	}

}