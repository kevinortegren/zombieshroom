#include <RootTools/Treenity/Include/TerrainTextureBrush.h>

TerrainTextureBrush::TerrainTextureBrush() : TerrainBaseBrush(), m_opacity(1.0f), m_channelIndex(0)
{
	
}

TerrainTextureBrush::~TerrainTextureBrush()
{

}

int TerrainTextureBrush::GetChannel()
{
	return m_channelIndex;
}

void TerrainTextureBrush::SetChannel( int p_channel )
{
	m_channelIndex = p_channel;
}

float TerrainTextureBrush::GetOpacity()
{
	return m_opacity;
}

void TerrainTextureBrush::SetOpacity( float p_opacity )
{
	m_opacity = p_opacity;
}
