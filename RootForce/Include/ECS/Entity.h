#pragma once

namespace ECS
{
	class Entity
	{
	public:
		Entity(int p_id)
			: m_id(p_id) {}

		int GetId() const { return m_id; }

	private:
		int m_id;
	};
}