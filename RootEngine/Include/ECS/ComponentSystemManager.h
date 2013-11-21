#pragma once

#include "RootEngine/Include/ECS/ComponentSystem.h"
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

		template<class T>
		std::shared_ptr<T> CreateSystem(const std::string& p_systemName)
		{
			std::shared_ptr<T> system = std::shared_ptr<T>(new T(m_world));

			assert(m_systems.find(p_systemName) == m_systems.end());
			
			m_systems[p_systemName] = system;

			return system;
		}

		template<class T>
		T* GetSystem(const std::string& p_systemName)
		{
			return static_cast<T*>(m_systems[p_systemName].get());
		}

		void InitializeSystems();

	private:
		void AddEntityToSystems(std::shared_ptr<Entity> p_entity);
		void RemoveEntityFromSystems(std::shared_ptr<Entity> p_entity);

		World* m_world;
		std::map<std::string, std::shared_ptr<ComponentSystem>> m_systems;
	};
}