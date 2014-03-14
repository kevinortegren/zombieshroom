#pragma once

#include <Utility/ECS/Include/Component.h>
#include <set>
#include <map>
#include <vector>
#include <glm/glm.hpp>

namespace RootForce
{
	struct ControllerActions : public ECS::Component<ControllerActions>
	{
		//Scancodes associated with lists of actions
		std::map<int, std::string> m_actions;
		std::set<std::string> m_activeActions;
	};
}