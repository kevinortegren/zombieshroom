#pragma once
#include <RootTools/Treenity/Include/TerrainBaseBrush.h>

class TerrainGeometryBrush : public TerrainBaseBrush
{
public:
	TerrainGeometryBrush();
	~TerrainGeometryBrush();

	float GetStrength();
	void SetStrength(float p_strength);
	bool GetAutoSmooth();
	void SetAutoSmooth(bool p_val);
	bool GetSmoothOnly();
	void SetSmoothOnly(bool p_val);
	bool GetFlat();
	void SetFlat(bool p_val);
	bool GetPlateau();
	void SetPlateau(bool p_val);

private:

	float m_strength;
	bool m_autoSmooth;
	bool m_smoothOnly;
	bool m_flat;
	bool m_plateau;
};