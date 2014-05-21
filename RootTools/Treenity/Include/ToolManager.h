#pragma once

#include <RootTools/Treenity/Include/RotationTool.h>
#include <RootTools/Treenity/Include/TranslationTool.h>
#include <RootTools/Treenity/Include/Tool.h>

namespace ToolBox
{
	enum ToolBox
	{
		ROTATION_TOOL,
		TRANSLATION_TOOL,
		RESIZE_TOOL,
	};
}

class ToolManager
{
public:
	ToolManager();
	void Initialize(ECS::World* p_world);
	Tool* GetSelectedTool(); 
	void SetTool(ToolBox::ToolBox p_tool);

private:
	RotationTool m_rotation;
	TranslationTool m_translationTool;
	Tool* m_selectedTool;
};