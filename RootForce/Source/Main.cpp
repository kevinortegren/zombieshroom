#include <Main.h>
#include <exception>

#undef main

int main(int argc, char* argv[]) 
{
	try 
	{
		Main m;
		m.Start();
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		return 1;
	}
	
	return 0;
}



Main::Main() 
	: m_running(true) 
{

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		// TODO: Log error and throw exception (?)
	}

	// TODO: Set OpenGL attributes? This needs to be done before a window is opened, but responsibility suits the engine/renderer better.

	// TODO: Make these parameters more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			"Root Force",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1280,
			720,
			SDL_WINDOW_OPENGL),
		SDL_DestroyWindow);
	if (m_window == nullptr) 
	{
		// TODO: Log error and throw exception (?)
	}
}

Main::~Main() 
{
	SDL_Quit();
}

void Main::Start() 
{
	while (m_running)
	{
		// TODO: Poll and handle events
		// TODO: Update game state
		// TODO: Render and present game
		SDL_Delay(2000);
		m_running = false;
	}
}