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

		unsigned int GetId() const { return m_id; }
		int64_t GetFlag() const { return m_flag; }

	private:
		unsigned int m_id;
		int64_t m_flag;
	};
}