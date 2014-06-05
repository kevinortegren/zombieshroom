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

class TerrainBrush
{
public:
	TerrainBrush();
	~TerrainBrush();

	void	SetSize(int p_size);
	void	SetStrength(float p_strength);
	void	SetFlat(bool p_flat);

	int		GetSize();
	float	GetStrength();
	bool	GetFlat();

	std::vector<BrushElement>* GetBrush();
	std::vector<BrushElement>* GetNormalBrush();

protected:
	
	//Implement this with desired brush shape algorithm
	virtual void CalculateBrush() = 0;

	std::vector<BrushElement> m_brushElements;
	std::vector<BrushElement> m_brushNormalElements;

	int m_size;
	float m_strength;
	bool m_flat;
};