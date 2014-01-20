#pragma once

#include "RootSystems/Include/Math/Plane.h"

#include "glm/glm.hpp"

namespace RootForce
{
	class Frustum
	{
	public:
		Frustum(float p_fov, float p_near, float p_far, float p_aspectRatio = 1)
		{
			m_fov = p_fov;
			m_near = p_near;
			m_far = p_far;
			m_aspectRatio = p_aspectRatio;
			RecalculatePlanes();
		}
		Frustum(){
			m_fov = 50.0f;
			m_near = 0.1f;
			m_far = 10.0f;
			m_aspectRatio = 1.0f;
			//RecalculatePlanes();
		}
		~Frustum(){}

		void RecalculatePlanes();
		void DrawLines(glm::mat4 p_space);

		Plane m_planes[6];

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

		glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	};
}