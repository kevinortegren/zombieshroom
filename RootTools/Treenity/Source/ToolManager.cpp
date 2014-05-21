#include <RootTools/Treenity/Include/ToolManager.h>

ToolManager::ToolManager()
	: m_selectedTool(&m_translationTool)
{

}

void ToolManager::Initialize(ECS::World* p_world)
{
	m_rotation.LoadResources(p_world);
	m_translationTool.LoadResources(p_world);
}

Tool* ToolManager::GetSelectedTool()
{
	return m_selectedTool;
}

void ToolManager::SetTool(ToolBox::ToolBox p_tool)
{
	m_rotation.Hide();
	m_translationTool.Hide();
	switch (p_tool)
	{
	case ToolBox::ROTATION_TOOL:
		m_rotation.SetSelectedEntity(GetSelectedTool()->GetSelectedEntity());
		m_selectedTool = &m_rotation;
		break;
	case ToolBox::TRANSLATION_TOOL:
		m_translationTool.SetSelectedEntity(GetSelectedTool()->GetSelectedEntity());
		m_selectedTool = &m_translationTool;
		break;
	case ToolBox::RESIZE_TOOL:
		break;
	default:
		break;
	}
	
}