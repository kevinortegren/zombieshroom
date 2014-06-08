#include <RootTools/Treenity/Include/BrushManager.h>
#include <RootTools/Treenity/Include/BrushCircle.h>

BrushManager::BrushManager() : m_activeMaterial(0)
{
	m_brushes[BrushShape::CIRCLE] = new BrushCircle();
	m_currentBrush = m_brushes[BrushShape::CIRCLE];

	//Some hardcoded material names
	m_materials.push_back("terrainBrushMatCircle");
	m_materials.push_back("terrainBrushMatWireframe");
}

BrushManager::~BrushManager()
{

}

TerrainBrush* BrushManager::GetCurrentBrush()
{
	return m_currentBrush;
}

void BrushManager::SetBrushShape( BrushShape::BrushShape p_brush )
{
	m_currentBrush = m_brushes[p_brush];
}

std::string BrushManager::GetActiveMaterial()
{
	return m_materials[m_activeMaterial];
}

void BrushManager::SetActiveMaterial( int p_index )
{
	m_activeMaterial = p_index;
}

int BrushManager::GetActiveMaterialIndex()
{
	return m_activeMaterial;
}
