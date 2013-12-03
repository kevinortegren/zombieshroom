#include <Main.h>

#include <stdexcept>
#include <exception>
#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <RenderingSystem.h>
#include <LightSystem.h>
#include <PlayerControlSystem.h>

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

static void Exporter(YAML::Emitter& p_emitter, ECS::ComponentInterface* p_component, int p_type)
{
	switch(p_type)
	{
		case 0:
			{
				RootForce::Renderable* renderable = static_cast<RootForce::Renderable*>(p_component);	
				if(renderable->m_model != nullptr)
				{
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromModel(renderable->m_model);
					p_emitter << YAML::Key << "Model" << YAML::Value << s;
				}
				if(renderable->m_material.m_effect != nullptr)
				{
					std::string s = g_engineContext.m_resourceManager->ResolveStringFromEffect(renderable->m_material.m_effect);
					p_emitter << YAML::Key << "Effect" << YAML::Value << s;
				}				
			}
			break;
		case 1:
			{
				RootForce::Transform* transform = static_cast<RootForce::Transform*>(p_component);
				glm::vec3 position = transform->m_position;
				glm::vec3 scale = transform->m_scale;
				glm::vec3 rotation = transform->m_orientation.GetAxis();

				p_emitter << YAML::Key << "Position" << YAML::Value << YAML::Flow << YAML::BeginSeq << position.x << position.y << position.z << YAML::EndSeq;
				p_emitter << YAML::Key << "Rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq << rotation.x << rotation.y << rotation.z << YAML::EndSeq;
				p_emitter << YAML::Key << "Scale" << YAML::Value << YAML::Flow << YAML::BeginSeq << scale.x << scale.y << scale.z << YAML::EndSeq;
			}
			break;
		case 2:
			{
				RootForce::PointLight* pointLight = static_cast<RootForce::PointLight*>(p_component);
				glm::vec3 attenuation = pointLight->m_attenuation;
				glm::vec4 color = pointLight->m_color;
				float range = pointLight->m_range;

				p_emitter << YAML::Key << "Atenuation" << YAML::Value << YAML::Flow << YAML::BeginSeq << attenuation.x << attenuation.y << attenuation.z << YAML::EndSeq;
				p_emitter << YAML::Key << "Color" << YAML::Value << YAML::Flow << YAML::BeginSeq << color.x << color.y << color.z << color.w << YAML::EndSeq;
				p_emitter << YAML::Key << "Range" << YAML::Value << range;
			}
			break;
		case 3:
			{
				RootForce::PlayerInputControlComponent* input = static_cast<RootForce::PlayerInputControlComponent*>(p_component);
				p_emitter << YAML::Key << "Speed" << YAML::Value << input->speed;
			}
			break;
		default:
			break;
	}
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

	g_engineContext.m_resourceManager->LoadEffect("Mesh");
	g_engineContext.m_resourceManager->LoadCollada("testchar");

	// Cube mesh.
	std::shared_ptr<Render::Mesh> cubeMesh = g_engineContext.m_renderer->CreateMesh();
	Utility::Cube cube(Render::VertexType::VERTEXTYPE_1P);
	cubeMesh->m_vertexBuffer = g_engineContext.m_renderer->CreateBuffer();
	cubeMesh->m_elementBuffer = g_engineContext.m_renderer->CreateBuffer();
	cubeMesh->m_vertexAttributes = g_engineContext.m_renderer->CreateVertexAttributes();
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

	RootForce::Renderable::SetTypeId(0);
	RootForce::Transform::SetTypeId(1);
	RootForce::PointLight::SetTypeId(2);
	RootForce::PlayerInputControlComponent::SetTypeId(3);

	m_world.SetExporter(Exporter);

	m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(&m_world));
	m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
	m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
	m_playerControlSystem->SetKeybindings(keybindings);

	// Initialize the system for rendering the scene.
	RootForce::RenderingSystem* renderingSystem = new RootForce::RenderingSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

	renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
	renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

	RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

	// Setup lights.
	g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	Render::DirectionalLight directional;
	directional.m_color = glm::vec4(0.2f,0.2f,0.1f,1);
	directional.m_direction = glm::vec3(1, 0, 0);

	g_engineContext.m_renderer->AddDirectionalLight(directional, 0);
	
	ECS::Entity* red = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* redTrans = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(red);
	redTrans->m_position = glm::vec3(1.0f, 3.0f, 0.0f);
	redTrans->m_scale = glm::vec3(0.1f);

	RootForce::PointLight* redPL = m_world.GetEntityManager()->CreateComponent<RootForce::PointLight>(red);
	redPL->m_color = glm::vec4(0.4f, 0.0f, 0.0f, 1.0f);
	redPL->m_attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
	redPL->m_range = 2.0f;

	RootForce::Renderable* redRender = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(red);
	redRender->m_material.m_effect = g_engineContext.m_resourceManager->GetEffect("Mesh");

	ECS::Entity* blue = m_world.GetEntityManager()->CreateEntity();

	RootForce::Transform* blueTrans = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(blue);
	blueTrans->m_position = glm::vec3(-1.0f, 3.0f, 0.0f);
	blueTrans->m_scale = glm::vec3(0.1f);

	RootForce::PointLight* bluePL = m_world.GetEntityManager()->CreateComponent<RootForce::PointLight>(blue);
	bluePL->m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	bluePL->m_attenuation = glm::vec3(0.0f, 0.2f, 0.0f);
	bluePL->m_range = 20.0f;

	RootForce::Renderable* blueRender = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(blue);
	blueRender->m_material.m_effect = g_engineContext.m_resourceManager->GetEffect("Mesh");

	m_world.GetGroupManager()->RegisterEntity("Lights", blue);
	m_world.GetGroupManager()->RegisterEntity("Lights", red);
	

	// Setup a dummy player entity and add components to it
	
	ECS::Entity* guy = m_world.GetEntityManager()->CreateEntity();
	
	RootForce::Transform* guyTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(guy);
	guyTransform->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	
	RootForce::Renderable* guyRenderable = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(guy);
	guyRenderable->m_model = g_engineContext.m_resourceManager->GetModel("testchar");

	Render::Material guyMaterial;
	guyMaterial.m_effect = g_engineContext.m_resourceManager->GetEffect("Mesh");
	guyMaterial.m_diffuseMap = g_engineContext.m_resourceManager->GetTexture(g_engineContext.m_resourceManager->GetModel("testchar")->m_textureHandles[0]);
	guyRenderable->m_material = guyMaterial;
	
	RootForce::PlayerInputControlComponent* guyControl = m_world.GetEntityManager()->CreateComponent<RootForce::PlayerInputControlComponent>(guy);
	guyControl->speed = 10.0f;
  
	g_engineContext.m_gui->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	g_engineContext.m_gui->LoadURL("debug.html");
	g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->GetView());

	////////////////////////////////////////////////////////////////////////// AMAZING PHYSICS TEST CODE

	/*
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
	int handle = m_engineContext.m_physics->AddPlayerObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos, rot,5.0f, 10, 0.2f,0.02f);
	float pos2[3] = {0,5,-20};
	float rot2[3] = {0,0,0};
	int handle2 = m_engineContext.m_physics->AddDynamicObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos2, rot2,5.0f);
	
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
	for(int i = 0 ; i < 10; i++)
	{
		float ballpos[3] = {0,3 + i * 1.5f, 0};
		int ballHandle = m_engineContext.m_physics->CreateSphere(1, 1.05f,ballpos );
	}
	
	float ballspeed[3] = {0, 0, 5};
	float orientationPlayer[4] = {0,0,0, 0};
	float orientation[4] = {0,0,0, 0};
	float yaw = 0;
	*/
	//////////////////////////////////////////////////////////////////////////


	m_world.GetTagManager()->RegisterEntity("Player", guy);
	
	m_world.GetGroupManager()->PrintEntitiesInGroup("Lights");

	m_world.GetGroupManager()->UnregisterEntity("Lights", red);

	m_world.GetGroupManager()->PrintEntitiesInGroup("Lights");

	RootForce::Transform* t = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(m_world.GetTagManager()->GetEntityByTag("Player"));
	glm::vec3 a = t->m_position;

	m_world.Export();

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

		g_engineContext.m_renderer->Clear();

		g_engineContext.m_physics->Update(dt);

		pointLightSystem->Process(); 
		renderingSystem->Process();

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
