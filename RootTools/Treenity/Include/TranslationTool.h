#pragma once

#include <RootTools/Treenity/Include/Tool.h>
#include <RootEngine/Render/Include/Material.h>
#include <RootTools/Treenity/Include/EngineInterface.h>

namespace TranslationAxis
{
	enum TranslationAxis
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_NONE,
	};
}

class TranslationTool : public Tool
{
public:
	TranslationTool();
	~TranslationTool();

	void LoadResources(ECS::World* p_world);
	bool Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);

	void SetPosition(const glm::vec3& p_position);
	void Hide();
	void Show();

private:



};