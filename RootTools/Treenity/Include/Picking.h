#pragma once

#include <RootSystems/Include/Shapes/OBB.h>
#include <glm/glm.hpp>
#include <glm/gtc/swizzle.hpp>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

static void Debug(RootForce::OBB* obb, const glm::mat4x4& p_space, const glm::vec3& p_color)
{
	glm::vec4 positions[8];
	positions[0] = p_space * glm::vec4(obb->m_minX, obb->m_minY, obb->m_minZ, 1.0f);
	positions[1] = p_space * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_minZ, 1.0f);
	positions[2] = p_space * glm::vec4(obb->m_minX, obb->m_minY, obb->m_maxZ, 1.0f);
	positions[3] = p_space * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_maxZ, 1.0f);
	positions[4] = p_space * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_minZ, 1.0f);
	positions[5] = p_space * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_minZ, 1.0f);
	positions[6] = p_space * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_maxZ, 1.0f);
	positions[7] = p_space * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_maxZ, 1.0f);

	unsigned int indices[] = 
	{ 
		0, 2, 2, 3, 3, 1, 1, 0,
		4, 6, 6, 7, 7, 5, 5, 4,
		0, 4, 2, 6, 3, 7, 1, 5 
	};

	for(int i = 0; i < 24; i += 2)
	{
		glm::vec3 pos1, pos2;
		pos1 = glm::swizzle<glm::X, glm::Y, glm::Z>(positions[indices[i]]);
		pos2 = glm::swizzle<glm::X, glm::Y, glm::Z>(positions[indices[i+1]]);
		g_engineContext.m_renderer->AddLine(pos1, pos2, glm::vec4(p_color.x, p_color.y, p_color.z, 1.0f));
	}
}

static bool RayVsOBB(const glm::vec3& cameraPos, const glm::vec3& ray, RootForce::OBB* obb, const glm::mat4x4& transform, float& t)
{
	float tMin = -99999.9f;
	float tMax = 99999.9f;
 
	// OBB Positions.
	glm::vec4 positions[8];
	positions[0] = transform * glm::vec4(obb->m_minX, obb->m_minY, obb->m_minZ, 1.0f);
	positions[1] = transform * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_minZ, 1.0f);
	positions[2] = transform * glm::vec4(obb->m_minX, obb->m_minY, obb->m_maxZ, 1.0f);
	positions[3] = transform * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_maxZ, 1.0f);
	positions[4] = transform * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_minZ, 1.0f);
	positions[5] = transform * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_minZ, 1.0f);
	positions[6] = transform * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_maxZ, 1.0f);
	positions[7] = transform * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_maxZ, 1.0f);
 
	glm::vec4 normalX = positions[1] - positions[0];
	glm::vec4 normalY = positions[4] - positions[0];
	glm::vec4 normalZ = positions[2] - positions[0];
 
	glm::vec4 center = positions[0] + (normalX / 2) + (normalY / 2) + (normalZ / 2); 
 
	glm::vec3 normals[3];
	normals[0] = glm::normalize(glm::vec3(normalX.x, normalX.y, normalX.z));
	normals[1] = glm::normalize(glm::vec3(normalY.x, normalY.y, normalY.z));
	normals[2] = glm::normalize(glm::vec3(normalZ.x, normalZ.y, normalZ.z));
 
	float distances[3];
	distances[0] = glm::length(normalX) / 2;
	distances[1] = glm::length(normalY) / 2;
	distances[2] = glm::length(normalZ) / 2;
 
	glm::vec3 direction = glm::vec3(center.x, center.y, center.z) - cameraPos;
 
	for(int i=0; i<3; i++)
	{
		float e = glm::dot(normals[i], direction);
		float f = glm::dot(normals[i], ray);
 
		if(abs(f) > 0.000001f)
		{
			float t1 = (e + distances[i]) / f;
			float t2 = (e - distances[i]) / f;
 
			if(t1>t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}
 
			if(t1 > tMin) tMin = t1;
			if(t2 < tMax) tMax = t2;
 
			if(tMin > tMax) return false;
			if(tMax < 0) return false;
		}
 
		else if(-e-distances[i] > 0 || -e+distances[i] < 0) return false;     
	}
 
	if(tMin > 0) 
 
	{
		t = tMin;
		return true;
	}
	else
	{
		t = tMax;
		return true;
	}
 
	return false;
}

static bool RayVsPlane(const glm::vec3& center, const glm::vec3& normal, const glm::vec3& cameraPos, const glm::vec3& ray, float& t)
{
	float denom = glm::dot(normal, ray);
	glm::vec3 centerToCamera = center - cameraPos;
	t = glm::dot(centerToCamera, normal) / denom;
	return true;
}