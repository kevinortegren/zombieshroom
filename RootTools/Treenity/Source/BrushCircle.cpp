#include <RootTools/Treenity/Include/BrushCircle.h>

BrushCircle::BrushCircle() : TerrainBrush()
{

}

BrushCircle::~BrushCircle()
{

}

void BrushCircle::CalculateBrush()
{
	for(int i = m_size; abs(i) <= m_size; i--)
	{
		m_brushElements.push_back(BrushElement(glm::ivec2(0, i), (1 - abs(i) / (float)m_size) * m_strength));

		for (int j = 1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size; j++)
		{
			m_brushElements.push_back(BrushElement(glm::ivec2(j, i), (1 - ((sqrt(pow((float)j,2) + pow((float)i,2))) / (float)m_size)) * m_strength));
		}

		for (int j = -1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size; j--)
		{
			m_brushElements.push_back(BrushElement(glm::ivec2(j, i), (1 - ((sqrt(pow((float)j,2) + pow((float)i,2))) / (float)m_size)) * m_strength));
		}
	}

	//Calculate normal brush( used for calculating normals). Needs to be bigger than the other brush
	for(int i = m_size+1; abs(i) <= m_size+1; i--)
	{
		m_brushNormalElements.push_back(BrushElement(glm::ivec2(0, i), (1 - abs(i) / (float)m_size) * m_strength));

		for (int j = 1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size; j++)
		{
			m_brushNormalElements.push_back(BrushElement(glm::ivec2(j, i), (1 - ((sqrt(pow((float)j,2) + pow((float)i,2))) / (float)m_size)) * m_strength));
		}

		for (int j = -1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size; j--)
		{
			m_brushNormalElements.push_back(BrushElement(glm::ivec2(j, i), (1 - ((sqrt(pow((float)j,2) + pow((float)i,2))) / (float)m_size)) * m_strength));
		}
	}
}

