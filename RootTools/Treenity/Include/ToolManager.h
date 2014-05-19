#pragma once

#include <RootTools/Treenity/Include/RotationTool.h>
#include <RootTools/Treenity/Include/Tool.h>

class ToolManager
{
public:
	ToolManager();
	void Initialize(ECS::World* p_world);
	Tool* GetSelectedTool(); 
	void SetTool();

private:
	RotationTool m_rotation;
	Tool* m_selectedTool;
};