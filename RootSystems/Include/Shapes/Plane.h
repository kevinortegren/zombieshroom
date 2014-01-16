#pragma once

#include <glm/glm.hpp>

namespace RootForce
{
	struct Plane
	{
		enum
		{
			ABOVE,
			BELOW,
			INTERSECTION
		};

		void SetNormalAndPoint(glm::vec3 p_normal, glm::vec3 p_point);
		float Distance(glm::vec3 p_position);

		glm::vec3 m_point;
		glm::vec3 m_normal;
	};
}