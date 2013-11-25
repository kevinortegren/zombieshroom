#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <ECS/World.h>

class Main {
public:
	Main();
	~Main();

	void Start();
private:

	void HandleEvents();

	bool m_running;
	void* m_engineModule;
	std::shared_ptr<SDL_Window> m_window;
	RootEngine::GameSharedContext m_engineContext;
	ECS::World m_world;
};