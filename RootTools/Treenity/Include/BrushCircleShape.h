#pragma once

#include <RootTools/Treenity/Include/BrushBaseShape.h>

class BrushCircleShape : public BrushBaseShape
{
public:
	BrushCircleShape();
	~BrushCircleShape();

private:
	void CalculateBrush();
	void CalculateNormalBrush();
};