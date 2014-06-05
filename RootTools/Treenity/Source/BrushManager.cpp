#include <RootTools/Treenity/Include/BrushManager.h>
#include <RootTools/Treenity/Include/BrushCircle.h>

BrushManager::BrushManager()
{
	m_brushes[BrushShape::CIRCLE] = new BrushCircle();
	m_currentBrush = m_brushes[BrushShape::CIRCLE];
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
