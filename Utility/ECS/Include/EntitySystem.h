#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <Utility\ECS\Include\ComponentMapper.h>

#include <cstdint>
#include <set>
#include <memory>

namespace ECS
{
	class World;

	// Simple system to perform custom logic on the world.
	class VoidSystem
	{
	public:
		VoidSystem(World* p_world)
			: m_world(p_world) { }

		virtual void Process() = 0;

	protected:
		World* m_world;
	};

	// System to process a set of entities.
	class EntitySystem : public VoidSystem
	{
	public:
		friend class EntitySystemManager;

		EntitySystem(World* p_world)
			: VoidSystem(p_world), m_flag(0) { }

		template<class T>
		void SetUsage()
		{
			m_flag |= (1ULL << Component<T>::GetTypeId());
		}

		void Process();

		virtual void Init() = 0;
		virtual void Begin(){}
		virtual void ProcessEntity(Entity* p_entity) = 0;
		virtual void End(){}

	protected:
		int64_t m_flag;
		std::set<Entity*> m_activeEntities;
	};

	// System to process a set of entities at a set time interval.
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