#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <map>
#include <string>

namespace ECS
{
	class World;

	class TagManager
	{
	public:
		friend class EntityImporter;
		friend class EntityExporter;

		TagManager(World* p_world);

		void RegisterEntity(const std::string& p_tag, Entity* p_entity);
		void Unregister(const std::string& p_tag);
		void UnregisterAll();

		Entity* GetEntityByTag(const std::string& p_tag);

	private:
		World* m_world;
		std::map<std::string, Entity*> m_tags; 
	};
}