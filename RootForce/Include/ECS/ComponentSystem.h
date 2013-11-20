#pragma once

#include <ECS/Entity.h>
#include <ECS/ComponentMapper.h>

#include <bitset>
#include <set>
#include <memory>

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
		void SetUsage(bool p_use)
		{
			m_componentTypes[Component<T>::GetTypeId()] = p_use;
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
		std::bitset<2> m_componentTypes;
		std::set<std::shared_ptr<Entity>> m_activeEntities;
		
	};
}