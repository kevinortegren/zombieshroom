#pragma once

#include <ECS/EntityManager.h>
#include <ECS/ComponentSystemManager.h>

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