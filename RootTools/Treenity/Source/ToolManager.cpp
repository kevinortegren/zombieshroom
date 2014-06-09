#include <RootTools/Treenity/Include/ToolManager.h>
#include <RootTools/Treenity/Include/RotationTool.h>
#include <RootTools/Treenity/Include/TranslationTool.h>
#include <RootTools/Treenity/Include/TerrainTool.h>
#include <RootTools/Treenity/Include/TerrainTextureTool.h>

ToolManager::ToolManager()
{
	//Add tools to the toolbox here
	m_tools[ToolBox::ROTATION_TOOL] = new RotationTool();
	m_tools[ToolBox::TRANSLATION_TOOL] = new TranslationTool();
	m_tools[ToolBox::TERRAIN_TOOL] = new TerrainTool();
	m_tools[ToolBox::TERRAIN_TEXTURE_TOOL] = new TerrainTextureTool();

	//This is the default tool, must correspond to what is selected by the GUI by default (Currently translation tool)
	m_selectedTool = m_tools[ToolBox::TRANSLATION_TOOL];
}

void ToolManager::SetEditorInterface(EditorInterface* p_editorInterface)
{
	m_editorInterface = p_editorInterface;

	for(auto tool = m_tools.begin(); tool != m_tools.end(); tool++)
	{
		tool->second->SetEditorInterface(p_editorInterface);
	}
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
	// If changing tool.
	if(m_tools[p_tool] != m_selectedTool)
	{
		m_selectedTool->SetSelectedEntity(nullptr);
		m_selectedTool = m_tools[p_tool];
	}

	if(m_editorInterface->GetSelection().size() != 0)
	{
		m_selectedTool->SetSelectedEntity(*m_editorInterface->GetSelection().begin());
	}
}