#pragma once
#include <glm/glm.hpp>

namespace RootForce
{
	struct AABB
	{
		AABB();
		AABB(int p_minX, int p_maxX, int p_minY, int p_maxY, int p_minZ, int p_maxZ);

		int GetLengthX( void ) const;
		int GetLengthY( void ) const;
		int GetLengthZ( void ) const;

		glm::ivec4 GetRect( void ) const;

		int m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
	};
}