#include <RootTools/Treenity/Include/BrushBaseShape.h>

BrushBaseShape::BrushBaseShape()
	: m_size(0)
{

}

BrushBaseShape::~BrushBaseShape()
{

}

//Set an attribute of the brush, this will clear the old brush data and calculate a new vector of brush elements
void BrushBaseShape::SetSize( int p_size )
{
	m_size = p_size;
	m_brushElements.clear();
	m_brushNormalElements.clear();
	CalculateBrush();
	CalculateNormalBrush();
}

//Getters
int BrushBaseShape::GetSize()
{
	return m_size;
}

int BrushBaseShape::GetNormalSize()
{
	return m_size + 1;
}

//Get a pointer to the brush element data
std::vector<BrushElement>* BrushBaseShape::GetBrushShapeData()
{
	return &m_brushElements;
}

std::vector<BrushElement>* BrushBaseShape::GetBrushNormalShapeData()
{
	return &m_brushNormalElements;
}

