#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include <ECS/ECS.h>

enum ComponentType
{
	TYPE_TEST
};

struct TestComponent : public ECS::ComponentInterface
{
	float data;
};

class Main {
public:
	Main();
	~Main();

	void Start();
private:

	void HandleEvents();

	bool m_running;
	std::shared_ptr<SDL_Window> m_window;

	ECS::World world;
};