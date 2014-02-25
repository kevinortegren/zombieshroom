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
		//Render stuff i guess
	}

}