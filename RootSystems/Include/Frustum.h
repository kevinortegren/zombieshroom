#pragma once

#include <RootSystems/Include/Shapes/Plane.h>
#include <RootSystems/Include/Shapes/AABB.h>
#include <RootEngine/Render/Include/Renderer.h>
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
		void RecalculatePlanesEx(glm::mat4x4& p_view, glm::mat4x4& p_proj);
		glm::vec3 Intersect3Planes(PlaneEx& a, PlaneEx& b, PlaneEx& c);

		void DrawLines(glm::mat4 p_space, Render::RendererInterface* p_renderer);

		bool CheckBox(const AABB& p_box);
		bool CheckBoxEx(const AABB& p_box);

		Plane m_planes[6];
		PlaneEx m_planesEx[6];

		float m_near;
		float m_far;
		float m_fov;
		float m_aspectRatio;

		glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

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

		
	};
}