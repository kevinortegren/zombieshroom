#pragma once
#include <glm/glm.hpp>

namespace RootForce
{
	struct Rectangle
	{
		Rectangle();
		Rectangle(int p_x, int p_y, int p_width, int p_height);
	
		bool IsPointContained(glm::vec2 p_point);

		int m_x, m_y, m_width, m_height;
	};
}