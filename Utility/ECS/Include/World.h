#pragma once

#include <Utility\ECS\Include\EntityManager.h>
#include <Utility\ECS\Include\EntitySystemManager.h>
#include <Utility\ECS\Include\TagManager.h>
#include <Utility\ECS\Include\GroupManager.h>
#include <Utility\ECS\Include\EntityImporter.h>
#include <Utility\ECS\Include\EntityExporter.h>
#include <Utility\ECS\Include\Storage.h>
#include <vector>

namespace ECS
{
	namespace MessageType
	{
		enum MessageType
		{
			ENTITY_ADDED,
			ENTITY_REMOVED,
			COMPONENT_ADDED,
			COMPONENT_REMOVED,
			NONE
		};
	}

	struct Message
	{
		Message() : 
			m_type(MessageType::NONE),
			m_entity(nullptr),
			m_compType(-1) 
		{}

		MessageType::MessageType m_type;
		Entity* m_entity;
		int m_compType;
	};

	/* Entity Component System context. */
	class World
	{
	public:

		friend class EntityManager;

		World() : 
			m_systemManager(this),
			m_entityManager(this),
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
		Storage* GetStorage();

		const std::vector<Message>& GetMessages() const;

		void SetDelta(float p_dt);
		float GetDelta();

		void TestMessaging();

	private:
		EntitySystemManager m_systemManager;
		EntityManager m_entityManager;
		TagManager m_tagManager;
		GroupManager m_groupManager;
		EntityImporter m_importer;
		EntityExporter m_exporter;
		Storage m_storage;
		float m_dt;

		std::vector<Message> m_messages;
	};
}