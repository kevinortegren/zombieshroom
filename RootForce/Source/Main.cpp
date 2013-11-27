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
	m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_RENDER, p_workingDirectory);

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
	m_engineContext.m_resourceManager->LoadEffect("test");
	m_engineContext.m_resourceManager->LoadEffect("DiffuseTexture");
	m_engineContext.m_resourceManager->LoadCollada("testhouse");
	m_engineContext.m_resourceManager->LoadCollada("testchar");

	ECS::ComponentSystem* renderingSystem = m_world.GetSystemManager()->CreateSystem<RootForce::RenderingSystem>("RenderingSystem");
	reinterpret_cast<RootForce::RenderingSystem*>(renderingSystem)->SetLoggingInterface(m_engineContext.m_logger);
	reinterpret_cast<RootForce::RenderingSystem*>(renderingSystem)->SetRendererInterface(m_engineContext.m_renderer);

	m_world.GetSystemManager()->InitializeSystems();
	
	ECS::Entity* guy = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* guyTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(guy);
	guyTransform->m_position = glm::vec3(0.0999f, 0.0999f, 0.0999f);


	Utility::Cube quad(Render::VertexType::VERTEXTYPE_1P);

	Render::MeshInterface* mesh = m_engineContext.m_renderer->CreateMesh();
	mesh->Init(reinterpret_cast<Render::Vertex1P*>(quad.m_vertices), quad.m_numberOfVertices, quad.m_indices, quad.m_numberOfIndices);


	RootForce::Renderable* guyRenderable = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(guy);
	guyRenderable->m_mesh = m_engineContext.m_resourceManager->GetModel("testchar")->m_meshes[0];
	//guyRenderable->m_mesh = mesh;

	Render::Material guyMaterial;
	//guyMaterial.m_effect = m_engineContext.m_resourceManager->GetEffect("DiffuseTexture");
	guyMaterial.m_effect = m_engineContext.m_resourceManager->GetEffect("test");
	guyRenderable->m_material = guyMaterial;

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

		HandleEvents();

		// TODO: Poll and handle events
		// TODO: Update game state
		// TODO: Render and present game


		renderingSystem->Process();

		m_engineContext.m_renderer->Render();
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