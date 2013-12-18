#include <Main.h>
#include <stdexcept>
#include <exception>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <RootForce/Include/LuaAPI.h>
#include <RootForce/Include/RawMeshPrimitives.h>
#include <glm/glm.hpp>

#include <RootSystems/Include/Network/Messages.h>

#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>
#include <RootSystems/Include/Components.h>

#undef main

int main(int argc, char* argv[]) 
{
	RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
	RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
	RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
	RootForce::Player::SetTypeId(RootForce::ComponentType::PLAYER);
	RootForce::PlayerControl::SetTypeId(RootForce::ComponentType::PLAYERCONTROL);
	RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
	RootForce::Network::NetworkClientComponent::SetTypeId(RootForce::ComponentType::NETWORKCLIENT);
	RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
	RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
	RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
	RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
	RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
	RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
	RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);

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
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::physicsaccessor_f, RootForce::LuaAPI::physicsaccessor_m, "Physics");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::collision_f, RootForce::LuaAPI::collision_m, "Collision");
		
		g_engineContext.m_resourceManager->LoadScript("AbilityTest");

		g_engineContext.m_resourceManager->LoadCollada("AnimationTest");

		g_world = &m_world;

		m_world.GetEntityImporter()->SetImporter(Importer);
		m_world.GetEntityExporter()->SetExporter(Exporter);

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

		// System responsible for controlling the player.
		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(&m_world));
		m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
		m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_playerControlSystem->SetKeybindings(keybindings);
		m_playerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

		// System responsible for updating the world.
		m_worldSystem = std::shared_ptr<RootForce::WorldSystem>(new RootForce::WorldSystem(&m_world));

		// System responsible for updating the player.
		m_playerSystem = std::shared_ptr<RootForce::PlayerSystem>(new RootForce::PlayerSystem(&m_world));

		// System responsible for executing script based on actions.
		RootForce::ScriptSystem* scriptSystem = new RootForce::ScriptSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::ScriptSystem>(scriptSystem, "ScriptSystem");

		// Initialize physics system
		RootForce::PhysicsSystem* m_physicsSystem = new RootForce::PhysicsSystem(&m_world);
		m_physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
		m_physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_world.GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem, "PhysicsSystem");

		RootForce::CollisionSystem* m_collisionSystem = new RootForce::CollisionSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::CollisionSystem>(m_collisionSystem, "CollisionSystem");

		// Initialize render and point light system.
		RootForce::RenderingSystem* renderingSystem = new RootForce::RenderingSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

		renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
		renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

		RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
		m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

		// Initialize camera systems.
		RootForce::CameraSystem* cameraSystem = new RootForce::CameraSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(cameraSystem, "CameraSystem");
		RootForce::LookAtSystem* lookAtSystem = new RootForce::LookAtSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::LookAtSystem>(lookAtSystem, "LookAtSystem");
		RootForce::ThirdPersonBehaviorSystem* thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(&m_world);
		m_world.GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(thirdPersonBehaviorSystem, "ThirdPersonBehaviorSystem");

		m_worldSystem->CreateWorld("level");

		m_playerSystem->CreatePlayer();

		//Plane at bottom
		glm::vec3 normal (0,1,0);
		glm::vec3 position (0, -2, 0);
	
		normal = glm::vec3 (0,0,-1);
		position = glm::vec3 (0,0,9);
	//	g_engineContext.m_physics->CreatePlane(normal, position);
		// Setup the skybox.
		auto e = m_world.GetTagManager()->GetEntityByTag("Skybox");
		auto r = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(e);
		r->m_material = g_engineContext.m_resourceManager->GetMaterial("Skybox"); 
		r->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(
			"SkyBox", Render::TextureType::TEXTURE_CUBEMAP);

		g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));

		g_engineContext.m_gui->LoadURL("debug.html");
		g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->GetView());

		// Initialize the network system
		RootForce::Network::MessageHandler::ServerType serverType = RootForce::Network::MessageHandler::LOCAL;
		m_networkHandler = std::shared_ptr<RootForce::Network::MessageHandler>(new RootForce::Network::MessageHandler(&m_world, g_engineContext.m_logger, g_engineContext.m_network, serverType, 5567, "127.0.0.1"));

		m_displayPhysicsDebug = false;

		// Start the main loop
		uint64_t old = SDL_GetPerformanceCounter();
		while (m_running)
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
			if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F10) == RootEngine::InputManager::KeyState::DOWN_EDGE)
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
				PROFILE("Physics", g_engineContext.m_profiler);
				g_engineContext.m_physics->Update(dt);
				m_physicsSystem->Process();
			}

			{
				PROFILE("Collision system", g_engineContext.m_profiler);
				m_collisionSystem->Process();
			}

			{
				PROFILE("Script system", g_engineContext.m_profiler);
				scriptSystem->Process();
			}

			{
				PROFILE("Network message handler", g_engineContext.m_profiler);
				m_networkHandler->Update();
			}

			{
				PROFILE("Camera systems", g_engineContext.m_profiler);
				m_playerControlSystem->UpdateAimingDevice();
				thirdPersonBehaviorSystem->Process();
				lookAtSystem->Process();
				cameraSystem->Process();
			}

			{
				PROFILE("RenderingSystem", g_engineContext.m_profiler);
				pointLightSystem->Process();
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

