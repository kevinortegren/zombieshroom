#include <RootTools/Treenity/Include/TranslationTool.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/Transform.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/InputManager/Include/InputManager.h>

extern RootEngine::GameSharedContext g_engineContext;

TranslationTool::TranslationTool()
{
}

TranslationTool::~TranslationTool()
{
}

void TranslationTool::LoadResources(ECS::World* p_world)
{
	m_world = p_world;
}

bool TranslationTool::Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray)
{
	if(m_selectedEntity == nullptr)
		return false;
}

void TranslationTool::SetPosition(const glm::vec3& p_position)
{
}

void TranslationTool::Hide()
{
}

void TranslationTool::Show()
{
}
