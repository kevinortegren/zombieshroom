#pragma once

#include <bitset>
#include <cstdint>

namespace ECS
{
	class Entity
	{
	public:
		friend class EntityManager;
		friend class EntitySystem;
		friend class EntitySystemManager; 
		friend class EntityExporter; 

		Entity() : m_id(-1), m_flag(0) {}

		int GetId() const { return m_id; }
		int64_t GetFlag() const { return m_flag; }

	private:
		int m_id;
		int64_t m_flag;
	};
}