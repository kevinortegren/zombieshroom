#pragma once

#include <RootSystems/Include/PhysicsSystem.h>
#include <iostream>

struct ConcurrentTest : public ECS::ConcurrentSystem
{
	ConcurrentTest(ECS::World* p_world)
		: ECS::ConcurrentSystem(p_world)
	{
		SetUsage<RootForce::Physics>();
	}

	void Init()
	{
		m_physics.Init(m_world->GetEntityManager());
	}

	void Begin()
	{

	}

	void ProcessEntity(ECS::Entity* p_entity)
	{
		auto p = m_physics.Get(p_entity);

		std::cout << p_entity->GetId() << " " << p->m_mass << std::endl;
	}

	void End()
	{

	}

	ECS::ComponentMapper<RootForce::Physics> m_physics;
};