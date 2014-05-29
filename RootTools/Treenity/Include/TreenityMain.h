#pragma once

// Root Systems
#include <RootSystems/Include/Components.h>
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/ShadowSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/ControllerActionSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/ScriptSystem.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootSystems/Include/RagdollSystem.h>
#include <RootSystems/Include/MatchStateSystem.h>
#include <RootSystems/Include/ActionSystem.h>
#include <RootSystems/Include/RespawnSystem.h>
#include <RootSystems/Include/StateSystem.h>
#include <RootSystems/Include/WaterSystem.h>
#include <RootSystems/Include/AbilitySpawnSystem.h>
#include <RootSystems/Include/TryPickupResetSystem.h>
#include <RootSystems/Include/SoundSystem.h>
#include <RootSystems/Include/FollowSystem.h>
#include <RootSystems/Include/BotanySystem.h>
#include <RootSystems/Include/HomingSystem.h>
#include <RootSystems/Include/RaySystem.h>
#include <RootSystems/Include/TransformInterpolationSystem.h>
#include <RootSystems/Include/Network/NetworkDebugSystem.h>
#include <RootSystems/Include/WaterDeathSystem.h>
#include <RootSystems/Include/ScaleSystem.h>
#include <RootSystems/Include/DeserializationSystem.h>

// Treenity
#include <RootTools/Treenity/Include/EngineActions.h>
#include <RootTools/Treenity/Include/Treenity.h>
#include <RootTools/Treenity/Include/ProjectManager.h>
#include <RootTools/Treenity/Include/GlobalKeys.h>

class TreenityMain
{
public:
	TreenityMain();
	~TreenityMain();

	void HandleEditorEvents();
	void HandleIngameEvents();
	void ProcessWorldMessages();
	void HandleAltModifier();
	
	void Update(float dt);
	bool IsRunning();

	ProjectManager* GetProjectManager() { return &m_projectManager; }
	Treenity* GetEditor() { return &m_treenityEditor; }
	RootForce::WorldSystem* GetWorldSystem() { return m_worldSystem.get(); }
	RootForce::ShadowSystem* GetShadowSystem() { return m_shadowSystem; }
	RootForce::AnimationSystem* GetAnimationSystem() { return m_animationSystem; }
	RootForce::WaterSystem*		GetWaterSystem() { return m_waterSystem; }
private:

	void UpdateTools();
	void RenderSelectedEntity();
	void SelectPick(const glm::vec3& p_cameraPos, const glm::vec3& p_ray);

	glm::vec3 ConstructRay();
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

	std::shared_ptr<RootForce::MatchStateSystem>		m_matchStateSystem;
	std::shared_ptr<RootForce::PlayerControlSystem>		m_playerControlSystem;
	std::shared_ptr<RootForce::WorldSystem>				m_worldSystem;
	std::shared_ptr<RootForce::DeserializationSystem>	m_deserializationSystem;
	RootForce::AbilitySpawnSystem*						m_abilitySpawnSystem;
	RootSystems::RespawnSystem*							m_respawnSystem;
	RootForce::ControllerActionSystem*					m_controllerActionSystem;
	RootForce::PhysicsTransformCorrectionSystem*		m_physicsTransformCorrectionSystem;
	RootForce::PhysicsSystem*							m_physicsSystem;
	RootForce::ScriptSystem*							m_scriptSystem;
	RootForce::CollisionSystem*							m_collisionSystem;
	RootForce::ShadowSystem*							m_shadowSystem;
	RootForce::RenderingSystem*							m_renderingSystem;
	RootForce::PointLightSystem*						m_pointLightSystem;
	RootForce::ParticleSystem*							m_particleSystem;
	RootForce::CameraSystem*							m_cameraSystem;
	RootForce::LookAtSystem*							m_lookAtSystem;
	RootForce::ThirdPersonBehaviorSystem*				m_thirdPersonBehaviorSystem;
	RootForce::AnimationSystem*							m_animationSystem;
	RootForce::RagdollSystem*							m_ragdollSystem;
	RootForce::WaterSystem*								m_waterSystem;
	RootSystems::ActionSystem*							m_actionSystem;
	RootSystems::StateSystem*							m_stateSystem;
	RootForce::DirectionalLightSystem* 					m_directionalLightSystem;
	RootForce::TryPickupResetSystem* 					m_tryPickupResetSystem;
	RootForce::SoundSystem*								m_soundSystem;
	RootForce::BotanySystem*							m_botanySystem;
	RootForce::TimerSystem*								m_timerSystem;
	RootForce::FollowSystem*							m_followSystem;
	RootForce::HomingSystem*							m_homingSystem;
	RootForce::RaySystem*								m_raySystem;
	RootForce::TransformInterpolationSystem*			m_transformInterpolationSystem;
	RootForce::Network::NetworkDebugSystem*				m_networkDebugSystem;
	RootForce::WaterDeathSystem*						m_waterDeathSystem;
	RootForce::ScaleSystem*								m_scaleSystem;

	Render::Material* m_selectedEntityMaterial;
	std::set<ECS::Entity*> m_pickedEntities;
};