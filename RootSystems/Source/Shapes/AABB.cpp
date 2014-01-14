#include <RootSystems/Include/Shapes/AABB.h>>
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

	Rectangle AABB::GetRect( void ) const
	{
		return Rectangle(m_minX, m_minZ, GetLengthX(), GetLengthZ());
	}
}

		
