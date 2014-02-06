#pragma once
#include <glm/glm.hpp>

namespace RootForce
{
	struct Rectangle
	{
		Rectangle();
		Rectangle(float p_x, float p_y, float p_width, float p_height);
	
		bool IsPointContained(glm::vec2 p_point);

		float m_x, m_y, m_width, m_height;
	};
}