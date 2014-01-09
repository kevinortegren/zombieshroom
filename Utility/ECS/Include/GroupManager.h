#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <map>

namespace ECS
{
	class GroupManager
	{
	public:
		friend class EntityImporter;
		friend class EntityExporter;
		typedef std::pair<std::multimap<std::string, ECS::Entity*>::iterator, std::multimap<std::string, ECS::Entity*>::iterator> GroupRange;

		void RegisterEntity(const std::string& p_group, ECS::Entity* p_entity);
		void UnregisterEntity(const std::string& p_group, ECS::Entity* p_entity);
		void UnregisterAll();

		void PrintEntitiesInGroup(const std::string& p_group);
		GroupRange GetEntitiesInGroup(const std::string& p_group);

	private:
		std::multimap<std::string, Entity*> m_groups;
	};
}