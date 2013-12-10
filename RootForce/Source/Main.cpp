#include <Main.h>
#include <stdexcept>
#include <exception>
#include <gtest/gtest.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

//#include <RenderingSystem.h>
//#include <LightSystem.h>
#include <PhysicsSystem.h>


#include <RootForce/Include/RawMeshPrimitives.h>
#include <glm/glm.hpp>

#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#undef main

TEST(Test, Foo) 
{
	int a = 0;
	EXPECT_TRUE(a == 0);
}

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Rootforce.exe";
	path = path.substr(0, path.size() - rootforcename.size());
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
			Main m(path);
			m.Start();
		}
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		std::cout << e.what() << "\n";
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

Main::Main(std::string p_workingDirectory) 
	: m_running(true), m_world(5)
{
	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, p_workingDirectory);

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		// TODO: Log error and throw exception (?)
	}

	// TODO: Make these parameters more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			"Root Force",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
		SDL_DestroyWindow);
	if (m_window == nullptr) 
	{
		// TODO: Log error and throw exception (?)
	}
}

Main::~Main() 
{
	m_world.GetEntityExporter()->Export(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	g_engineContext.m_renderer->SetupSDLContext(m_window.get());

	RootForce::Renderable::SetTypeId(0);
	RootForce::Transform::SetTypeId(1);
	RootForce::PointLight::SetTypeId(2);
	RootForce::PlayerInputControlComponent::SetTypeId(3);
	RootForce::PhysicsAccessor::SetTypeId(4);

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

	Render::DirectionalLight dl;
	dl.m_color = glm::vec4(1,0,1,1);
	dl.m_direction = glm::vec3(0,0,-1);

	g_engineContext.m_renderer->AddDirectionalLight(dl, 0);

	RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

	RootForce::AbilitySystem* abilitySystem = new RootForce::AbilitySystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::AbilitySystem>(abilitySystem, "AbilitySystem");

	// Import test world.
	m_world.GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");


	
	//Plane at bottom
	float normal[3] = {0,1,0};
	float position[3] = {0, -2, 0};
	
	g_engineContext.m_physics->CreatePlane(normal, position);

	float dir[3] = {1,0,0};
	float grav[3] = {0,-1,0};
	float pos[3] = {-6,1,-6};
	float rot[4] = {0,0,0,1};
	RootEngine::Physics::AbilityPhysicsInfo test;
	test.m_collidesWorld = true;
	test.m_direction = dir;
	test.m_entityId = -1;
	test.m_gravity = grav;
	test.m_height = 2.f;
	test.m_mass = 1;
	test.m_position = pos;
	test.m_radius = 0.5f;
	test.m_orientation = rot;
	test.m_shape = RootEngine::Physics::AbilityShape::SHAPE_SPHERE;
	test.m_speed = 0.03f;
	test.m_type = RootEngine::Physics::PhysicsType::TYPE_ABILITY;
	g_engineContext.m_physics->AddAbilityToWorld(test);
	float pos1[3] = {0, -1, 0};
	float dir1[3] = {-1,-1,0};
	test.m_position = pos1;
	test.m_direction = dir1;
	test.m_radius = 10;
	test.m_mass = 0;
	test.m_collidesWorld = true;
	test.m_shape = RootEngine::Physics::AbilityShape::SHAPE_CONE;
	g_engineContext.m_physics->AddAbilityToWorld(test);


	g_engineContext.m_gui->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	g_engineContext.m_gui->LoadURL("debug.html");
	g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->GetView());


	// Start the main loop
	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{	
		
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;
		
		g_engineContext.m_debugOverlay->Clear();

		m_world.SetDelta(dt);
		g_engineContext.m_debugOverlay->AddHTML(std::to_string(dt).c_str(), RootEngine::TextColor::GRAY, false);
		
		{
			PROFILE("Handle Events", g_engineContext.m_profiler);
			HandleEvents();
		}
		{
			PROFILE("Player control system", g_engineContext.m_profiler);
			m_playerControlSystem->Process();
		}
		abilitySystem->Process();

		g_engineContext.m_renderer->Clear();

		{
			PROFILE("Physics", g_engineContext.m_profiler);
			g_engineContext.m_physics->Update(dt);
		}

		// Update Game systems.
		pointLightSystem->Process();
		m_physicsSystem->Process();
		renderingSystem->Process();
		
		{
			PROFILE("Render", g_engineContext.m_profiler);
			g_engineContext.m_renderer->Render();
		}

		// Update Engine systems.
		{
			PROFILE("RenderLines", g_engineContext.m_profiler);
			g_engineContext.m_renderer->RenderLines();
		}
		
	
			g_engineContext.m_profiler->Update(dt);
		
		
		{
			PROFILE("GUI", g_engineContext.m_profiler);
			g_engineContext.m_gui->Update();
			g_engineContext.m_gui->Render();
		}

		g_engineContext.m_renderer->Swap();
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

		default:
			if (g_engineContext.m_inputSys != nullptr)
				g_engineContext.m_inputSys->HandleInput(event);
			if(g_engineContext.m_gui != nullptr)
				g_engineContext.m_gui->HandleEvents(event);
		}
	}
}
