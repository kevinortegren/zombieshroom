#pragma once

#include <ECS/Component.h>

#include <glm/glm.hpp>

namespace RootForce
{
	struct Transform : public ECS::Component<Transform>
	{
		glm::vec3 m_position;
		//Orientation m_orientation;
	};
}