#pragma once

#include <Utility\ECS\Include\EntityManager.h>
#include <Utility\ECS\Include\EntitySystemManager.h>
#include <Utility\ECS\Include\TagManager.h>
#include <Utility\ECS\Include\GroupManager.h>
#include <Utility\ECS\Include\EntityImporter.h>
#include <Utility\ECS\Include\EntityExporter.h>

namespace ECS
{
	/* Entity Component System context. */
	class World
	{
	public:

		World() : 
			m_systemManager(this),
			m_entityManager(&m_systemManager),
			m_exporter(this),
			m_importer(this),
			m_dt(0)
		{}

		EntityManager* GetEntityManager();
		EntitySystemManager* GetSystemManager();
		TagManager* GetTagManager();
		GroupManager* GetGroupManager();
		EntityImporter* GetEntityImporter();
		EntityExporter* GetEntityExporter();

		void SetDelta(float p_dt);
		float GetDelta();

	private:
		EntitySystemManager m_systemManager;
		EntityManager m_entityManager;
		TagManager m_tagManager;
		GroupManager m_groupManager;
		EntityImporter m_importer;
		EntityExporter m_exporter;
		float m_dt;
	};
}