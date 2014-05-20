#pragma once

#include <Utility/ECS/Include/World.h>
#include <Utility/ECS/Include/Entity.h>
#include <RootSystems/Include/Orientation.h>
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
};