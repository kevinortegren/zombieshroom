#pragma once

#include <ECS/EntityManager.h>

namespace ECS
{
	class World
	{
	public:
		World();

		EntityManager* GetEntityManager() { return &m_entityManager; }

	private:
		EntityManager m_entityManager;
	};
}