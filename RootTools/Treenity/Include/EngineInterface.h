#pragma once

#include <Utility/ECS/Include/Entity.h>
#include <glm/glm.hpp>

struct EngineInterface
{
	// Entity
	virtual ECS::Entity* CreateEntity() = 0;
	virtual void DeleteEntity(ECS::Entity* p_entity) = 0;

	// Transform
	virtual void TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation) = 0;

	// Renderable
	virtual void AddRenderable(ECS::Entity* p_entity) = 0;
	virtual void RemoveRenderable(ECS::Entity* p_entity) = 0;
};