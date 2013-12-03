#pragma once

#include <Utility\ECS\Include\Component.h>
#include <Orientation.h>

#include <glm/glm.hpp>

namespace RootForce
{
	struct Transform : public ECS::Component<Transform>
	{
		Transform()
		{
			m_position = glm::vec3(0.0f);
			m_scale = glm::vec3(1.0f);
		}
		glm::vec3 m_position;
		Orientation m_orientation;
		glm::vec3 m_scale;
	};
}