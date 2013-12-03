#pragma once

#include <Utility\ECS\Include\Entity.h>
#include <map>
#include <string>

namespace ECS
{
	class TagManager
	{
	public:
		void RegisterEntity(const std::string& p_tag, Entity* p_entity);
		void Unregister(const std::string& p_tag);

		Entity* GetEntityByTag(const std::string& p_tag);

	private:
		std::map<std::string, Entity*> m_tags; 
	};
}