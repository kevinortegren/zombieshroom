#pragma once
#include <RootTools/Treenity/Include/Brush.h>
#include <RootTools/Treenity/Include/BrushCircle.h>
#include <map>

namespace BrushShape
{
	enum BrushShape
	{
		CIRCLE,
	};
}

class BrushManager
{
public:
	BrushManager();
	~BrushManager();

	TerrainBrush* GetCurrentBrush(); 
	void SetBrushShape(BrushShape::BrushShape p_brush);

private:

	TerrainBrush* m_currentBrush;
	std::map<BrushShape::BrushShape, TerrainBrush*> m_brushes;
};