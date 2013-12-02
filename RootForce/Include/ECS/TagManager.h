#pragma once

#include <ECS/Entity.h>
#include <map>
#include <string>

namespace ECS
{
	class TagManager
	{
	public:
		void RegisterEntity(const char* p_tag, Entity* p_entity);

	private:
		std::map<std::string, Entity*> m_tags; 
	};
}