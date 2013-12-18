#pragma once

namespace RootForce
{
	namespace ActionType
	{
		enum ActionType
		{
			ACTION_CREATE,
			ACTION_COLLIDE,
			ACTION_DESTROY,
		};
	}

	struct CollisionData
	{
		int m_entityId;
	};

	struct Action
	{
		Action(ActionType::ActionType p_action)
			: m_action(p_action) {}

		ActionType::ActionType m_action;

		union 
		{
			CollisionData m_collision;
		} m_data;
	};
}