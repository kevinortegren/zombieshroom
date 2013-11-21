#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <RootEngine/Include/RootEngine.h>
#include <ECS/World.h>

class Main {
public:
	Main();
	~Main();

	void Start();
private:

	void HandleEvents();

	bool m_running;
	std::shared_ptr<SDL_Window> m_window;
	RootEngine::ContextInterface* m_engineContext;
	ECS::World m_world;
};