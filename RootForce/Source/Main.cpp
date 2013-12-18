#include <Main.h>
#include <stdexcept>
#include <exception>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

//#include <RenderingSystem.h>
//#include <LightSystem.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>

#include <RootForce/Include/LuaAPI.h>
#include <RootForce/Include/RawMeshPrimitives.h>
#include <glm/glm.hpp>

#include <RootSystems/Include/Network/Messages.h>

#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>
#include <RootSystems/Include/Components.h>

#include "Menu.h"

#undef main

int main(int argc, char* argv[]) 
{
	RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
	RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
	RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
	RootForce::Player::SetTypeId(RootForce::ComponentType::PLAYER);
	RootForce::PlayerControl::SetTypeId(RootForce::ComponentType::PLAYERCONTROL);
	RootForce::PhysicsAccessor::SetTypeId(RootForce::ComponentType::PHYSICS);
	RootForce::Network::NetworkClientComponent::SetTypeId(RootForce::ComponentType::NETWORKCLIENT);
	RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
	RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
	RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
	RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
	RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);

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

		m_currentState = GameState::Menu;
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

		//Bind c++ functions and members to Lua
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::entity_f, RootForce::LuaAPI::entity_m, "Entity");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::renderable_f, RootForce::LuaAPI::renderable_m, "Renderable");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::transformation_f, RootForce::LuaAPI::transformation_m, "Transformation");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::physicsaccessor_f, RootForce::LuaAPI::physicsaccessor_m, "PhysicsAccessor");
		
		g_engineContext.m_resourceManager->LoadScript("AbilityTest");

		g_world = &m_world;

		// Initialize the system for controlling the player.
		std::vector<RootForce::Keybinding> keybindings(5);
		keybindings[0].Bindings.push_back(SDL_SCANCODE_UP);
		keybindings[0].Bindings.push_back(SDL_SCANCODE_W);
		keybindings[0].Action = RootForce::PlayerAction::MOVE_FORWARDS;

		keybindings[1].Bindings.push_back(SDL_SCANCODE_DOWN);
		keybindings[1].Bindings.push_back(SDL_SCANCODE_S);
		keybindings[1].Action = RootForce::PlayerAction::MOVE_BACKWARDS;

		keybindings[2].Bindings.push_back(SDL_SCANCODE_LEFT);
		keybindings[2].Bindings.push_back(SDL_SCANCODE_A);
		keybindings[2].Action = RootForce::PlayerAction::STRAFE_LEFT;

		keybindings[3].Bindings.push_back(SDL_SCANCODE_RIGHT);
		keybindings[3].Bindings.push_back(SDL_SCANCODE_D);
		keybindings[3].Action = RootForce::PlayerAction::STRAFE_RIGHT;

		keybindings[4].Bindings.push_back(SDL_SCANCODE_SPACE);
		keybindings[4].Action = RootForce::PlayerAction::ACTIVATE_ABILITY;
		keybindings[4].Edge = true;

		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(&m_world));
		m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
		m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_playerControlSystem->SetKeybindings(keybindings);
		m_playerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

		m_playerSystem = std::shared_ptr<RootForce::PlayerSystem>(new RootForce::PlayerSystem(&m_world, &g_engineContext));

		RootForce::ScriptSystem* scriptSystem = new RootForce::ScriptSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::ScriptSystem>(scriptSystem, "ScriptSystem");

		// Initialize physics system
		RootForce::PhysicsSystem* m_physicsSystem = new RootForce::PhysicsSystem(&m_world);
		m_physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
		m_physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_world.GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem, "PhysicsSystem");

		m_world.GetEntityImporter()->SetImporter(Importer);
		m_world.GetEntityExporter()->SetExporter(Exporter);

		// Initialize render and point light system.
		RootForce::RenderingSystem* renderingSystem = new RootForce::RenderingSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

		renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
		renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

		RootForce::CameraSystem* cameraSystem = new RootForce::CameraSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(cameraSystem, "CameraSystem");
		RootForce::LookAtSystem* lookAtSystem = new RootForce::LookAtSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::LookAtSystem>(lookAtSystem, "LookAtSystem");
		RootForce::ThirdPersonBehaviorSystem* thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(thirdPersonBehaviorSystem, "ThirdPersonBehaviorSystem");

		g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

		Render::DirectionalLight dl;
		dl.m_color = glm::vec4(1.0f,1.f,1.f,1);
		dl.m_direction = glm::vec3(0,0,-1);

		g_engineContext.m_renderer->AddDirectionalLight(dl, 0);

		RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
		m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

		RootForce::AbilitySystem* abilitySystem = new RootForce::AbilitySystem(&m_world, g_engineContext.m_renderer);
		m_world.GetSystemManager()->AddSystem<RootForce::AbilitySystem>(abilitySystem, "AbilitySystem");


		// Import test world.
		m_world.GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");
		
		m_playerSystem->CreatePlayer();

		ECS::EntityManager* em = m_world.GetEntityManager();


		//Create camera
		ECS::Entity* cameraEntity = m_world.GetEntityManager()->CreateEntity();
		m_world.GetTagManager()->RegisterEntity("Camera", cameraEntity);
		RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
		camera->m_near = 0.1f;
		camera->m_far = 1000.0f;
		camera->m_fov = 45.0f;
		RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);
		RootForce::LookAtBehavior* cameraLookAt = m_world.GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
		cameraLookAt->m_targetTag = "AimingDevice";
		cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
		RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world.GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
		cameraThirdPerson->m_targetTag = "AimingDevice";
		cameraThirdPerson->m_displacement = glm::vec3(0.0f, 4.0f, -8.0f);


		//Initiate GUI
		g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));

		g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->LoadURL("debug.html"));

		//Plane at bottom

		glm::vec3 normal (0,1,0);
		glm::vec3 position (0, -2, 0);
	
		//g_engineContext.m_physics->CreatePlane(normal, position);
		normal = glm::vec3 (0,0,-1);
		position = glm::vec3 (0,0,9);
	//	g_engineContext.m_physics->CreatePlane(normal, position);
		// Setup the skybox.
		auto e = m_world.GetTagManager()->GetEntityByTag("Skybox");
		auto r = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(e);
		r->m_material.m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(
			"SkyBox", Render::TextureType::TEXTURE_CUBEMAP);

		//Initiate Gui
        g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
                g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));
        
        g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->LoadURL("debug.html"));
        m_chat.Initialize(g_engineContext.m_gui->LoadURL("hud.html"), g_engineContext.m_gui->GetDispatcher());

		//Initiate Menu
		Menu* m_menu = new Menu(g_engineContext.m_gui->LoadURL("menu.html"), g_engineContext.m_gui->GetDispatcher());

		m_displayPhysicsDebug = false;

		uint64_t old;
		while(m_running)
			switch (m_currentState)
			{
			case RootForce::GameState::Menu:
				m_menu->Unhide();
				g_engineContext.m_inputSys->LockMouseToCenter(false);
				old = SDL_GetPerformanceCounter();

				// Initialize a client (defer the initialization of the server until we know we need one)
				m_client = std::shared_ptr<RootForce::Network::Client>(new RootForce::Network::Client(g_engineContext.m_logger));
				m_clientMessageHandler = std::shared_ptr<RootForce::Network::ClientMessageHandler>(new RootForce::Network::ClientMessageHandler(m_client->GetPeerInterface(), g_engineContext.m_logger));
				m_clientMessageHandler->SetLanList(&m_lanList);
				m_clientMessageHandler->SetChatSystem(&m_chat);
				m_client->SetMessageHandler(m_clientMessageHandler.get());
				m_client->SetChatSystem(&m_chat);

				while (m_currentState == RootForce::GameState::Menu && m_running)
				{
					HandleEvents();

					m_client->Update();

					g_engineContext.m_renderer->Clear();

					g_engineContext.m_gui->Update();
					g_engineContext.m_gui->Render();

					g_engineContext.m_renderer->Swap();

					std::vector<std::pair<uint64_t, RootSystems::ServerInfoInternal>> lanList = m_lanList.GetList();
					for(int i = 0; i < lanList.size(); i++)
						m_menu->AddServer(lanList.at(i));

					MenuEvent::MenuEvent event = m_menu->PollEvent();

					switch (event.type)
					{
					case MenuEvent::EventType::Exit:
						m_running = false;
						break;
					case MenuEvent::EventType::Host:
						m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(g_engineContext.m_logger, "Local Server", event.data[0].ToInteger(), 12));
						m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_server->GetPeerInterface(), g_engineContext.m_logger));
						m_server->SetMessageHandler(m_serverMessageHandler.get());
						m_client->Connect("127.0.0.1", event.data[0].ToInteger());
						m_currentState = RootForce::GameState::Ingame;

						//m_networkHandler->SetChatSystem(&m_chat);
						//m_networkHandler->Host(event.data[0].ToInteger(), Network::MessageHandler::ServerType::LOCAL); //TODO: make sure Host returns boolean and only change gamestate if true
						//m_networkHandler->GetClientMessageHandler()->SetPlayerSystem(m_playerSystem.get());
						//m_networkHandler->GetClientMessageHandler()->SetChatSystem(&m_chat);
						break;
					case MenuEvent::EventType::Connect:
						m_client->Connect(Awesomium::ToString(event.data[1].ToString()).c_str(), event.data[0].ToInteger()); 
						m_currentState = RootForce::GameState::Ingame;

						//m_networkHandler->SetChatSystem(&m_chat);
						//m_networkHandler->Connect(event.data[0].ToInteger(), Awesomium::ToString(event.data[1].ToString()).c_str()); //TODO: make sure Connect returns boolean and only change gamestate if true
						//m_networkHandler->GetClientMessageHandler()->SetPlayerSystem(m_playerSystem.get());
						//m_networkHandler->GetClientMessageHandler()->SetChatSystem(&m_chat);
						
						break;
					case MenuEvent::EventType::Refresh:
						m_client->PingNetwork(&m_lanList, 5567);
						break;
					default:
						break;
					}
				}
				m_menu->Hide();
				break;
			case RootForce::GameState::Ingame:
				// Start the main loop
				
				g_engineContext.m_inputSys->LockMouseToCenter(true);
				old = SDL_GetPerformanceCounter();
				while (m_currentState == RootForce::GameState::Ingame && m_running)
				{	
					uint64_t now = SDL_GetPerformanceCounter();
					float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
					old = now;

					m_world.SetDelta(dt);

					g_engineContext.m_renderer->Clear();

					if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_ESCAPE) == RootEngine::InputManager::KeyState::DOWN_EDGE)
					{
						m_running = false;
					}

					// Toggle rendering of normals.
					if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F12) == RootEngine::InputManager::KeyState::DOWN_EDGE)
					{
						if(m_displayNormals)
						{
							m_displayNormals = false;
							g_engineContext.m_renderer->DisplayNormals(m_displayNormals);
						}
						else
						{
							m_displayNormals = true;
							g_engineContext.m_renderer->DisplayNormals(m_displayNormals);
						}
					}

					// Toggle physics debug draw
					if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F11) == RootEngine::InputManager::KeyState::DOWN_EDGE)
					{
						if(m_displayPhysicsDebug)
						{
							m_displayPhysicsDebug = false;
							g_engineContext.m_physics->EnableDebugDraw(m_displayPhysicsDebug);
						}
						else
						{
							m_displayPhysicsDebug = true;
							g_engineContext.m_physics->EnableDebugDraw(m_displayPhysicsDebug);
						}
					}

					g_engineContext.m_debugOverlay->AddHTMLToBuffer(std::to_string(dt).c_str(), RootEngine::TextColor::GRAY, false);

					{
						PROFILE("Handle Events", g_engineContext.m_profiler);
						HandleEvents();
					}

					{
						PROFILE("Player control system", g_engineContext.m_profiler);
						m_playerControlSystem->Process();
					}

					{
						PROFILE("Script system", g_engineContext.m_profiler);
						scriptSystem->Process();
					}

					{
						PROFILE("Ability system", g_engineContext.m_profiler);
						abilitySystem->Process();
					}

					if (m_server != nullptr)
					{
						PROFILE("Server", g_engineContext.m_profiler);
						m_server->Update();
					}

					{
						PROFILE("Client", g_engineContext.m_profiler);
						m_client->Update();
					}

					{
						PROFILE("Physics", g_engineContext.m_profiler);
						g_engineContext.m_physics->Update(dt);
						m_physicsSystem->Process();

					}
					m_playerControlSystem->UpdateAimingDevice();
					thirdPersonBehaviorSystem->Process();
					lookAtSystem->Process();
					cameraSystem->Process();
					pointLightSystem->Process();
					{
						PROFILE("RenderingSystem", g_engineContext.m_profiler);
						renderingSystem->Process();
					}
					g_engineContext.m_renderer->Render();

					{
						PROFILE("Render Lines", g_engineContext.m_profiler);
						g_engineContext.m_renderer->RenderLines();
					}


					g_engineContext.m_profiler->Update(dt);


					{
						PROFILE("GUI", g_engineContext.m_profiler);

						g_engineContext.m_gui->Update();
						g_engineContext.m_gui->Render();
					}
					g_engineContext.m_debugOverlay->RenderOverlay();
					g_engineContext.m_renderer->Swap();
				}
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

