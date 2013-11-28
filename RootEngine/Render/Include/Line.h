#pragma once

#include <glm/glm.hpp>

namespace Render
{
	struct Line
	{
		Line(glm::vec3 p_fromPoint, glm::vec3 p_toPoint, glm::vec4 p_color)
		{
			m_fromPoint = p_fromPoint;
			m_toPoint = p_toPoint;
			m_color = p_color;
		}
		glm::vec3 m_fromPoint;
		glm::vec3 m_toPoint;
		glm::vec4 m_color;
	};
}