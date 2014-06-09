#include <RootTools/Treenity/Include/TerrainBaseBrush.h>

TerrainBaseBrush::TerrainBaseBrush() : m_activeMaterial(0), m_optionIndex(0)
{
	m_brushes[BrushShape::CIRCLE] = new BrushCircleShape();
	m_currentBrush = m_brushes[BrushShape::CIRCLE];

	//Some hardcoded material names
	m_materials.push_back("terrainBrushMatCircle");
	m_materials.push_back("terrainBrushMatWireframe");
}

TerrainBaseBrush::~TerrainBaseBrush()
{
	for(auto br : m_brushes)
	{
		delete br.second;
	}

	m_brushes.clear();
}

std::vector<BrushElement>* TerrainBaseBrush::GetCurrentBrushShapeData()
{
	return m_currentBrush->GetBrushShapeData();
}

std::vector<BrushElement>* TerrainBaseBrush::GetCurrentNormalBrushShapeData()
{
	return m_currentBrush->GetBrushNormalShapeData();
}

void TerrainBaseBrush::SetBrushShape( BrushShape::BrushShape p_brush )
{
	m_currentBrush = m_brushes[p_brush];
}

std::string TerrainBaseBrush::GetActiveMaterial()
{
	return m_materials[m_activeMaterial];
}

int TerrainBaseBrush::GetActiveMaterialIndex()
{
	return m_activeMaterial;
}

void TerrainBaseBrush::SetActiveMaterial( int p_index )
{
	m_activeMaterial = p_index;
}

int TerrainBaseBrush::GetSize()
{
	return m_currentBrush->GetSize();
}

int TerrainBaseBrush::GetNormalSize()
{
	return m_currentBrush->GetNormalSize();
}

void TerrainBaseBrush::SetSize( int p_size )
{
	m_currentBrush->SetSize(p_size);
}

int TerrainBaseBrush::GetOptionIndex()
{
	return m_optionIndex;
}

void TerrainBaseBrush::SetOptionIndex( int p_index )
{
	m_optionIndex = p_index;
}



