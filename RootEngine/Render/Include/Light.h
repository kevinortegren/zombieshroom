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

	struct PointLight
	{
		glm::vec3 m_position;
		float padd;
		glm::vec3 m_attenuation;
		float m_range;
		glm::vec4 m_color; 
	};
}

