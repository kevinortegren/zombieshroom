#include <RootTools/Treenity/Include/ToolManager.h>

ToolManager::ToolManager()
	: m_selectedTool(nullptr)
{

}

void ToolManager::Initialize(ECS::World* p_world)
{
	m_rotation.LoadResources(p_world);
}

Tool* ToolManager::GetSelectedTool()
{
	return m_selectedTool;
}

void ToolManager::SetTool()
{
	m_selectedTool = &m_rotation;
}