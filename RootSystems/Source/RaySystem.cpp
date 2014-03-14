#ifndef COMPILE_LEVEL_EDITOR

#include "RaySystem.h"

namespace RootForce
{
	void RaySystem::Init()
	{
		m_collision.Init(m_world->GetEntityManager());
		m_ray.Init(m_world->GetEntityManager());
	}

	void RaySystem::ProcessEntity(ECS::Entity* p_entity)
	{
		//RootForce::RayComponent* ray = m_ray.Get(p_entity);

	}

}

#endif