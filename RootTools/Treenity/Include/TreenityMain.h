#pragma once
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>

#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/TransformInterpolationSystem.h>
#include <RootSystems/Include/ScriptSystem.h>
#include <RootSystems/Include/ControllerActionSystem.h>
#include <RootSystems/Include/ShadowSystem.h>
#include <RootSystems/Include/LightSystem.h>

#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

#include <RootTools/Treenity/Include/EngineActions.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;
RootForce::Network::NetworkEntityMap g_networkEntityMap;
RootForce::Network::DeletedNetworkEntityList g_networkDeletedList;

class TreenityMain
{
public:
	TreenityMain(const std::string& p_path);
	~TreenityMain();

	void HandleEvents();
	void HandleAltModifier();
	
	void Update(float dt);
	bool IsRunning();

	void CreateFreeFlyingCamera();

private:
	void* m_engineModule;

	ECS::World m_world;
	Treenity m_treenityEditor;
	EngineActions m_engineActions;	
	ProjectManager m_projectManager;

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

	ECS::Entity* m_cameraEntity;
	ECS::Entity* m_aimingDevice;

	bool m_altMode;
};