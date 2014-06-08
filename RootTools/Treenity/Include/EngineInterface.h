#pragma once

#include <Utility/ECS/Include/World.h>
#include <Utility/ECS/Include/Entity.h>
#include <RootSystems/Include/Orientation.h>
#include <RootEngine/Render/Include/Material.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <glm/glm.hpp>
#include <QString>

namespace EditorMode
{
	enum EditorMode
	{
		EDITOR,
		GAME
	};
}

struct EngineInterface
{
	// Project management
	virtual void NewScene() = 0;
	virtual void ClearScene() = 0;
	virtual void AddDefaultEntities() = 0;
	virtual void InitializeScene() = 0;
	virtual void LoadScene(const QString& p_filePath) = 0;

	// Mode switching
	virtual void EnterPlayMode() = 0;
	virtual void ExitPlayMode() = 0;
	virtual EditorMode::EditorMode GetMode() = 0;

	// Entity
	virtual ECS::Entity* CreateEntity() = 0;
	virtual void DeleteEntity(ECS::Entity* p_entity) = 0;
	virtual void TargetEntity(ECS::Entity* p_entity) = 0;
	virtual ECS::Entity* GetEntityByTag(const std::string& p_tag) = 0;
	virtual void DuplicateEntity(ECS::Entity* p_entity) = 0;

	//Special entities
	virtual ECS::Entity* CreateTerrainEntity(int p_width, int p_height) = 0; 
	virtual void UpdateTerrainPhysicsMesh() = 0;
	// World
	virtual const ECS::World* GetWorld() = 0;

	//Water settings
	virtual void SetWaterParam(std::stringstream* p_ss) = 0;
	virtual void SetWaterColor(const glm::vec3& p_color) = 0;
	virtual float GetWaterSpeed() = 0;
	virtual float GetWaterDepth() = 0;
	virtual bool GetWaterNormals() = 0;
	virtual bool GetWaterReflections() = 0;
	virtual bool GetWaterRefractions() = 0;
	virtual bool GetWaterPaused() = 0;
	virtual glm::vec3 GetWaterColor() = 0;

	// Transform
	virtual void SetPosition(ECS::Entity* p_entity, const glm::vec3& p_position) = 0;
	virtual void TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation) = 0;
	virtual void SetOrientation(ECS::Entity* p_entity, const RootForce::Orientation& p_orientation) = 0;
	// TODO: Add RotateEntity
	virtual void SetScale(ECS::Entity* p_entity, const glm::vec3& p_scale) = 0;
	// TODO: Add ScaleEntity?
	virtual const glm::vec3& GetPosition(ECS::Entity* p_entity) = 0;
	virtual RootForce::Orientation& GetOrientation(ECS::Entity* p_entity) = 0;
	virtual const glm::vec3& GetScale(ECS::Entity* p_entity) = 0;

	// Renderable
	virtual void AddRenderable(ECS::Entity* p_entity) = 0;
	virtual void RemoveRenderable(ECS::Entity* p_entity) = 0;
	virtual std::string GetRenderableModelName(ECS::Entity* p_entity) = 0;
	virtual std::string GetRenderableMaterialName(ECS::Entity* p_entity) = 0;
	virtual void SetRenderableModelName(ECS::Entity* p_entity, std::string p_modelName) = 0;
	virtual void SetRenderableMaterialName(ECS::Entity* p_entity, std::string p_materialName) = 0;
	virtual std::string GetMaterialEffectName(Render::Material* p_material) = 0;
	virtual Render::Material* GetMaterial(const std::string& p_string) = 0;
	virtual Render::TextureInterface* GetTexture(const std::string& p_path) = 0;
	virtual std::string GetTextureName(Render::TextureInterface* p_texture) = 0;
	virtual std::string GetContentPath() = 0;
	virtual Render::EffectInterface* GetEffect(const std::string& p_path) = 0;

	//Physics
	virtual void AddPhysics(ECS::Entity* p_entity, bool p_dynamic) = 0;
	virtual void RemovePhysics(ECS::Entity* p_entity) = 0;
	virtual RootEngine::Physics::PhysicsType::PhysicsType GetPhysicsType(ECS::Entity* p_entity) = 0;
	virtual bool GetCollideWithWorld(ECS::Entity* p_entity) = 0;
	virtual bool GetCollideWithStatic(ECS::Entity* p_entity) = 0;
	virtual glm::vec3 GetGravity(ECS::Entity* p_entity) = 0;
	virtual float GetMass(ECS::Entity* p_entity) = 0;
	virtual RootEngine::Physics::PhysicsShape::PhysicsShape GetPhysicsShape(ECS::Entity* p_entity) = 0;
	virtual float GetShapeRadius(ECS::Entity* p_entity) = 0;
	virtual float GetShapeHeight(ECS::Entity* p_entity) = 0;
	virtual std::string GetPhysicsMesh(ECS::Entity* p_entity) = 0;
	virtual void SetPhysicsType(ECS::Entity* p_entity, bool p_dynamic) = 0;
	virtual void SetCollideWithWorld(ECS::Entity* p_entity, bool p_collide) = 0;
	virtual void SetCollideWithStatic(ECS::Entity* p_entity, bool p_collide) = 0;
	virtual void SetGravity(ECS::Entity* p_entity, const glm::vec3& p_gravity) = 0;
	virtual void SetMass(ECS::Entity* p_entity, float p_mass) = 0;
	virtual void SetPhysicsShape(ECS::Entity* p_entity, RootEngine::Physics::PhysicsShape::PhysicsShape p_shape) = 0;
	virtual void SetShapeRadius(ECS::Entity* p_entity, float p_radius) = 0;
	virtual void SetShapeHeight(ECS::Entity* p_entity, float p_height) = 0;
	virtual void SetPhysicsMesh(ECS::Entity* p_entity, const std::string& p_mesh) = 0;
	virtual void SetCollisionVisualized(ECS::Entity* p_entity, bool p_visualize) = 0;

	//Water collider
	virtual void	AddWaterCollider(ECS::Entity* p_entity) = 0;
	virtual void	RemoveWaterCollider(ECS::Entity* p_entity) = 0;
	virtual int		GetWaterColliderRadius(ECS::Entity* p_entity) = 0;
	virtual float	GetWaterColliderInterval(ECS::Entity* p_entity) = 0;
	virtual float	GetWaterColliderPower(ECS::Entity* p_entity) = 0;
	virtual void	SetWaterColliderRadius(ECS::Entity* p_entity, int p_val) = 0;
	virtual void	SetWaterColliderInterval(ECS::Entity* p_entity, float p_val) = 0;
	virtual void	SetWaterColliderPower(ECS::Entity* p_entity, float p_val) = 0;

	//Script
	virtual void		AddScript(ECS::Entity* p_entity) = 0;
	virtual void		RemoveScript(ECS::Entity* p_entity) = 0;
	virtual std::string GetScript(ECS::Entity* p_entity) = 0;
	virtual void		SetScript(ECS::Entity* p_entity, std::string p_script) = 0;

	//Collision responder
	virtual void		AddCollisionResponder(ECS::Entity* p_entity) = 0;
	virtual void		RemoveCollisionResponder(ECS::Entity* p_entity) = 0;

	//Particle
	virtual void		AddParticle(ECS::Entity* p_entity) = 0;
	virtual void		RemoveParticle(ECS::Entity* p_entity) = 0;
	virtual void		SetParticleEmitter(ECS::Entity* p_entity, const std::string& p_name) = 0;
	virtual std::string	GetParticleEmitter(ECS::Entity* p_entity) = 0;
};