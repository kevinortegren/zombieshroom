#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <Utility\ECS\Include\ComponentMapper.h>

#include <cstdint>
#include <set>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

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
		std::set<Entity*> m_entitiesToRemove;
	};

	// System to process a set of entities at a set time interval.
	class IntervalEntitySystem : public EntitySystem
	{
	public:
		friend class EntitySystemManager;

		IntervalEntitySystem(World* p_world, float p_interval, int p_ticksPerFrame)
			: EntitySystem(p_world), m_interval(p_interval), m_ticksPerFrame(p_ticksPerFrame), m_time(0.0f), m_ticks(0) {}

		bool CheckProcessing();
		void Process();

	protected:
		float GetSystemInterval();

	private:
		unsigned int m_ticks;
		unsigned int m_ticksPerFrame;
		float m_interval;
		float m_time;
	};

	// System to be executed on different thread.
	class ConcurrentSystem : public EntitySystem
	{
	public:
		friend class EntitySystemManager;

		ConcurrentSystem(World* p_world)
			: EntitySystem(p_world) { m_run = false; m_terminate = false; }

		void Process();	
		void Run();
		bool IsRunning();
		
		void Start();
		void Terminate();

		void Synch();

	private:
		std::atomic_bool m_run;
		std::atomic_bool m_terminate;
		std::thread m_thread;
	};
}