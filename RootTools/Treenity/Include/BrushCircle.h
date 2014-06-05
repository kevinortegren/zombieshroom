#pragma once

#include <RootTools/Treenity/Include/Brush.h>

class BrushCircle : public TerrainBrush
{
public:
	BrushCircle();
	~BrushCircle();
private:
	void CalculateBrush();
};