#include <Main.h>
#include <stdexcept>

#include "RootEngine/Include/Logging/Logging.h"
#include "RootEngine/Render/Include/Renderer.h"

#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>

#undef main

TEST(Test, Foo) 
{
	int a = 0;
	EXPECT_TRUE(a == 0);
}

int main(int argc, char* argv[]) 
{
	try 
	{
		if (argc > 1 && strcmp(argv[1], "-test") == 0)
		{
			testing::InitGoogleTest(&argc, argv);

			int result = RUN_ALL_TESTS();
			std::cin.get();
			return result;
		}
		else
		{
			Main m;
			m.Start();
		}
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		std::cin.get();
		return 1;
	}
	
	return 0;
}

Main::Main() 
	: m_running(true)
{

	void* handle = DynamicLoader::LoadSharedLibrary("RootEngine.dll");
	if (handle == nullptr)
	{
		std::cerr << "Failed to load engine: " << DynamicLoader::GetLastError() << std::endl;
		throw std::runtime_error("");
	}

	typedef RootEngine::ContextInterface* ( *CREATECONTEXT )(int);
	CREATECONTEXT createContextFunc = (CREATECONTEXT)DynamicLoader::LoadProcess(handle, "CreateContext");

	m_engineContext = createContextFunc(RootEngine::SubsystemInit::INIT_NETWORK | RootEngine::SubsystemInit::INIT_RENDER);
	if (createContextFunc == nullptr)
	{
		std::cout << "Failed to load engine function: " << DynamicLoader::GetLastError() << std::endl;
		throw std::runtime_error("");
	}

	if(m_engineContext != nullptr)
	{


	}
	else
	{
		std::cout << "Couldn't call create context." << std::endl;
	}

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		// TODO: Log error and throw exception (?)
	}

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

	m_engineContext->GetRenderer()->SetupSDLContext(m_window.get());
}

Main::~Main() 
{
	SDL_Quit();
}

void Main::Start() 
{

	//Open the log file stream for this instance(Do this once at the beginning of the program)
	//Logging::GetInstance()->OpenLogStream();

	//Include Logging.h in the file you want to use the logging function
	//Write a string to the log file stream(Do this when you want to log something...)
	//Logging::GetInstance()->LogTextToFile("Log %f entry test ", 1.435);
	
	//Write a log string to console
	//Logging::GetInstance()->LogTextToConsole("Console entry test %d", 12);


	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		// TODO: Poll and handle events
		// TODO: Update game state
		// TODO: Render and present game
		HandleEvents();
	}
}

void Main::HandleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type) 
		{
		case SDL_QUIT:
			m_running = false;
			break;

		case SDL_KEYDOWN:
			break;

		case SDL_KEYUP:
			break;

		default:
			break;
		}
	}
}
