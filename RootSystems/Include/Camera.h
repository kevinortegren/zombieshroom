#pragma once

#include <Utility/ECS/Include/Component.h>
#include <glm/glm.hpp>

namespace RootForce
{
	struct Camera : public ECS::Component<Camera>
	{
		float m_fov;
		float m_near;
		float m_far;
	};
}