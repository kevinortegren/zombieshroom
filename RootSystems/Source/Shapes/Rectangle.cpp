#include <RootSystems/Include/Shapes/Rectangle.h>

namespace RootForce
{
	Rectangle::Rectangle()
		: m_x(0), m_y(0), m_width(0), m_height(0)
	{

	}

	Rectangle::Rectangle(float p_x, float p_y, float p_width, float p_height)
		: m_x(p_x), m_y(p_y), m_width(p_width), m_height(p_height)
	{

	}

	bool Rectangle::IsPointContained(glm::vec2 p_point)
	{
		return ((p_point.x >= m_x && p_point.x <= m_x + m_width) &&
		( p_point.y >= m_y && p_point.y <= m_y + m_height ));
	}
}


