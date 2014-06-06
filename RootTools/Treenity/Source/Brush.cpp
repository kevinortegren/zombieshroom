#include <RootTools/Treenity/Include/Brush.h>

TerrainBrush::TerrainBrush()
	: m_size(0), m_strength(0.0f), m_flat(false), m_autoSmooth(false), m_smoothOnly(false)
{

}

TerrainBrush::~TerrainBrush()
{

}

//Set an attribute of the brush, this will clear the old brush data and calculate a new vector of brush elements
void TerrainBrush::SetSize( int p_size )
{
	m_size = p_size;
	m_brushElements.clear();
	m_brushNormalElements.clear();
	CalculateBrush();
}

void TerrainBrush::SetStrength( float p_strength )
{
	m_strength = p_strength;
	m_brushElements.clear();
	m_brushNormalElements.clear();
	CalculateBrush();
}

void TerrainBrush::SetFlat( bool p_flat )
{
	m_flat = p_flat;
	m_brushElements.clear();
	m_brushNormalElements.clear();
	CalculateBrush();
}

//Getters
int TerrainBrush::GetSize()
{
	return m_size;
}

float TerrainBrush::GetStrength()
{
	return m_strength;
}

bool TerrainBrush::GetFlat()
{
	return m_flat;
}

//Get a pointer to the brush element data
std::vector<BrushElement>* TerrainBrush::GetBrush()
{
	return &m_brushElements;
}

std::vector<BrushElement>* TerrainBrush::GetNormalBrush()
{
	return &m_brushNormalElements;
}

void TerrainBrush::SetSmoothOnly( bool p_smoothOnly )
{
	m_smoothOnly = p_smoothOnly;
}

void TerrainBrush::SetAutoSmooth( bool p_autoSmooth )
{
	m_autoSmooth = p_autoSmooth;
}

bool TerrainBrush::GetSmoothOnly()
{
	return m_smoothOnly;
}

bool TerrainBrush::GetAutoSmooth()
{
	return m_autoSmooth;
}
