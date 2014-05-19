#pragma once

#include <Utility/ECS/Include/World.h>

class Tool
{
public:
	virtual ~Tool() {}
	virtual void LoadResources(ECS::World* p_world) = 0;
	virtual bool Pick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray) = 0;
	virtual void Update() = 0;
	virtual void Hide() = 0;
	virtual void Show() = 0;

	virtual void SetSelectedEntity(ECS::Entity* p_entity);
	virtual ECS::Entity* GetSelectedEntity();

private:
	ECS::Entity* m_selectedEntity;

};