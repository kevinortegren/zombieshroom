#include <Main.h>

#include <stdexcept>
#include <exception>
#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <RenderingSystem.h>
#include <LightSystem.h>

#include <RootForce/Include/RawMeshPrimitives.h>

#include <glm/glm.hpp>

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
	m_engineContext.m_renderer->SetupSDLContext(m_window.get());

	m_engineContext.m_resourceManager->LoadEffect("Mesh");
	m_engineContext.m_resourceManager->LoadCollada("testchar");

	// Cube mesh.
	std::shared_ptr<Render::Mesh> cubeMesh = m_engineContext.m_renderer->CreateMesh();
	Utility::Cube cube(Render::VertexType::VERTEXTYPE_1P);
	cubeMesh->m_vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
	cubeMesh->m_elementBuffer = m_engineContext.m_renderer->CreateBuffer();
	cubeMesh->m_vertexAttributes = m_engineContext.m_renderer->CreateVertexAttributes();
	cubeMesh->CreateIndexBuffer(cube.m_indices, cube.m_numberOfIndices);
	cubeMesh->CreateVertexBuffer1P(reinterpret_cast<Render::Vertex1P*>(cube.m_vertices), cube.m_numberOfVertices);

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
	m_playerControlSystem->SetInputInterface(m_engineContext.m_inputSys);
	m_playerControlSystem->SetLoggingInterface(m_engineContext.m_logger);
	m_playerControlSystem->SetKeybindings(keybindings);


	RootForce::Renderable::SetTypeId(0);
	RootForce::Transform::SetTypeId(1);
	RootForce::PointLight::SetTypeId(2);
	RootForce::PlayerInputControlComponent::SetTypeId(3);

	// Initialize the system for rendering the scene.
	RootForce::RenderingSystem* renderingSystem = new RootForce::RenderingSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

	renderingSystem->SetLoggingInterface(m_engineContext.m_logger);
	renderingSystem->SetRendererInterface(m_engineContext.m_renderer);

	RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, m_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

	// Setup lights.
	m_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	Render::DirectionalLight directional;
	directional.m_color = glm::vec4(0.2f,0.2f,0.1f,1);
	directional.m_direction = glm::vec3(1, 0, 0);

	m_engineContext.m_renderer->AddDirectionalLight(directional, 0);
	
	ECS::Entity* red = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* redTrans = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(red);
	redTrans->m_position = glm::vec3(1.0f, 3.0f, 0.0f);
	redTrans->m_scale = glm::vec3(0.1f);

	RootForce::PointLight* redPL = m_world.GetEntityManager()->CreateComponent<RootForce::PointLight>(red);
	redPL->m_color = glm::vec4(0.4f, 0.0f, 0.0f, 1.0f);
	redPL->m_attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
	redPL->m_range = 2.0f;

	RootForce::Renderable* redRender = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(red);
	redRender->m_mesh = cubeMesh;
	redRender->m_material.m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");

	ECS::Entity* blue = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* blueTrans = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(blue);
	blueTrans->m_position = glm::vec3(-1.0f, 3.0f, 0.0f);
	blueTrans->m_scale = glm::vec3(0.1f);

	RootForce::PointLight* bluePL = m_world.GetEntityManager()->CreateComponent<RootForce::PointLight>(blue);
	bluePL->m_color = glm::vec4(0.0f, 0.0f, 0.4f, 1.0f);
	bluePL->m_attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
	bluePL->m_range = 2.0f;

	RootForce::Renderable* blueRender = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(blue);
	blueRender->m_mesh = cubeMesh;
	blueRender->m_material.m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");

	m_world.GetGroupManager()->RegisterEntity("Lights", blue);
	m_world.GetGroupManager()->RegisterEntity("Lights", red);
	

	// Setup a dummy player entity and add components to it
	
	ECS::Entity* guy = m_world.GetEntityManager()->CreateEntity();
	
	RootForce::Transform* guyTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(guy);
	guyTransform->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	
	RootForce::Renderable* guyRenderable = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(guy);
	guyRenderable->m_mesh = m_engineContext.m_resourceManager->GetModel("testchar")->m_meshes[0];

	Render::Material guyMaterial;
	guyMaterial.m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
	guyRenderable->m_material = guyMaterial;
	
	RootForce::PlayerInputControlComponent* guyControl = m_world.GetEntityManager()->CreateComponent<RootForce::PlayerInputControlComponent>(guy);
	guyControl->speed = 10.0f;
  
	m_engineContext.m_gui->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_engineContext.m_gui->LoadURL("debug.html");
	m_engineContext.m_debugOverlay->SetView(m_engineContext.m_gui->GetView());

	////////////////////////////////////////////////////////////////////////// AMAZING PHYSICS TEST CODE

	
	ECS::Entity* guy2 = m_world.GetEntityManager()->CreateEntity();
	RootForce::Transform* guyTransform2 = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(guy2);
	guyTransform2->m_position = glm::vec3(0.0f, 0.0f, -6.0f);

	RootForce::Renderable* guyRenderable2 = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(guy2);
	guyRenderable2->m_mesh = m_engineContext.m_resourceManager->GetModel("testchar")->m_meshes[0];

	guyRenderable2->m_material = guyMaterial;

	RootForce::PlayerInputControlComponent* guyControl2 = m_world.GetEntityManager()->CreateComponent<RootForce::PlayerInputControlComponent>(guy2);
	guyControl2->speed = 10.0f;

	int facesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfFaces;
	int verticesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfVertices;
	int indicesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfIndices;
	float* tempVertices = (float*)malloc(verticesTotal * 3 * sizeof(float));
	for(int i = 0; i < verticesTotal; i ++)
	{
		tempVertices[i*3] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].x;  // 0, 3, 6, 9
		tempVertices[i*3 + 1] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].y; //1, 4, 7, 10
		tempVertices[i*3 + 2] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].z;  //2,5,8,11   
	}
	int* tempIndices = (int*)malloc(indicesTotal * sizeof(int));
	tempIndices = (int*)&m_engineContext.m_resourceManager->GetModel("testchar")->meshIndices[0];

	float pos[3] = {3,0,0};
	float rot[3] = {0,0,0};
	int* handle = m_engineContext.m_physics->AddPlayerObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos, rot,5.0f, 10, 0.2f,0.02f);
	float pos2[3] = {0,5,-20};
	float rot2[3] = {0,0,0};
	int* handle2 = m_engineContext.m_physics->AddDynamicObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos2, rot2,5.0f);
	
	float normal[3] = {0,1,0};
	float position[3] = {0, -2, 0};
	m_engineContext.m_physics->CreatePlane(normal, position);
	float normal2[3] = {0,0,1};
	float position2[3] = {0, 0, -60};
	m_engineContext.m_physics->CreatePlane(normal2, position2);
	float normal3[3] = {0,0,-1};
	float position3[3] = {0, 0, 4};
	m_engineContext.m_physics->CreatePlane(normal3, position3);
	//float speed[3] = {0, 5, -5};
	float* speed;
	float speedup[3] = {0, 10 , 0};
	float x[3], x2[3];
	for(int i = 0 ; i < 5; i++)
	{
		float ballpos[3] = {0,3 + i * 1.5f, 0};
		int ballHandle = m_engineContext.m_physics->CreateSphere(i +0.04f, 1.05f,ballpos );
	}
	
	float ballspeed[3] = {0, 0, 5};
	float orientationPlayer[4] = {0,0,0, 0};
	float orientation[4] = {0,0,0, 0};
	float yaw = 0;
	
	//////////////////////////////////////////////////////////////////////////


	m_world.GetTagManager()->RegisterEntity("Player", guy);
	
	m_world.GetGroupManager()->PrintEntitiesInGroup("Lights");

	m_world.GetGroupManager()->UnregisterEntity("Lights", red);

	m_world.GetGroupManager()->PrintEntitiesInGroup("Lights");

	RootForce::Transform* t = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(m_world.GetTagManager()->GetEntityByTag("Player"));
	glm::vec3 a = t->m_position;
	// Start the main loop
	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;
    
		m_engineContext.m_debugOverlay->Clear();
		m_world.SetDelta(dt);
		m_engineContext.m_debugOverlay->AddHTML(std::to_string(dt).c_str(), RootEngine::TextColor::GRAY, false);
		HandleEvents();
		// TODO: Update game state
		// TODO: Render and present game

		m_engineContext.m_renderer->Clear();

		// Update Game systems.
		pointLightSystem->Process();
		m_playerControlSystem->Process();
		renderingSystem->Process();

		// Update Engine systems.
		m_engineContext.m_physics->Update(dt);

		

		/////// PHYSICS TESTING CODE, UNCOMMENT FOR AMAZING PHYSICS
		m_engineContext.m_physics->GetPlayerPos(*handle, x);
		m_engineContext.m_physics->GetObjectPos(*handle2, x2);


		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_SPACE) == RootEngine::InputManager::KeyState::DOWN)
			m_engineContext.m_physics->PlayerJump(*handle, 10.0f);
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_LCTRL) == RootEngine::InputManager::KeyState::DOWN_EDGE )
		{
			glm::vec3 temp = guyTransform->m_orientation.GetFront();
			speed = &temp.x;
			speed[1] = 4;
			m_engineContext.m_physics->PlayerKnockback(*handle, speed, 50.0f);
		}
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_RCTRL) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			//speed[2] *= -1;
			//m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Collisionshape x: %f y: %f z: %f", x[0], x[1], x[2]);
			m_engineContext.m_physics->SetDynamicObjectVelocity(*handle2, speedup);
			//m_engineContext.m_logger->LogText(  LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Orientation %f %f %f", orientation[0], orientation[1], orientation[2]);
		}
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_W) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			
		}
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_UP) == RootEngine::InputManager::KeyState::DOWN)
		{
			//speed[2] *= -1;
			glm::vec3 test = guyTransform->m_orientation.GetFront();
			float* funtime = &test.x;
			m_engineContext.m_physics->PlayerMoveXZ(*handle, funtime);
			//m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Collisionshape x: %f y: %f z: %f", x[0], x[1], x[2]);
			//m_engineContext.m_physics->SetDynamicObjectVelocity(ballHandle, ballspeed);
			//m_engineContext.m_logger->LogText(  LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Orientation %f %f %f", orientation[0], orientation[1], orientation[2]);
		}
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_LSHIFT) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			glm::vec3 test = guyTransform->m_orientation.GetFront();
			float* funtime = &test.x;
			float gravity[3] = {0,0,0};
			float* position = x;
			position[0] += funtime[0];
			position[1] += 1;
			position[2] += funtime[2];
			m_engineContext.m_physics->AddAbilityToWorld(0.8f, x, funtime, 100.0f, 1, 500, gravity );
		}
		if(m_engineContext.m_inputSys->GetKeyState(SDL_Scancode::SDL_SCANCODE_RSHIFT) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_engineContext.m_physics->RemoveObject(1, 1);
		}
		guyTransform->m_position = glm::vec3(x[0], x[1], x[2]);
		guyTransform2->m_position = glm::vec3(x2[0], x2[1], x2[2]);
		//float target[3] = {x[0] - x2[0] , x[1] - x2[1] , x[2] - x2[2]};
		//m_engineContext.m_physics->SetGravity(*handle2, target);
		
		m_engineContext.m_physics->GetObjectOrientation(*handle2, orientation);
		guyTransform2->m_orientation.SetOrientation(glm::quat(orientation[0], orientation[1], orientation[2], orientation[3]));
		glm::quat test = guyTransform->m_orientation.GetQuaterion();

		orientationPlayer[0] =- test.x;
		orientationPlayer[1] = -test.y;
		orientationPlayer[2] = -test.z;
		orientationPlayer[3] = -test.w;
		m_engineContext.m_physics->SetPlayerOrientation(*handle,orientationPlayer);


		m_engineContext.m_renderer->Render();
		//m_engineContext.m_renderer->RenderLines();


		m_engineContext.m_gui->Update();
		m_engineContext.m_gui->Render();

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
			if(m_engineContext.m_gui != nullptr)
				m_engineContext.m_gui->HandleEvents(event);
		}
	}
}
