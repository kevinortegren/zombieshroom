#pragma once

//change to full path
#include <RootForce\Include\ECS\World.h>
#include <RootForce\Include\ECS\ComponentSystemManager.h>

namespace ECS
{
	/* Entity Component System context. */
	class World
	{
	public:
		World() : 
			m_systemManager(this),
			m_entityManager(&m_systemManager)
		{}

		EntityManager* GetEntityManager();
		ComponentSystemManager* GetSystemManager();

	private:
		ComponentSystemManager m_systemManager;
		EntityManager m_entityManager;
	};
}