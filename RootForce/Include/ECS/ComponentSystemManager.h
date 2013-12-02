#pragma once

#include <ECS/ComponentSystem.h>
#include <map>
#include <string>
#include <memory>
#include <assert.h>

namespace ECS
{
	class World;

	class ComponentSystemManager
	{
	public:
		friend class EntityManager;

		ComponentSystemManager(World* p_world)
			: m_world(p_world) {}
		~ComponentSystemManager();

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
		std::map<std::string, ComponentSystem*> m_systems;
	};
}