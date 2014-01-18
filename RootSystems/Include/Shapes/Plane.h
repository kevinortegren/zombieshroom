#pragma once

#include <glm/glm.hpp>
#include <RootSystems/Include/Shapes/Line.h>

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

	void NormalizePlane(PlaneEx& p_plane);
	float PlaneIntersectLine(PlaneEx& p_plane, Line& p_line);
}