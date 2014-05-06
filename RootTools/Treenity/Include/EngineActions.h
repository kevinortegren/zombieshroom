#pragma once

#include <RootTools/Treenity/Include/EngineInterface.h>

#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/WorldSystem.h>

class TreenityMain;

class EngineActions : public EngineInterface
{

public:
	EngineActions(ECS::World* p_world, TreenityMain* p_treenityMain);

	// Project startup
	void NewScene();
	void CreateFreeFlyingCamera();
	
	void ClearScene();
	void AddDefaultEntities();
	void InitializeScene();

	// Entity
	ECS::Entity* CreateEntity();
	void DeleteEntity(ECS::Entity* p_entity);

	void TargetEntity(ECS::Entity* p_entity);

	// Transform
	void SetPosition(ECS::Entity* p_entity, const glm::vec3& p_position);
	void TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation);
	void SetOrientation(ECS::Entity* p_entity, const RootForce::Orientation& p_orientation);
	void SetScale(ECS::Entity* p_entity, const glm::vec3& p_scale);
	const glm::vec3& GetPosition(ECS::Entity* p_entity);
	RootForce::Orientation& GetOrientation(ECS::Entity* p_entity);
	const glm::vec3& GetScale(ECS::Entity* p_entity);

	// Renderable
	void AddRenderable(ECS::Entity* p_entity);
	void RemoveRenderable(ECS::Entity* p_entity);
private:

	TreenityMain* m_treenityMain;
	ECS::World* m_world;

	ECS::Entity* m_cameraEntity;
	ECS::Entity* m_aimingDevice;
};