#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <map>

namespace ECS
{
	class World;

	class GroupManager
	{
	public:
		friend class EntityImporter;
		friend class EntityExporter;
		typedef std::pair<std::multimap<std::string, ECS::Entity*>::iterator, std::multimap<std::string, ECS::Entity*>::iterator> GroupRange;

		GroupManager(World* p_world);

		void RegisterEntity(const std::string& p_group, ECS::Entity* p_entity);
		void UnregisterEntity(const std::string& p_group, ECS::Entity* p_entity);
		void UnregisterAll();
		bool IsEntityInGroup(ECS::Entity* p_entity, const std::string& p_group);

		void PrintEntitiesInGroup(const std::string& p_group);
		GroupRange GetEntitiesInGroup(const std::string& p_group);

	private:
		World* m_world;
		std::multimap<std::string, Entity*> m_groups;
	};
}