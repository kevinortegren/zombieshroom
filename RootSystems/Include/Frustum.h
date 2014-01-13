#pragma once

#include "RootSystems/Include/Math/Plane.h"

#include "glm/glm.hpp"

namespace RootForce
{
	struct Frustum
	{
		Frustum(float p_fov, float p_near, float p_far, float p_aspectRatio = 1)
		{
			m_fov = p_fov;
			m_near = p_near;
			m_far = p_far;
			m_aspectRatio = p_aspectRatio;
		}
		~Frustum(){}

		void recalculatePlanes();

		Plane m_planes[8];

	private:
		float m_near;
		float m_far;
		float m_fov;
		float m_aspectRatio;

		enum 
		{
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEARP,
			FARP
		};
	};
}