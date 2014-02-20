#pragma once

#include <Utility\ECS\Include\EntitySystem.h>
#include <map>
#include <string>
#include <memory>
#include <assert.h>

namespace ECS
{
	class World;

	class EntitySystemManager
	{
	public:
		friend class EntityManager;

		EntitySystemManager(World* p_world)
			: m_world(p_world) {}

		template<class T>
		void AddSystem(T* p_system)
		{
			p_system->Init();
			m_systems.push_back(p_system);
		}

		void Clear();

	private:

		void AddEntityToSystems(Entity* p_entity);
		void RemoveEntityFromSystems(Entity* p_entity);

		World* m_world;
		std::vector<EntitySystem*> m_systems;
	};
}