#pragma once

#include "glm/glm.hpp"

namespace RootForce
{
	struct Plane
	{
		Plane();
		~Plane();

		enum
		{
			ABOVE,
			BELOW,
			INTERSECTION
		};

		void setNormalAndPoint(glm::vec3 p_normal, glm::vec3 p_point)
		{
			m_point = p_point;
			m_normal = p_normal;
		}

		float distance(glm::vec3 p_position)
		{
			float D = glm::dot(-m_normal, m_point);
			return glm::dot(m_normal, p_position) + D;
		}

	private:
		glm::vec3 m_point;
		glm::vec3 m_normal;
	};
}