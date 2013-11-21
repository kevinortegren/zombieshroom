#include <Main.h>
#include <exception>

#include "RootEngine/Include/Logging/Logging.h"
#include "RootEngine/Render/Include/Renderer.h"

#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>

#include <RootEngine/Include/RootEngine.h>

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

	void* handle = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	typedef RootEngine::ContextInterface* ( *CREATECONTEXT )(int);
	CREATECONTEXT createContextFunc = (CREATECONTEXT)DynamicLoader::LoadProcess(handle, "CreateContext");

	m_engineContext = createContextFunc(RootEngine::SubsystemInit::INIT_NETWORK);
	if(m_engineContext != nullptr)
	{


	}
	else
	{
		std::cout << "Couldnt call create context." << std::endl;
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

	/*Engine::Renderer::GLRenderer::GetInstance()->SetupSDLContext(m_window.get());

	// CreateSystem allocates and stores a system with a string handler.
	std::shared_ptr<ECS::ComponentSystem> gameLogic = m_engineContext.m_world->GetSystemManager()->CreateSystem<GameLogicSystem>("GameLogic");

	// CreateEntity allocates and stores a entity.
	std::shared_ptr<ECS::Entity> rolf = m_engineContext.m_world->GetEntityManager()->CreateEntity();

	// CreateComponent allocates and stores a specified component belonging to a entity.
	std::shared_ptr<Player> playerData = m_engineContext.m_world->GetEntityManager()->CreateComponent<Player>(rolf);
	playerData->m_health = 10.0f;
	playerData->m_name = "Rolf";

	std::shared_ptr<Transform> transformData = m_engineContext.m_world->GetEntityManager()->CreateComponent<Transform>(rolf);
	transformData->m_x = 0.0f;
	transformData->m_y = -5.0f;

	// Initialize system sets up all the system for processing.
	m_engineContext.m_world->GetSystemManager()->InitializeSystems();

	// Process will execute the logic flow.
	gameLogic->Process();

	// Processing by requesting the system from the system manager.
	m_engineContext.m_world->GetSystemManager()->GetSystem<GameLogicSystem>("GameLogic")->Process();*/

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
