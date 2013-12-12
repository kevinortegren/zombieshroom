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

#undef main

int main(int argc, char* argv[]) 
{
	RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
	RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
	RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
	RootForce::PlayerControl::SetTypeId(RootForce::ComponentType::PLAYERCONTROL);
	RootForce::PhysicsAccessor::SetTypeId(RootForce::ComponentType::PHYSICS);
	RootForce::Network::NetworkClientComponent::SetTypeId(RootForce::ComponentType::NETWORKCLIENT);
	RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
	RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
	RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
	RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);

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
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::entity_f, "Entity");
		RootForce::LuaAPI::LuaSetupTypeWithMethods(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::renderable_f, RootForce::LuaAPI::renderable_m, "Renderable");
		RootForce::LuaAPI::LuaSetupTypeWithMethods(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::transformation_f, RootForce::LuaAPI::transformation_m, "Transformation");
		RootForce::LuaAPI::LuaSetupTypeWithMethods(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::physicsaccessor_f, RootForce::LuaAPI::physicsaccessor_m, "PhysicsAccessor");
		
		g_world = &m_world;

		// Initialize the system for controlling the player.
		std::vector<RootForce::Keybinding> keybindings(4);
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

		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(&m_world));
		m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
		m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_playerControlSystem->SetKeybindings(keybindings);
		m_playerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

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

		g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

		Render::DirectionalLight dl;
		dl.m_color = glm::vec4(0.3f,0.3f,0.3f,1);
		dl.m_direction = glm::vec3(0,0,-1);

		g_engineContext.m_renderer->AddDirectionalLight(dl, 0);

		RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
		m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

		RootForce::AbilitySystem* abilitySystem = new RootForce::AbilitySystem(&m_world, g_engineContext.m_renderer);
		m_world.GetSystemManager()->AddSystem<RootForce::AbilitySystem>(abilitySystem, "AbilitySystem");

		// Import test world.
		m_world.GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");

		ECS::EntityManager* em = m_world.GetEntityManager();

		//Create player aiming device

		ECS::Entity* aimingDevice = em->CreateEntity();
		m_world.GetTagManager()->RegisterEntity("AimingDevice", aimingDevice);
		RootForce::Transform* aimingDeviceTransform = em->CreateComponent<RootForce::Transform>(aimingDevice);
		

		//Create camera
		ECS::Entity* cameraEntity = m_world.GetEntityManager()->CreateEntity();
		m_world.GetTagManager()->RegisterEntity("Camera", cameraEntity);
		RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
		camera->m_near = 0.1f;
		camera->m_far = 1000.0f;
		camera->m_fov = 75.0f;
		RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);
		RootForce::LookAtBehavior* cameraLookAt = m_world.GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
		cameraLookAt->m_targetTag = "AimingDevice";
		cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
		RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world.GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
		cameraThirdPerson->m_targetTag = "AimingDevice";
		cameraThirdPerson->m_displacement = glm::vec3(0.0f, 4.0f, -8.0f);


		//Plane at bottom

		glm::vec3 normal (0,1,0);
		glm::vec3 position (0, -2, 0);
	
		g_engineContext.m_physics->CreatePlane(normal, position);

		// Setup the skybox.
		auto e = m_world.GetTagManager()->GetEntityByTag("Skybox");
		auto r = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(e);
		r->m_material.m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(
			"rnl_cross", Render::TextureType::TEXTURE_CUBEMAP);

		g_engineContext.m_gui->Initialize(g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenWidth"),
			g_engineContext.m_configManager->GetConfigValueAsInteger("ScreenHeight"));

		g_engineContext.m_gui->LoadURL("debug.html");
		g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->GetView());

		// Initialize the network system
		RootForce::Network::MessageHandler::ServerType serverType = RootForce::Network::MessageHandler::LOCAL;
		m_networkHandler = std::shared_ptr<RootForce::Network::MessageHandler>(new RootForce::Network::MessageHandler(&m_world, g_engineContext.m_logger, g_engineContext.m_network, serverType, 5567, "127.0.0.1"));

	

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

			// Code for testing scripts
			if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_1) == RootEngine::InputManager::KeyState::DOWN_EDGE)
			{
				g_engineContext.m_script->LoadScript("AbilityTest.lua");
				g_engineContext.m_script->SetFunction("AbilityTest", "OnActivate");
				g_engineContext.m_script->ExecuteScript();
			}

			g_engineContext.m_debugOverlay->AddHTMLToBuffer(std::to_string(dt).c_str(), RootEngine::TextColor::GRAY, false);
		
			{
				PROFILE("Handle Events", g_engineContext.m_profiler);
				HandleEvents();
			}

			{
				PROFILE("Entity Systems", g_engineContext.m_profiler);
				m_playerControlSystem->Process();
				abilitySystem->Process();

			}

			{
				PROFILE("Network message handler", g_engineContext.m_profiler);
				m_networkHandler->Update();
			}

			{
				PROFILE("Physics", g_engineContext.m_profiler);
				m_physicsSystem->Process();
				g_engineContext.m_physics->Update(dt);
				
			}

			thirdPersonBehaviorSystem->Process();
			lookAtSystem->Process();
			cameraSystem->Process();
			pointLightSystem->Process();
			renderingSystem->Process();
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

