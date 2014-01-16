#include <RootSystems/Include/Shapes/AABB.h>
#include <cstdlib>

namespace RootForce
{
	AABB::AABB()
		: m_minX(0), m_maxX(0), m_minY(0), m_maxY(0), m_minZ(0), m_maxZ(0)
	{}

	AABB::AABB(int p_minX, int p_maxX, int p_minY, int p_maxY, int p_minZ, int p_maxZ)
		: m_minX(p_minX), m_maxX(p_maxX), m_minY(p_minY), m_maxY(p_maxY), m_minZ(p_minZ), m_maxZ(p_maxZ)
	{}

	int AABB::GetLengthX( void ) const
	{
		return abs( m_maxX -  m_minX ); 
	}

	int AABB::GetLengthY( void ) const
	{
		return abs(  m_maxY -  m_minY ); 
	}

	int AABB::GetLengthZ( void ) const
	{
		return abs(  m_maxZ -  m_minZ ); 
	}

	Rectangle AABB::GetXZRect( void ) const
	{
		return Rectangle(m_minX, m_minZ, GetLengthX(), GetLengthZ());
	}

	void AABB::DebugDraw(Render::RendererInterface* p_renderer, glm::vec3& p_color) const
	{
		glm::vec3 positions[8];
		positions[0] = glm::vec3(m_minX, m_minY, m_minZ);
		positions[1] = glm::vec3(m_maxX, m_minY, m_minZ);
		positions[2] = glm::vec3(m_minX, m_minY, m_maxZ);
		positions[3] = glm::vec3(m_maxX, m_minY, m_maxZ);

		positions[4] = glm::vec3(m_minX, m_maxY, m_minZ);
		positions[5] = glm::vec3(m_maxX, m_maxY, m_minZ);
		positions[6] = glm::vec3(m_minX, m_maxY, m_maxZ);
		positions[7] = glm::vec3(m_maxX, m_maxY, m_maxZ);

		unsigned int indices[] = 
		{ 
		  0, 2, 2, 3, 3, 1, 1, 0,
		  4, 6, 6, 7, 7, 5, 5, 4,
		  0, 4, 2, 6, 3, 7, 1, 5 
		};

		for(int i = 0; i < 24; i += 2)
		{
			p_renderer->AddLine(positions[indices[i]], positions[indices[i+1]], glm::vec4(p_color.x, p_color.y, p_color.z, 1.0f));
		}
	}
}

		
