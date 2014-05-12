#pragma once
#include <glm/glm.hpp>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootSystems/Include/Shapes/AABB.h>

namespace RootEngine
{
	struct OBB
	{
		OBB();
		OBB(float p_minX, float p_maxX, float p_minY, float p_maxY, float p_minZ, float p_maxZ, glm::mat4 p_transform);

		void DebugDraw(Render::RendererInterface* p_renderer, glm::vec3& p_color, glm::mat4 p_space) const;
		void Expand(const glm::vec3& p_expansion);

		float m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
		glm::mat4 m_transform;
	};
}