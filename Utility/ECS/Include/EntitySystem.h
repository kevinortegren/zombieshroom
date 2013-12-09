#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <Utility\ECS\Include\ComponentMapper.h>

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

	class EntitySystem : public VoidSystem
	{
	public:
		friend class EntitySystemManager;

		EntitySystem(World* p_world)
			: VoidSystem(p_world) { }

		template<class T>
		void SetUsage()
		{
			m_componentTypes[Component<T>::GetTypeId()] = 1;
		}

		void Process();

		virtual void Init() = 0;
		virtual void Begin(){}
		virtual void ProcessEntity(Entity* p_entity) = 0;
		virtual void End(){}

	protected:
		std::bitset<ECS_COMPSYSTEM_BITS> m_componentTypes;
		std::set<Entity*> m_activeEntities;
	};

	class IntervalEntitySystem : EntitySystem
	{
	public:
		friend class EntitySystemManager;

		IntervalEntitySystem(World* p_world, float p_interval)
			: EntitySystem(p_world), m_interval(p_interval), m_time(0.0f) {}

		bool CheckProcessing();
		void Process();

	private:
		float m_interval;
		float m_time;
	};
}