#pragma once

#include <Utility/ECS/Include/World.h>
#include <RootTools/Treenity/Include/EditorInterface.h>

class Tool
{
public:
	Tool();
	virtual ~Tool() {}
	void SetEditorInterface(EditorInterface* p_interface);
	void UpdateRenderingPosition();
	void SetSelectedEntity(ECS::Entity* p_entity);
	ECS::Entity* GetSelectedEntity();
	bool IsVisible();

	virtual void LoadResources(ECS::World* p_world) = 0;
	virtual bool Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray) = 0;
	virtual void SetPosition(const glm::vec3& p_position) = 0;
	virtual void Hide() = 0;
	virtual void Show() = 0;

protected:
	ECS::World* m_world;
	ECS::Entity* m_selectedEntity;
	EditorInterface* m_editorInterface;
	bool m_visible;
};