#pragma once

#include <RootForce\Include\ECS/Entity.h>
#include <RootForce\Include\ECS/ComponentMapper.h>

#include <bitset>
#include <set>
#include <memory>

#define ECS_COMPSYSTEM_BITS 32

namespace ECS
{
	class World;

	class  ComponentSystem
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

		void Process(float dt);

		// Impl. Interface.
		virtual void Init() = 0;
		virtual void Begin() = 0;
		virtual void ProcessEntity(Entity* p_entity, float dt) = 0;
		virtual void End() = 0;

	protected:
		World* m_world;

	private:
		std::bitset<ECS_COMPSYSTEM_BITS> m_componentTypes;
		std::set<Entity*> m_activeEntities;
		
	};
}