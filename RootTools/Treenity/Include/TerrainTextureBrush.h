#pragma once
#include <RootTools/Treenity/Include/TerrainBaseBrush.h>

class TerrainTextureBrush : public TerrainBaseBrush
{
public:
	TerrainTextureBrush();
	~TerrainTextureBrush();

	int GetChannel();
	void SetChannel(int p_channel);

	float GetOpacity();
	void SetOpacity(float p_opacity);
private:

	int m_channelIndex;
	float m_opacity;
};