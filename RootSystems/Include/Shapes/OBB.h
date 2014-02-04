#pragma once
#include <glm/glm.hpp>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootSystems/Include/Shapes/AABB.h>

namespace RootForce
{
	struct OBB
	{
		OBB();
		OBB(float p_minX, float p_maxX, float p_minY, float p_maxY, float p_minZ, float p_maxZ, glm::mat4 p_transform);

		void DebugDraw(Render::RendererInterface* p_renderer, glm::vec3& p_color, glm::mat4 p_space) const;

		float m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
		glm::mat4 m_transform;
	};

	static void SATtest(const std::vector<glm::vec4>& p_points, glm::vec3& p_min, glm::vec3& p_max)
	{
		p_min = glm::vec3(99999.0f);
		p_max = glm::vec3(-99999.0f);

		for(int i = 0; i < p_points.size(); ++i)
		{
			if(p_points[i].x < p_min.x) p_min.x = p_points[i].x;
			if(p_points[i].x > p_max.x) p_max.x = p_points[i].x;

			if(p_points[i].y < p_min.y) p_min.y = p_points[i].y;
			if(p_points[i].y > p_max.y) p_max.y = p_points[i].y;

			if(p_points[i].z < p_min.z) p_min.z = p_points[i].z;
			if(p_points[i].z > p_max.z) p_max.z = p_points[i].z;
		}
	};

	static bool Overlaps(float p_min1, float p_max1, float p_min2, float p_max2)
	{
		return (p_min1 <= p_min2 && p_min2 <= p_max1) || (p_min2 <= p_min1 && p_min1 <= p_max2);
	}

	static bool Intersect(std::vector<glm::vec4>& p_obbPoints, const AABB& p_aabb)
	{
		glm::vec3 min;
		glm::vec3 max;
		SATtest(p_obbPoints, min, max);

		if(!Overlaps(min.x, max.x, p_aabb.m_minX, p_aabb.m_maxX))
			return false;
		if(!Overlaps(min.y, max.y, p_aabb.m_minY, p_aabb.m_maxY))
			return false;
		if(!Overlaps(min.z, max.z, p_aabb.m_minZ, p_aabb.m_maxZ))
			return false;

		return true;
	};

}