#pragma once

#include <Utility/ECS/Include/Component.h>
#include <RootSystems/Include/Orientation.h>
#include <glm/glm.hpp>

namespace RootForce
{
	struct Transform : public ECS::Component<Transform>
	{
		Transform()
		{
			m_position = glm::vec3(0);
			m_scale = glm::vec3(1);
		}

		glm::vec3 m_position;
		Orientation m_orientation;
		glm::vec3 m_scale;
	};
}