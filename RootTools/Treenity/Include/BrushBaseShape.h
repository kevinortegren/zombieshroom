#pragma once

#include "glm/glm.hpp"
#include <vector>

struct BrushElement
{
	BrushElement(){}
	BrushElement(glm::ivec2 p_pos, float p_strength) : pos(p_pos), strength(p_strength){}

	glm::ivec2 pos;
	float strength;
};

class BrushBaseShape
{
public:
	BrushBaseShape();
	~BrushBaseShape();

	void	SetSize(int p_size);
	int		GetSize();

	int		GetNormalSize();

	std::vector<BrushElement>* GetBrushShapeData();
	std::vector<BrushElement>* GetBrushNormalShapeData();

protected:
	
	//Implement this with desired brush shape algorithm
	virtual void CalculateBrush() = 0;
	virtual void CalculateNormalBrush() = 0;

	std::vector<BrushElement> m_brushElements;
	std::vector<BrushElement> m_brushNormalElements;

	int m_size;
};