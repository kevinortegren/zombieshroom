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

	std::string GetActiveMaterial();
	int GetActiveMaterialIndex();
	void SetActiveMaterial(int p_index);

private:

	TerrainBrush* m_currentBrush;
	std::map<BrushShape::BrushShape, TerrainBrush*> m_brushes;

	int m_activeMaterial;

	std::vector<std::string> m_materials;
};