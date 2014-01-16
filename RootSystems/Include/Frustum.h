#pragma once

#include <RootSystems/Include/Shapes/Plane.h>
#include <RootSystems/Include/Shapes/AABB.h>
#include <glm/glm.hpp>

namespace RootForce
{
	class Frustum
	{
	public:
		Frustum(){};
		Frustum(float p_fov, float p_near, float p_far, float p_aspectRatio = 1)
		{
			m_fov = p_fov;
			m_near = p_near;
			m_far = p_far;
			m_aspectRatio = p_aspectRatio;
			RecalculatePlanes();
		}

		void RecalculatePlanes();
		void DrawLines(glm::mat4 p_space);

		bool CheckBox(const AABB& p_box);

		Plane m_planes[6];

		float m_near;
		float m_far;
		float m_fov;
		float m_aspectRatio;

	private:

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