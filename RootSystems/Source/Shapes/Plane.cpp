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

	void NormalizePlane(PlaneEx& p_plane)
	{
		float mag;
		mag = sqrt(p_plane.a * p_plane.a + p_plane.b * p_plane.b + p_plane.c * p_plane.c);

		p_plane.a = p_plane.a / mag;
		p_plane.b = p_plane.b / mag;
		p_plane.c = p_plane.c / mag;
		p_plane.d = p_plane.d / mag;
	}

	float PlaneIntersectLine(PlaneEx& p_plane, Line& p_line)
	{
		glm::vec3 normal = glm::vec3(p_plane.a, p_plane.b, p_plane.c);

		float f = -glm::dot((p_line.m_origin + p_plane.d), normal) / glm::dot(p_line.m_direction, normal); 

		return f;
	}

	glm::vec3 PlaneIntersectLineA(PlaneEx& p_plane, glm::vec3& p_a, glm::vec3& p_b)
	{
		glm::vec3 ba = p_b-p_a;
		float nDotA = glm::dot(glm::vec3(p_plane.a, p_plane.b, p_plane.c), p_a);
		float nDotBA = glm::dot(glm::vec3(p_plane.a, p_plane.b, p_plane.c), ba);

		return p_a + (((p_plane.d - nDotA)/nDotBA) * ba);
	}

}