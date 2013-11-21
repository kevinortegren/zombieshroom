#pragma once

#include "RootEngine/Include/ECS/EntityManager.h"
#include "RootEngine/Include/ECS/ComponentSystemManager.h"

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

		EntityManager* GetEntityManager() { return &m_entityManager; }
		ComponentSystemManager* GetSystemManager() { return &m_systemManager; }

	private:
		ComponentSystemManager m_systemManager;
		EntityManager m_entityManager;
	};
}