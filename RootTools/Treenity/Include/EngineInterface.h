#pragma once

#include <Utility/ECS/Include/World.h>
#include <Utility/ECS/Include/Entity.h>
#include <RootSystems/Include/Orientation.h>
#include <glm/glm.hpp>

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
	virtual void AddPhysics(ECS::Entity* p_entity) = 0;
	virtual void RemovePhysics(ECS::Entity* p_entity) = 0;
	virtual float GetMass(ECS::Entity* p_entity) = 0;
	virtual glm::vec3& GetVelocity(ECS::Entity* p_entity) = 0;
	virtual void SetMass(ECS::Entity* p_entity, float p_mass) = 0;
	virtual void SetVelocity(ECS::Entity* p_entity, glm::vec3& p_velocity) = 0;

};