#include <Main.h>
#include <stdexcept>
#include <exception>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

//#include <RenderingSystem.h>
//#include <LightSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>


#include <RootForce/Include/RawMeshPrimitives.h>
#include <glm/glm.hpp>

#include <RootSystems/Include/Network/Messages.h>

#include <RootForce/Include/ComponentExporter.h>
#include <RootForce/Include/ComponentImporter.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#undef main

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Rootforce.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		Main m(path);
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

	RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");


	RootForce::AbilitySystem* abilitySystem = new RootForce::AbilitySystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::AbilitySystem>(abilitySystem, "AbilitySystem");


	// Import test world.
	m_world.GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");

	//RootForce::PhysicsAccessor* physaccessor = m_world.GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(m_world.GetTagManager()->GetEntityByTag("Player"));
	//int temp[1] = {0};
	//physaccessor->m_handle = temp;
	
	//m_world.GetEntityExporter()->Export(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");

	g_engineContext.m_gui->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	g_engineContext.m_gui->LoadURL("debug.html");
	g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->GetView());


	////////////////////////////////////////////////////////////////////////// AMAZING PHYSICS TEST CODE

	//
	//ECS::Entity* guy2 = m_world.GetEntityManager()->CreateEntity();
	//RootForce::Transform* guyTransform2 = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(guy2);
	//guyTransform2->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	//RootForce::PhysicsAccessor* guyPhysics2 = m_world.GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(guy2);
	//RootForce::Renderable* guyRenderable2 = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(guy2);
	//guyRenderable2->m_mesh = g_engineContext.m_resourceManager->GetModel("testchar")->m_meshes[0];
	//Render::Material guyMaterial2;
	//guyMaterial2.m_effect = g_engineContext.m_resourceManager->GetEffect("Mesh");
	//guyMaterial2.m_diffuseMap = g_engineContext.m_resourceManager->GetTexture(g_engineContext.m_resourceManager->GetModel("testchar")->m_textureHandles[0]);
	//guyRenderable2->m_material = guyMaterial2;

	//RootForce::PlayerInputControlComponent* guyControl2 = m_world.GetEntityManager()->CreateComponent<RootForce::PlayerInputControlComponent>(guy2);
	//guyControl2->speed = 10.0f;

	//int facesTotal = g_engineContext.m_resourceManager->GetModel("testchar")->numberOfFaces;
	//int verticesTotal = g_engineContext.m_resourceManager->GetModel("testchar")->numberOfVertices;
	//int indicesTotal = g_engineContext.m_resourceManager->GetModel("testchar")->numberOfIndices;
	//float* tempVertices = (float*)malloc(verticesTotal * 3 * sizeof(float));
	//for(int i = 0; i < verticesTotal; i ++)
	//{
	//	tempVertices[i*3] = g_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].x;  // 0, 3, 6, 9
	//	tempVertices[i*3 + 1] = g_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].y; //1, 4, 7, 10
	//	tempVertices[i*3 + 2] = g_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].z;  //2, 5, 8, 11
	//}
	//int* tempIndices = (int*)malloc(indicesTotal * sizeof(int));
	//tempIndices = (int*)&g_engineContext.m_resourceManager->GetModel("testchar")->meshIndices[0];

	//float pos[3] = {3,0,0};
	//float rot[3] = {0,0,0};
	//guyPhysics->m_handle = g_engineContext.m_physics->AddPlayerObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos, rot,5.0f, 10, 0.2f,0.02f);
	//float pos2[3] = {0,5,-20};
	//float rot2[3] = {0,0,0};
	//guyPhysics2->m_handle = g_engineContext.m_physics->AddDynamicObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos2, rot2,5.0f);
	//
	//float normal[3] = {0,1,0};
	//float position[3] = {0, -2, 0};
	//g_engineContext.m_physics->CreatePlane(normal, position);
	//float normal2[3] = {0,0,1};
	//float position2[3] = {0, 0, -60};
	//g_engineContext.m_physics->CreatePlane(normal2, position2);
	//float normal3[3] = {0,0,-1};
	//float position3[3] = {0, 0, 4};
	//g_engineContext.m_physics->CreatePlane(normal3, position3);
	////float speed[3] = {0, 5, -5};
	//
	//float speedup[3] = {0, 10 , 0};
	//float x[3], x2[3];
	//for(int i = 0 ; i < 5; i++)
	//{
	//	float ballpos[3] = {0,3 + i * 1.5f, 0};
	////	int ballHandle = g_engineContext.m_physics->CreateSphere(i +0.04f, 1.05f,ballpos );
	//}
	//
	//float ballspeed[3] = {0, 0, 5};
	//float orientationPlayer[4] = {0,0,0, 0};
	//float orientation[4] = {0,0,0, 0};
	//float yaw = 0;
	//
	//////////////////////////////////////////////////////////////////////////
	
	// Initialize the network system
	RootForce::Network::MessageHandler::ServerType serverType = RootForce::Network::MessageHandler::LOCAL;
	m_networkHandler = std::shared_ptr<RootForce::Network::MessageHandler>(new RootForce::Network::MessageHandler(&m_world, g_engineContext.m_logger, g_engineContext.m_network, serverType, 5567, "127.0.0.1"));
	

	float normal[3] = {0,1,0};
	float position[3] = {0, -2, 0};
	g_engineContext.m_physics->CreatePlane(normal, position);

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
		HandleEvents();

		m_playerControlSystem->Process();

		m_networkHandler->Update();

		abilitySystem->Process();

		g_engineContext.m_renderer->Clear();

		g_engineContext.m_physics->Update(dt);


		// Update Game systems.
		pointLightSystem->Process();
		m_physicsSystem->Process();
		renderingSystem->Process();


		// Update Engine systems.

		g_engineContext.m_renderer->Render();
		g_engineContext.m_renderer->RenderLines();


		g_engineContext.m_gui->Update();
		g_engineContext.m_gui->Render();



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
