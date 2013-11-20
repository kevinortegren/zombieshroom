#pragma once

#include <bitset>

namespace ECS
{
	class Entity
	{
	public:
		friend class EntityManager;
		friend class ComponentSystemManager; 

		Entity(int p_id)
			: m_id(p_id) {}

		int GetId() const { return m_id; }

	private:
		int m_id;
		std::bitset<2> m_componentTypes;

	};
}