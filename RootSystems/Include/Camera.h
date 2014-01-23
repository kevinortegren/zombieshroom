#pragma once

#include <Utility/ECS/Include/Component.h>
#include <RootSystems/Include/Frustum.h>
#include <glm/glm.hpp>

namespace RootForce
{
	struct Camera : public ECS::Component<Camera>
	{
		Frustum m_frustum;
	};
}