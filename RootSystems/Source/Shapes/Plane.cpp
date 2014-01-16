#include <RootSystems/Include/Shapes/Plane.h>

namespace RootForce
{
	void Plane::SetNormalAndPoint(glm::vec3 p_normal, glm::vec3 p_point)
	{
		m_point = p_point;
		m_normal = p_normal;
	}

	float Plane::Distance(glm::vec3 p_position)
	{
		float D = glm::dot(-m_normal, m_point);
		return glm::dot(m_normal, p_position) + D;
	}
}