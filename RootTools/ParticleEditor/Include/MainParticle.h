#pragma once
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include "ParticleEditor.h"
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/ParticleSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/Components.h>
#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>
#include <RootEngine/Render/Include/Texture.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;

class MainParticle
{
public:
	MainParticle(std::string p_workingDirectory, ParticleEditor* p_particleEdtiorQt);
	~MainParticle();
	void HandleEvents();
	void Update(float p_delta);
private:
	void UpdateInput();
	void UpdateThirdPerson();
	void CreateSkyBox();

	bool m_running;
	std::string m_workingDirectory;
	void* m_engineModule;
	std::shared_ptr<SDL_Window> m_window;

	ECS::World m_world;

	RootForce::RenderingSystem*				m_renderingSystem;
	RootForce::PointLightSystem*			m_pointLightSystem;
	RootForce::ParticleSystem*				m_particleSystem;
	RootForce::CameraSystem*				m_cameraSystem;
	RootForce::LookAtSystem*				m_lookAtSystem;
	ParticleEditor*							m_particleEditorQt;

	ECS::Entity*							m_cameraEntity;
	ECS::Entity*							m_aimingDevice;

	bool m_focusInterpolation;
	float m_focusIntTime;
	glm::vec3 m_toPosition;
	glm::vec3 m_fromPosition;


};


