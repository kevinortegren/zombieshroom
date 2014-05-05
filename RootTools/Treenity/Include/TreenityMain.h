#pragma once
#include <Utility/ECS/Include/World.h>
#include <RootTools/Treenity/Include/EngineActions.h>

#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/TransformInterpolationSystem.h>

#include <RootEngine/Include/GameSharedContext.h>
RootEngine::GameSharedContext g_engineContext;

class TreenityMain
{
public:
	TreenityMain(const std::string& p_path);
	~TreenityMain();

	void HandleEvents();
	void Update(float dt);
	bool IsRunning();

	void CreateSkyBox();

private:
	void* m_engineModule;

	ECS::World m_world;
	Treenity m_treenityEditor;
	EngineActions m_engineActions;	
	ProjectManager m_projectManager;

	RootForce::RenderingSystem* m_renderingSystem;
	RootForce::CameraSystem* m_cameraSystem;
	RootForce::TransformInterpolationSystem* m_transformInterpolationSystem;

	ECS::Entity* m_skyBox;
	ECS::Entity* m_cameraEntity;
};