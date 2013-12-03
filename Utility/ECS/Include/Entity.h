#pragma once

#include <bitset>

#define ECS_ENTITY_BITS 32

namespace ECS
{
	class Entity
	{
	public:
		friend class EntityManager;
		friend class EntitySystemManager; 

		Entity(unsigned int p_id)
			: m_id(p_id) {}

		unsigned int GetId() const { return m_id; }

	private:
		unsigned int m_id;
		std::bitset<ECS_ENTITY_BITS> m_componentTypes;
	};
}