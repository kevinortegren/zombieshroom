#pragma once

#include <ECS/Entity.h>
#include <ECS/ComponentMapper.h>

#include <bitset>
#include <set>
#include <memory>

#define ECS_COMPSYSTEM_BITS 32

namespace ECS
{
	class World;

	class VoidSystem
	{
	public:
		VoidSystem(World* p_world)
			: m_world(p_world) { }

		virtual void Process() = 0;

	protected:
		World* m_world;
	};

	class ComponentSystem : public VoidSystem
	{
	public:
		friend class ComponentSystemManager;

		ComponentSystem(World* p_world)
			: VoidSystem(p_world) { }

		template<class T>
		void SetUsage()
		{
			m_componentTypes[Component<T>::GetTypeId()] = 1;
		}

		void Process();

		virtual void Init() = 0;
		virtual void Begin() = 0;
		virtual void ProcessEntity(Entity* p_entity) = 0;
		virtual void End() = 0;

	protected:
		std::bitset<ECS_COMPSYSTEM_BITS> m_componentTypes;
		std::set<Entity*> m_activeEntities;
	};

	class IntervalComponentSystem : ComponentSystem
	{
	public:
		friend class ComponentSystemManager;

		IntervalComponentSystem(World* p_world, float p_interval)
			: ComponentSystem(p_world), m_interval(p_interval), m_time(0.0f) {}

		bool CheckProcessing();
		void Process();

	private:
		float m_interval;
		float m_time;
	};
}