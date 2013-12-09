#pragma once

#include <WinSock2.h>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/AbilitySystem.h>
#include <RootSystems/Include/Network/MessageHandler.h>
#include <memory>
#include <string>

RootEngine::GameSharedContext g_engineContext;

class Main {
public:
	Main(std::string p_workingDirectory);
	~Main();

	void Start();
private:

	void HandleEvents();
	void testfuncofawesome(std::vector<glm::vec3> p_debugVectors);
	bool m_running;
	void* m_engineModule;
	std::shared_ptr<SDL_Window> m_window;
	
	ECS::World m_world;
	std::shared_ptr<RootForce::PlayerControlSystem> m_playerControlSystem;
	std::shared_ptr<RootForce::Network::MessageHandler> m_networkHandler;
	bool m_displayNormals;
};