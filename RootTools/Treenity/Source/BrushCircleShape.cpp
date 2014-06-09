#include <RootTools/Treenity/Include/BrushCircleShape.h>

BrushCircleShape::BrushCircleShape() : BrushBaseShape()
{

}

BrushCircleShape::~BrushCircleShape()
{

}

void BrushCircleShape::CalculateBrush()
{
	for(int i = m_size; abs(i) <= m_size; i--)
	{
		m_brushElements.push_back(BrushElement(glm::ivec2(0, i), (1 - abs(i) / (float)m_size)));

		for (int j = 1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size; j++)
		{
			m_brushElements.push_back(BrushElement(glm::ivec2(j, i), (1 - ((sqrt(pow((float)j,2) + pow((float)i,2))) / (float)m_size))));
		}

		for (int j = -1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size; j--)
		{
			m_brushElements.push_back(BrushElement(glm::ivec2(j, i), (1 - ((sqrt(pow((float)j,2) + pow((float)i,2))) / (float)m_size))));
		}
	}
}

void BrushCircleShape::CalculateNormalBrush()
{
	for(int i = m_size + 1; abs(i) <= m_size + 1; i--)
	{
		m_brushNormalElements.push_back(BrushElement(glm::ivec2(0, i),0));

		for (int j = 1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size + 1; j++)
		{
			m_brushNormalElements.push_back(BrushElement(glm::ivec2(j, i), 0));
		}

		for (int j = -1; sqrt(pow((float)j, 2) + pow((float)i, 2)) <= m_size + 1; j--)
		{
			m_brushNormalElements.push_back(BrushElement(glm::ivec2(j, i), 0));
		}
	}
}

