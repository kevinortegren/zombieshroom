#pragma once

#include <memory>
#include <SDL2/SDL.h>

class Main {
public:
	Main();
	~Main();

	void Start();
private:

	void HandleEvents();

	bool m_running;
	std::shared_ptr<SDL_Window> m_window;
};