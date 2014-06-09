#pragma once
#include <RootTools/Treenity/Include/BrushCircleShape.h>
#include <map>

namespace BrushShape
{
	enum BrushShape
	{
		CIRCLE,
	};
}

class TerrainBaseBrush
{
public:

	TerrainBaseBrush();
	~TerrainBaseBrush();

	std::vector<BrushElement>* GetCurrentNormalBrushShapeData(); 
	std::vector<BrushElement>* GetCurrentBrushShapeData(); 
	void SetBrushShape(BrushShape::BrushShape p_brush);

	std::string GetActiveMaterial();
	int GetActiveMaterialIndex();
	void SetActiveMaterial(int p_index);

	int GetNormalSize();
	int GetSize();
	void SetSize(int p_size);

	int GetOptionIndex();
	void SetOptionIndex(int p_index);

private:

	BrushBaseShape* m_currentBrush;
	std::map<BrushShape::BrushShape, BrushBaseShape*> m_brushes;

	int m_activeMaterial;

	//option index is used to keep track of custom settings of the brush
	int m_optionIndex;

	std::vector<std::string> m_materials;
};