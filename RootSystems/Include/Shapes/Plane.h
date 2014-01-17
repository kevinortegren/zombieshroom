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

	struct PlaneEx
	{
		float a;
		float b;
		float c;
		float d;
	};

	inline void NormalizePlane(PlaneEx& plane)
	{
		float mag;
		mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

		plane.a = plane.a / mag;
		plane.b = plane.b / mag;
		plane.c = plane.c / mag;
		plane.d = plane.d / mag;
	}

}