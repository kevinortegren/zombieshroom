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

	// World
	const ECS::World* GetWorld();

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

	Render::Material* GetMaterial(const std::string& p_string);
	std::string GetMaterialEffectName(Render::Material* p_material);

	Render::TextureInterface* GetTexture(const std::string& p_path);
	std::string GetTextureName(Render::TextureInterface* p_texture);
	
	Render::EffectInterface* GetEffect(const std::string& p_path);

	std::string GetContentPath();


	//Physics
	void ReconstructPhysicsObject(ECS::Entity* p_entity, bool p_dynamic, bool p_collideWithWorld, bool p_collideWithStatic, bool p_mass, RootEngine::Physics::PhysicsShape::PhysicsShape p_shape, float p_radius, float p_height, const std::string& p_meshHandle, bool p_visualize = false);
	void AddPhysics(ECS::Entity* p_entity, bool p_dynamic);
	void RemovePhysics(ECS::Entity* p_entity);
	RootEngine::Physics::PhysicsType::PhysicsType GetPhysicsType(ECS::Entity* p_entity);
	bool GetCollideWithWorld(ECS::Entity* p_entity);
	bool GetCollideWithStatic(ECS::Entity* p_entity);
	glm::vec3 GetGravity(ECS::Entity* p_entity);
	float GetMass(ECS::Entity* p_entity);
	RootEngine::Physics::PhysicsShape::PhysicsShape GetPhysicsShape(ECS::Entity* p_entity);
	float GetShapeRadius(ECS::Entity* p_entity);
	float GetShapeHeight(ECS::Entity* p_entity);
	std::string GetPhysicsMesh(ECS::Entity* p_entity);
	bool GetCollisionVisualized(ECS::Entity* p_entity);
	void SetPhysicsType(ECS::Entity* p_entity, bool p_dynamic);
	void SetCollideWithWorld(ECS::Entity* p_entity, bool p_collide);
	void SetCollideWithStatic(ECS::Entity* p_entity, bool p_collide);
	void SetGravity(ECS::Entity* p_entity, const glm::vec3& p_gravity);
	void SetMass(ECS::Entity* p_entity, float p_mass);
	void SetPhysicsShape(ECS::Entity* p_entity, RootEngine::Physics::PhysicsShape::PhysicsShape p_shape);
	void SetShapeRadius(ECS::Entity* p_entity, float p_radius);
	void SetShapeHeight(ECS::Entity* p_entity, float p_height);
	void SetPhysicsMesh(ECS::Entity* p_entity, const std::string& p_mesh);
	void SetCollisionVisualized(ECS::Entity* p_entity, bool p_visualize);

	//Water collider
	void	AddWaterCollider(ECS::Entity* p_entity);
	void	RemoveWaterCollider(ECS::Entity* p_entity);
	int		GetWaterColliderRadius(ECS::Entity* p_entity);
	float	GetWaterColliderInterval(ECS::Entity* p_entity);
	float	GetWaterColliderPower(ECS::Entity* p_entity);
	void	SetWaterColliderRadius(ECS::Entity* p_entity, int p_val);
	void	SetWaterColliderInterval(ECS::Entity* p_entity, float p_val);
	void	SetWaterColliderPower(ECS::Entity* p_entity, float p_val);

	//Script
	void		AddScript(ECS::Entity* p_entity);
	void		RemoveScript(ECS::Entity* p_entity);
	std::string GetScript(ECS::Entity* p_entity);
	void		SetScript(ECS::Entity* p_entity, std::string p_script);

	//Collision responder
	void		AddCollisionResponder(ECS::Entity* p_entity); 
	void		RemoveCollisionResponder(ECS::Entity* p_entity);

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