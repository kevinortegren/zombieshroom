#pragma once

#include <RootTools/Treenity/Include/EngineInterface.h>
#include <Utility/ECS/Include/World.h>

class EngineActions : public EngineInterface
{

public:
	EngineActions(ECS::World* p_world);

	// Entity
	ECS::Entity* CreateEntity();
	void DeleteEntity(ECS::Entity* p_entity);

	// Transform
	void TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation);

	// Renderable
	void AddRenderable(ECS::Entity* p_entity);
	void RemoveRenderable(ECS::Entity* p_entity);
private:
	ECS::World* m_world;
};