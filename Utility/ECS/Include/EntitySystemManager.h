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
		~EntitySystemManager();

		template<class T>
		void AddSystem(T* p_system, const char* p_systemName)
		{
			assert(m_systems.find(p_systemName) == m_systems.end());
			
			m_systems[p_systemName] = p_system;
		}

		template<class T>
		T* GetSystem(const char* p_systemName)
		{
			return static_cast<T*>(m_systems[p_systemName]);
		}

	private:
		void AddEntityToSystems(Entity* p_entity);
		void RemoveEntityFromSystems(Entity* p_entity);

		World* m_world;
		std::map<std::string, EntitySystem*> m_systems;
	};
}