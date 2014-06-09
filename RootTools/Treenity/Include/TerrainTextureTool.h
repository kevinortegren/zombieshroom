#pragma once

#include <RootSystems/Include/Transform.h>
#include <RootTools/Treenity/Include/Tool.h>
#include <RootTools/Treenity/Include/EngineInterface.h>
#include <RootTools/Treenity/Include/TerrainTextureBrush.h>

class TerrainTextureTool : public Tool
{
public:
	TerrainTextureTool();
	~TerrainTextureTool();

	void LoadResources(ECS::World* p_world);
	bool Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);

	void SetPosition(const glm::vec3& p_position);
	void Hide();
	void Show();
private:

	int GetVertexPosition(const glm::ivec2& p_pos);
	glm::ivec2 GetRayMarchCollision(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);
	bool IsCoordInsideTerrain(glm::ivec2 p_pos);
	unsigned char c_clamp(float x, float a, float b);
	RootForce::Transform* m_terrainTrans;
	Render::Vertex1P1N1UV* m_vertexData;

	int m_width;
	int m_numElements;

	float m_timer;

	glm::vec4 m_shaderdata;
	glm::mat4 m_terrainModelMatrix;

};