#pragma once

#include "RootEngine/Include/ECS/Entity.h"
#include "RootEngine/Include/ECS/ComponentMapper.h"

#include <bitset>
#include <set>
#include <memory>

#define ECS_COMPSYSTEM_BITS 32

namespace ECS
{
	class World;

	class ComponentSystem
	{
	public:
		friend class ComponentSystemManager;

		ComponentSystem(World* p_world)
			: m_world(p_world) {}

		template<class T>
		void SetUsage()
		{
			m_componentTypes[Component<T>::GetTypeId()] = 1;
		}

		void Process();

		// Impl. Interface.
		virtual void Init() = 0;
		virtual void Begin() = 0;
		virtual void ProcessEntity(std::shared_ptr<Entity> p_entity) = 0;
		virtual void End() = 0;

	protected:
		World* m_world;

	private:
		std::bitset<ECS_COMPSYSTEM_BITS> m_componentTypes;
		std::set<std::shared_ptr<Entity>> m_activeEntities;
		
	};
}