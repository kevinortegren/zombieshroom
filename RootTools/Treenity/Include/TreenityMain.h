#pragma once

// Root Systems
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/TransformInterpolationSystem.h>
#include <RootSystems/Include/ScriptSystem.h>
#include <RootSystems/Include/ControllerActionSystem.h>
#include <RootSystems/Include/ShadowSystem.h>
#include <RootSystems/Include/LightSystem.h>

// Treenity
#include <RootTools/Treenity/Include/EngineActions.h>
#include <RootTools/Treenity/Include/Treenity.h>
#include <RootTools/Treenity/Include/ProjectManager.h>
#include <RootTools/Treenity/Include/GlobalKeys.h>

class TreenityMain
{
public:
	TreenityMain(const std::string& p_path);
	~TreenityMain();

	void HandleEvents();
	void ProcessWorldMessages();
	void HandleAltModifier();
	
	void Update(float dt);
	bool IsRunning();

	Treenity* GetEditor() { return &m_treenityEditor; }
	RootForce::WorldSystem* GetWorldSystem() { return &m_worldSystem; }
	RootForce::ShadowSystem* GetShadowSystem() { return m_shadowSystem; }

private:

	void RenderSelectedEntity();
	void RaySelect();

	const glm::vec3& ConstructRay();
	bool RayVsSphere(const glm::vec3& cameraPos, const glm::vec3& ray, const glm::vec3& center, float radius, float& t);
	bool RayVsOBB(const glm::vec3& cameraPos, const glm::vec3& ray, RootEngine::OBB* obb, const glm::mat4x4& transform, float& t);
	bool RayVsTriangle(const glm::vec3& cameraPos, const glm::vec3& ray, const RootEngine::Model* model, const glm::mat4x4& transform, float& t);

	void Debug(RootEngine::OBB* obb, const glm::mat4x4& p_space, const glm::vec3& p_color);

	void* m_engineModule;

	ECS::World m_world;
	Treenity m_treenityEditor;
	EngineActions m_engineActions;	
	ProjectManager m_projectManager;
	GlobalKeys m_globalKeys;

	glm::vec3 debugRay;
	glm::vec3 debugCameraPos;

	RootForce::WorldSystem m_worldSystem;
	RootForce::ShadowSystem* m_shadowSystem;
	RootForce::PointLightSystem* m_pointLightSystem;
	RootForce::DirectionalLightSystem* m_directionalLightSystem;
	RootForce::RenderingSystem* m_renderingSystem;
	RootForce::TransformInterpolationSystem* m_transformInterpolationSystem;
	RootForce::CameraSystem* m_cameraSystem;
	RootForce::ScriptSystem* m_scriptSystem;
	RootForce::ControllerActionSystem* m_controllerActionSystem;
	RootForce::LookAtSystem* m_lookAtSystem;

	Render::Material* m_selectedEntityMaterial;
	std::set<ECS::Entity*> m_pickedEntities;
};