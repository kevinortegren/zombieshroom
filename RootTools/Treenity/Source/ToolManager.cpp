#include <RootTools/Treenity/Include/ToolManager.h>

ToolManager::ToolManager()
{
	//Add tools to the toolbox here
	m_tools[ToolBox::ROTATION_TOOL] = new RotationTool();
	m_tools[ToolBox::TRANSLATION_TOOL] = new TranslationTool();

	//This is the default tool, must correspond to what is selected by the GUI by default (Currently translation tool)
	m_selectedTool = m_tools[ToolBox::TRANSLATION_TOOL];
}

void ToolManager::Initialize(ECS::World* p_world)
{
	//Load all the tools by looping through and running LoadResources
	for(auto tool = m_tools.begin(); tool != m_tools.end(); tool++)
	{
		tool->second->LoadResources(p_world);
	}
}

Tool* ToolManager::GetSelectedTool()
{
	return m_selectedTool;
}

void ToolManager::SetTool(ToolBox::ToolBox p_tool)
{
	//Disregard tool change if setting same tool as currently selected
	if(m_tools[p_tool] != m_selectedTool)
	{
		//Hide the current tool
		m_selectedTool->Hide();

		//Transfer the selected entity from the previous tool
		m_tools[p_tool]->SetSelectedEntity(m_selectedTool->GetSelectedEntity());

		//Check if the new tool is visible and show it if its not
		if(!m_tools[p_tool]->IsVisible())
			m_tools[p_tool]->Show();

		//Set new selected tool
		m_selectedTool = m_tools[p_tool];
	}
		
}