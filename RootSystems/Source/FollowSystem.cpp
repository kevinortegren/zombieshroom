#ifndef COMPILE_LEVEL_EDITOR
#include "FollowSystem.h"

extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

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
		ECS::Entity* targ = FindEntity(g_networkEntityMap, target->TargetID);
		
		if (targ != nullptr)
		{
			Transform* targetTransform = m_world->GetEntityManager()->GetComponent<Transform>(targ);

			transform->m_orientation = targetTransform->m_orientation;
			transform->m_position = targetTransform->m_position + target->Offset * targetTransform->m_orientation.GetFront();
		}
	}

}
#endif
