#include <Main.h>
#include <stdexcept>
#include <exception>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <RootForce/Include/RawMeshPrimitives.h>
#include <glm/glm.hpp>

#include <RootSystems/Include/Network/Messages.h>

#undef main

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Rootforce.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		RootForce::Main m(path);
		m.Start();
	} 
	catch (std::exception& e) 
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		std::cout << "Unknown exception" << std::endl;
		std::cin.get();
		return 1;
	}
	
	return 0;
}

namespace RootForce
{
	Main::Main(std::string p_workingDirectory) 
		: m_running(true)
	{
		m_workingDirectory = p_workingDirectory;
		g_world = &m_world;

		// Load the engine
		m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

		if (m_engineModule == nullptr)
		{
			throw std::runtime_error("Failed to load RootEngine - please check your installation");
		}

		INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");
		
		if (libInitializeEngine == nullptr)
		{
			throw std::runtime_error("Failed to load RootEngine - please check your installation");
		}

		g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, p_workingDirectory);

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		{
			throw std::runtime_error("Failed to initialize SDL");
		}


		// TODO: Make these parameters (even?) more configurable.
		m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
				"Root Force",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
				g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"),
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
			SDL_DestroyWindow);
		if (m_window == nullptr) 
		{
			throw std::runtime_error("Failed to create window");
		}

		// Setup the SDL context
		g_engineContext.m_renderer->SetupSDLContext(m_window.get());

		// Setup the importer and exporter
		m_world.GetEntityImporter()->SetImporter(Importer);
		m_world.GetEntityExporter()->SetExporter(Exporter);

		// Initialize GUI
		g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));

		// Initialize shared systems
		m_sharedSystems.m_playerSystem = std::shared_ptr<RootForce::PlayerSystem>(new RootForce::PlayerSystem(g_world, &g_engineContext));
		m_sharedSystems.m_matchStateSystem = std::shared_ptr<RootForce::MatchStateSystem>(new RootForce::MatchStateSystem(g_world, &g_engineContext));

		m_menuState = std::shared_ptr<MenuState>(new MenuState(m_networkContext));
		m_connectingState = std::shared_ptr<ConnectingState>(new ConnectingState(m_networkContext, m_sharedSystems));
		m_ingameState = std::shared_ptr<IngameState>(new IngameState(m_networkContext, m_sharedSystems));

		m_menuState->Initialize(m_workingDirectory);
		m_connectingState->Initialize();
		m_ingameState->Initialize();

		m_currentState = GameStates::Menu;
	}

	Main::~Main() 
	{
		//m_world.GetEntityExporter()->Export(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");
		SDL_Quit();
		DynamicLoader::FreeSharedLibrary(m_engineModule);
	}

	void Main::Start() 
	{
		// Start the main loop!
		while(m_running)
		{
			switch (m_currentState)
			{
				case RootForce::GameStates::Menu:
				{
					m_menuState->Enter();
					
					while (m_currentState == RootForce::GameStates::Menu && m_running)
					{
						HandleEvents();
						m_currentState = m_menuState->Update();
					}

					m_menuState->Exit();
				} break;

				case RootForce::GameStates::Connecting:
				{
					m_connectingState->Enter(m_menuState->GetPlayData());

					while (m_currentState == RootForce::GameStates::Connecting && m_running)
					{
						HandleEvents();
						m_currentState = m_connectingState->Update();
					}

					m_connectingState->Exit();
				} break;

				case RootForce::GameStates::Ingame:
				{
					m_ingameState->Enter();

					uint64_t old = SDL_GetPerformanceCounter();
					while (m_currentState == RootForce::GameStates::Ingame && m_running)
					{
						uint64_t now = SDL_GetPerformanceCounter();
						float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
						old = now;

						HandleEvents();
						m_currentState = m_ingameState->Update(dt);
					}

					m_ingameState->Exit();
				} break;

				case RootForce::GameStates::Exit:
				{
					m_running = false;
				} break;
			}
		}
	}

	void Main::HandleEvents()
	{
		if (g_engineContext.m_inputSys != nullptr)
			g_engineContext.m_inputSys->Reset();
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type) 
			{
			case SDL_QUIT: 
				m_running = false;
				break;

			default:
				if (g_engineContext.m_inputSys != nullptr)
					g_engineContext.m_inputSys->HandleInput(event);
				if (g_engineContext.m_gui != nullptr)
					g_engineContext.m_gui->HandleEvents(event);
			}
		}
	}
}

