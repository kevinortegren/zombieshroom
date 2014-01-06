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


		// Setup the screen states
		m_menuState = std::shared_ptr<MenuState>(new MenuState(m_client, m_server));
		m_connectingState = std::shared_ptr<ConnectingState>(new ConnectingState(m_client, m_server));
		m_ingameState = std::shared_ptr<IngameState>(new IngameState(m_client, m_server));

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
		g_engineContext.m_renderer->SetupSDLContext(m_window.get());

		g_world = &m_world;
        m_world.GetEntityImporter()->SetImporter(Importer);
		m_world.GetEntityExporter()->SetExporter(Exporter);

		g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

		Render::DirectionalLight dl;
		dl.m_color = glm::vec4(1.0f,1.f,1.f,1);
		dl.m_direction = glm::vec3(0,0,-1);

		g_engineContext.m_renderer->AddDirectionalLight(dl, 0);


		// Initialize GUI
		g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));


		// Setup a client (but don't connect until we have connection details)
		m_client = std::shared_ptr<RootForce::Network::Client>(new RootForce::Network::Client(g_engineContext.m_logger));

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
					m_connectingState->Enter();

					while (m_currentState == RootForce::GameStates::Connecting && m_running)
					{
						HandleEvents();
						m_currentState = m_connectingState->Update();
					}

					m_connectingState->Exit();
				} break;

				case RootForce::GameStates::Ingame:
				{
					

					/*
					// Start the main loop
					//m_gamestate->Initialize(&g_engineContext, &m_world, m_menustate->GetPlayData(), m_client.get(), m_clientMessageHandler.get());
					g_engineContext.m_inputSys->LockMouseToCenter(true);
                
					uint64_t old = SDL_GetPerformanceCounter();
					while (m_currentState == RootForce::GameStates::Ingame && m_running)
					{	
						uint64_t now = SDL_GetPerformanceCounter();
						float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
						old = now;

                        HandleEvents();

						if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_ESCAPE) == RootEngine::InputManager::KeyState::DOWN_EDGE)
						{
							m_running = false;
						}
                    
						m_gamestate->Update(dt);
					}
					*/
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

