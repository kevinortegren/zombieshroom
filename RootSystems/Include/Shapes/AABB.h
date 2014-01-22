#pragma once
#include <glm/glm.hpp>
#include <RootSystems/Include/Shapes/Rectangle.h>
#include <RootEngine/Render/Include/Renderer.h>

namespace RootForce
{
	struct AABB
	{
		AABB();
		AABB(int p_minX, int p_maxX, int p_minY, int p_maxY, int p_minZ, int p_maxZ);

		int GetLengthX( void ) const;
		int GetLengthY( void ) const;
		int GetLengthZ( void ) const;

		Rectangle GetXZRect( void ) const;
		glm::vec3 GetCenter( void ) const;

		void DebugDraw(Render::RendererInterface* p_renderer, glm::vec3& p_color) const;

		int m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
	};
}