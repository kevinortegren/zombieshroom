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

void GameLogicSystem::Init()
{
	// Init will fill the map with entity data.
	//m_players.Init(m_world->GetEntityManager());
	//m_transforms.Init(m_world->GetEntityManager());

	m_world->GetEntityManager()->GetComponentList(0);
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

	if (createContextFunc == nullptr)
	{
		std::cout << "Failed to load engine function: " << DynamicLoader::GetLastError() << std::endl;
		throw std::runtime_error("");
	}

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

	
	//ECS::ComponentSystem* gameLogic = m_engineContext->GetWorld()->GetSystemManager()->CreateSystem<GameLogicSystem>("GameLogic");

	//Engine::Renderer::GLRenderer::GetInstance()->SetupSDLContext(m_window.get());

	// CreateSystem allocates and stores a system with a string handler.

	// CreateEntity allocates and stores a entity.
	/*ECS::Entity* rolf = m_engineContext->GetWorld()->GetEntityManager()->CreateEntity();

	// CreateComponent allocates and stores a specified component belonging to a entity.
	Player* playerData = m_engineContext->GetWorld()->GetEntityManager()->CreateComponent<Player>(rolf);
	playerData->m_health = 10.0f;
	playerData->m_name = "Rolf";

	Transform* transformData = m_engineContext->GetWorld()->GetEntityManager()->CreateComponent<Transform>(rolf);
	transformData->m_x = 0.0f;
	transformData->m_y = -5.0f;

	// Initialize system sets up all the system for processing.
	m_engineContext->GetWorld()->GetSystemManager()->InitializeSystems();

	// Process will execute the logic flow.
	gameLogic->Process();

	// Processing by requesting the system from the system manager.
	m_engineContext->GetWorld()->GetSystemManager()->GetSystem<GameLogicSystem>("GameLogic")->Process();*/
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
