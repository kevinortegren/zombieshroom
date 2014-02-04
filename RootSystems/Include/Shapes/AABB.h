#pragma once
#include <glm/glm.hpp>
#include <RootSystems/Include/Shapes/Rectangle.h>
#include <RootEngine/Render/Include/Renderer.h>

namespace RootForce
{
	struct AABB
	{
		AABB();
		AABB(float p_minX, float p_maxX, float p_minY, float p_maxY, float p_minZ, float p_maxZ);

		int GetLengthX( void ) const;
		int GetLengthY( void ) const;
		int GetLengthZ( void ) const;

		Rectangle GetXZRect( void ) const;
		glm::vec3 GetCenter( void ) const;

		void Expand(glm::vec3 p_expansion);
		bool Collide(const AABB& p_aabb);


		void DebugDraw(Render::RendererInterface* p_renderer, glm::vec3& p_color, glm::mat4 p_space) const;

		float m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
	};
}