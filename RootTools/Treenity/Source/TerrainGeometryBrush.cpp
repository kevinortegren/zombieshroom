#include <RootTools/Treenity/Include/TerrainGeometryBrush.h>

TerrainGeometryBrush::TerrainGeometryBrush() : TerrainBaseBrush(), m_autoSmooth(false), m_flat(false), m_smoothOnly(false), m_strength(3.0f), m_plateau(false)
{
}

TerrainGeometryBrush::~TerrainGeometryBrush()
{
}

float TerrainGeometryBrush::GetStrength()
{
	return m_strength;
}

void TerrainGeometryBrush::SetStrength( float p_strength )
{
	m_strength = p_strength;
}

bool TerrainGeometryBrush::GetAutoSmooth()
{
	return m_autoSmooth;
}

void TerrainGeometryBrush::SetAutoSmooth( bool p_val )
{
	m_autoSmooth = p_val;
}

bool TerrainGeometryBrush::GetSmoothOnly()
{
	return m_smoothOnly;
}

void TerrainGeometryBrush::SetSmoothOnly( bool p_val )
{
	m_smoothOnly = p_val;
}

bool TerrainGeometryBrush::GetFlat()
{
	return m_flat;
}

void TerrainGeometryBrush::SetFlat( bool p_val )
{
	m_flat = p_val;
}

bool TerrainGeometryBrush::GetPlateau()
{
	return m_plateau;
}

void TerrainGeometryBrush::SetPlateau( bool p_val )
{
	m_plateau = p_val;
}
