#pragma once

#include <glm/glm.hpp>

namespace Render
{
	struct DirectionalLight
	{
		glm::vec3 m_direction;
		float padd;
		glm::vec4 m_color;
	};
}