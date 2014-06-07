#pragma once

#include <RootSystems/Include/Transform.h>
#include <RootTools/Treenity/Include/Tool.h>
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/BrushManager.h>

class TerrainTool : public Tool
{
public:
	TerrainTool();
	~TerrainTool();

	void LoadResources(ECS::World* p_world);
	bool Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);

	void SetPosition(const glm::vec3& p_position);
	void Hide();
	void Show();
private:

	int GetVertexPosition(const glm::ivec2& p_pos);
	float AverageHeight(const glm::ivec2& p_pos);
	glm::ivec2 GetRayMarchCollision(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);
	bool IsCoordInsideTerrain(glm::ivec2 p_pos);
	glm::vec3 CalcNormalOnCoord(const glm::ivec2& p_pos);

	RootForce::Transform* m_terrainTrans;
	Render::Vertex1P1N1UV* m_vertexData;
	int m_width;
	int m_numElements;

	float m_timer;

};