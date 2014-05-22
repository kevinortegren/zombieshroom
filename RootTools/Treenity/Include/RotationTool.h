#pragma once

#include <RootTools/Treenity/Include/Tool.h>
#include <RootEngine/Render/Include/Material.h>
#include <RootTools/Treenity/Include/EngineInterface.h>

namespace RotationAxis
{
	enum RotationAxis
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_NONE,
	};
}

class RotationTool : public Tool
{
public:
	RotationTool();
	~RotationTool();

	void LoadResources(ECS::World* p_world);
	bool Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);

	void SetPosition(const glm::vec3& p_position);
	void Hide();
	void Show();

private:

	float GetAngleFromAxis(RotationAxis::RotationAxis axis, const glm::vec3& position);
	bool RayVsPlane(const glm::vec3& center, const glm::vec3& normal, const glm::vec3& cameraPos, const glm::vec3& ray, float& t);

	static const float s_pickMargin;
	static const glm::vec4 s_highlightColor;

	Render::Material* m_axisMaterial;
	ECS::Entity* m_axisEntities[3];
	glm::vec4 m_axisColors[3];
	RotationAxis::RotationAxis m_selectedAxis;
	RootForce::Orientation m_gimbalOrientation;
	RootForce::Orientation m_entityOrientation;
	float m_angle0;
};