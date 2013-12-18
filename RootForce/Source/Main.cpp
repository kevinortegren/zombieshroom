#include <Main.h>
#include <stdexcept>
#include <exception>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

//#include <RenderingSystem.h>
//#include <LightSystem.h>

#include <RootForce/Include/LuaAPI.h>
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
		m_menustate = std::shared_ptr<Menustate>(new Menustate());
		m_gamestate = std::shared_ptr<Ingamestate>(new Ingamestate());
		m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

		INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

		g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, p_workingDirectory);

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		{
			// TODO: Log error and throw exception (?)
		}
		// TODO: Make these parameters more configurable.
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
			// TODO: Log error and throw exception (?)
		}

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

		
		g_engineContext.m_resourceManager->LoadScript("AbilityTest");

		g_world = &m_world;

		g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

		Render::DirectionalLight dl;
		dl.m_color = glm::vec4(1.0f,1.f,1.f,1);
		dl.m_direction = glm::vec3(0,0,-1);

		g_engineContext.m_renderer->AddDirectionalLight(dl, 0);

        // Initialize the network system
        m_networkHandler = std::shared_ptr<RootForce::Network::MessageHandler>(new RootForce::Network::MessageHandler(&m_world, g_engineContext.m_logger, g_engineContext.m_network));

		m_menustate->Initialize(&g_engineContext, m_networkHandler);
		

		uint64_t old;
		while(m_running)
			switch (m_currentState)
			{
			case RootForce::GameStates::Menu:
				g_engineContext.m_inputSys->LockMouseToCenter(false);
				 old = SDL_GetPerformanceCounter();
				while (m_currentState == RootForce::GameStates::Menu && m_running)
				{
					HandleEvents();
					m_currentState = m_menustate->Update();
					m_menustate->Render();

				}
				break;
			case RootForce::GameStates::Ingame:
				// Start the main loop
				m_gamestate->Initialize(&g_engineContext, m_networkHandler, std::shared_ptr<ECS::World>(&m_world), m_menustate->GetPlayData());
				g_engineContext.m_inputSys->LockMouseToCenter(true);
				old = SDL_GetPerformanceCounter();
				while (m_currentState == RootForce::GameStates::Ingame && m_running)
				{	
					HandleEvents();

					if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_ESCAPE) == RootEngine::InputManager::KeyState::DOWN_EDGE)
					{
						m_running = false;
					}

					uint64_t now = SDL_GetPerformanceCounter();
					float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
					old = now;
					m_gamestate->Update(dt);
					m_gamestate->Render();
				}
				break;
			case RootForce::GameStates::Exit:
				m_running = false;
				break;
			default:
				break;
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

