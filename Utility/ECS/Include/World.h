#pragma once

#include <Utility\ECS\Include\EntityManager.h>
#include <Utility\ECS\Include\EntitySystemManager.h>
#include <Utility\ECS\Include\TagManager.h>
#include <Utility\ECS\Include\GroupManager.h>

namespace ECS
{
	/* Entity Component System context. */
	class World
	{
	public:
		World() : 
			m_systemManager(this),
			m_entityManager(&m_systemManager)
		{}

		EntityManager* GetEntityManager();
		EntitySystemManager* GetSystemManager();
		TagManager* GetTagManager();
		GroupManager* GetGroupManager();
		void SetDelta(float p_dt);
		float GetDelta();
	private:
		EntitySystemManager m_systemManager;
		EntityManager m_entityManager;
		TagManager m_tagManager;
		GroupManager m_groupManager;
		float m_dt;
	};
}