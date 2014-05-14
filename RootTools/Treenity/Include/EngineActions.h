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
	void CreateTestSpawnpoint();
	void CreateWater();
	
	void ClearScene();
	void AddDefaultEntities();
	void InitializeScene();
	void LoadScene(const QString& p_filePath);

	// Mode switching
	void EnterPlayMode();
	void ExitPlayMode();
	EditorMode::EditorMode GetMode();
	void ParallelPlayModeEnter();

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
	std::string GetRenderableModelName(ECS::Entity* p_entity);
	std::string GetRenderableMaterialName(ECS::Entity* p_entity);
	void SetRenderableModelName(ECS::Entity* p_entity, std::string p_modelName);
	void SetRenderableMaterialName(ECS::Entity* p_entity, std::string p_materialName);

	//Physics
	virtual void AddPhysics(ECS::Entity* p_entity);
	virtual void RemovePhysics(ECS::Entity* p_entity);
	virtual float GetMass(ECS::Entity* p_entity);
	virtual glm::vec3& GetVelocity(ECS::Entity* p_entity);
	virtual void SetMass(ECS::Entity* p_entity, float p_mass);
	virtual void SetVelocity(ECS::Entity* p_entity, glm::vec3& p_velocity);

private:

	TreenityMain* m_treenityMain;
	ECS::World* m_world;
	EditorMode::EditorMode m_editorMode;
	std::string m_editorLevelState;

	ECS::Entity* m_cameraEntity;
	ECS::Entity* m_aimingDevice;
	ECS::Entity* m_testSpawnpoint;
	ECS::Entity* m_water;
};