#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootForce\Include\PlayerControlSystem.h>
#include <string>

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
	RootEngine::GameSharedContext m_engineContext;
	ECS::World m_world;
	std::shared_ptr<RootForce::PlayerControlSystem> m_playerControlSystem;
};