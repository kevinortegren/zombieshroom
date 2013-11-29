
#include <Main.h>

#include <stdexcept>

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Vertex.h>


#include <gtest/gtest.h>

#include <RootForce/Include/RawMeshPrimitives.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <RenderingSystem.h>
#include <PlayerControlSystem.h>

#include <exception>

#include <glm/glm.hpp>

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
	: m_running(true)
{

	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, p_workingDirectory);

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
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
		SDL_DestroyWindow);
	if (m_window == nullptr) 
	{
		// TODO: Log error and throw exception (?)
	}

}

Main::~Main() 
{
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	m_engineContext.m_renderer->SetupSDLContext(m_window.get());

	m_engineContext.m_resourceManager->LoadEffect("Mesh");
	m_engineContext.m_resourceManager->LoadCollada("testchar");

	// Initialize the system for controlling the player.
	std::vector<RootForce::Keybinding> keybindings(4);
	keybindings[0].Bindings.push_back(SDL_SCANCODE_UP);
	keybindings[0].Bindings.push_back(SDL_SCANCODE_W);
	keybindings[0].Action = RootForce::PlayerAction::MOVE_FORWARDS;

	m_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	Render::DirectionalLight directional;
	directional.m_color = glm::vec4(0.2f,0.2f,0.1f,1);
	directional.m_direction = glm::vec3(1, 0, 0);

	m_engineContext.m_renderer->AddDirectionalLight(directional, 0);

	Render::PointLight red;
	red.m_position = glm::vec3(1.0f, 3.0f, 0.0f);
	red.m_attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
	red.m_range = 2.0f;
	red.m_color = glm::vec4(0.4f, 0.0f, 0.0f, 1.0f);

	Render::PointLight blue;
	blue.m_position = glm::vec3(-1.0f, 3.0f, 0.0f);
	blue.m_attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
	blue.m_range = 2.0f;
	blue.m_color = glm::vec4(0.0f, 0.0f, 0.4f, 1.0f);

	Render::PointLight green;
	green.m_position = glm::vec3(0.0f, 3.0f, 1.0f);
	green.m_attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
	green.m_range = 2.0f;
	green.m_color = glm::vec4(0.0f, 0.4f, 0.0f, 1.0f);

	m_engineContext.m_renderer->AddPointLight(red, 0);
	m_engineContext.m_renderer->AddPointLight(blue, 1);
	m_engineContext.m_renderer->AddPointLight(green, 2);

	Utility::Cube quad(Render::VertexType::VERTEXTYPE_1P);

	keybindings[1].Bindings.push_back(SDL_SCANCODE_DOWN);
	keybindings[1].Bindings.push_back(SDL_SCANCODE_S);
	keybindings[1].Action = RootForce::PlayerAction::MOVE_BACKWARDS;

	keybindings[2].Bindings.push_back(SDL_SCANCODE_LEFT);
	keybindings[2].Bindings.push_back(SDL_SCANCODE_A);
	keybindings[2].Action = RootForce::PlayerAction::STRAFE_LEFT;

	keybindings[3].Bindings.push_back(SDL_SCANCODE_RIGHT);
	keybindings[3].Bindings.push_back(SDL_SCANCODE_D);
	keybindings[3].Action = RootForce::PlayerAction::STRAFE_RIGHT;

	ECS::ComponentSystem* playerControlSystem = m_world.GetSystemManager()->CreateSystem<RootForce::PlayerControlSystem>("PlayerControlSystem");
	dynamic_cast<RootForce::PlayerControlSystem*>(playerControlSystem)->SetInputInterface(m_engineContext.m_inputSys);
	dynamic_cast<RootForce::PlayerControlSystem*>(playerControlSystem)->SetLoggingInterface(m_engineContext.m_logger);
	dynamic_cast<RootForce::PlayerControlSystem*>(playerControlSystem)->SetKeybindings(keybindings);

	// Initialize the system for rendering the scene.
	ECS::ComponentSystem* renderingSystem = m_world.GetSystemManager()->CreateSystem<RootForce::RenderingSystem>("RenderingSystem");
	reinterpret_cast<RootForce::RenderingSystem*>(renderingSystem)->SetLoggingInterface(m_engineContext.m_logger);
	reinterpret_cast<RootForce::RenderingSystem*>(renderingSystem)->SetRendererInterface(m_engineContext.m_renderer);

	m_world.GetSystemManager()->InitializeSystems();

	// Setup a dummy player entity and add components to it
	ECS::Entity* guy = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* guyTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(guy);
	guyTransform->m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	Render::MeshInterface* mesh = m_engineContext.m_renderer->CreateMesh();
	mesh->Init(reinterpret_cast<Render::Vertex1P*>(quad.m_vertices), quad.m_numberOfVertices, quad.m_indices, quad.m_numberOfIndices);

	RootForce::Renderable* guyRenderable = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(guy);
	guyRenderable->m_mesh = m_engineContext.m_resourceManager->GetModel("testchar")->m_meshes[0];
	//guyRenderable->m_mesh = mesh;

	Render::Material guyMaterial;
	//guyMaterial.m_effect = m_engineContext.m_resourceManager->GetEffect("DiffuseTexture");
	guyMaterial.m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
	guyRenderable->m_material = guyMaterial;

	RootForce::PlayerInputControlComponent* guyControl = m_world.GetEntityManager()->CreateComponent<RootForce::PlayerInputControlComponent>(guy);
	guyControl->speed = 10.0f;

	// Start the main loop
	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		HandleEvents();
		// TODO: Update game state
		// TODO: Render and present game
		
		m_engineContext.m_physics->Update(dt);

		m_engineContext.m_renderer->Clear();
	
		playerControlSystem->Process(dt);
		renderingSystem->Process(dt);

		m_engineContext.m_renderer->Swap();
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

			if (m_engineContext.m_inputSys != nullptr)
				m_engineContext.m_inputSys->HandleInput(event);
		}
	}
}

