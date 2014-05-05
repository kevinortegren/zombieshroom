#include "Treenity.h"
#include <RootTools/Treenity/Include/TreenityMain.h>
#include <QtWidgets/QApplication>
#include <SDL2/SDL.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootTools/Treenity/Include/EngineActions.h>

#include <RootEngine/Include/RootEngine.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootSystems/Include/Components.h>

#include <RootTools/Treenity/Include/ComponentImporter.h>
#include <RootTools/Treenity/Include/ComponentExporter.h>
#include <QString>

#undef main
int main(int argc, char *argv[])
{
	std::string path(argv[0]);
	std::string rootforcename = "Treenity.exe";
	path = path.substr(0, path.size() - rootforcename.size());

	QApplication a(argc, argv);
	TreenityMain m(path);

	uint64_t old = SDL_GetPerformanceCounter();
	while(m.IsRunning())
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		//w.Update(dt);
		a.processEvents(QEventLoop::AllEvents);
		m.Update(dt);
	}

	return 0;
}

TreenityMain::TreenityMain(const std::string& p_path)
	: m_engineActions(&m_world)
	, m_projectManager(&m_world)
{
	// Load & Initialize Root Engine.
	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	if (m_engineModule == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	if (libInitializeEngine == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_INPUT | RootEngine::SubsystemInit::INIT_RENDER, p_path);

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_TIMER) != 0) 
	{
		throw std::runtime_error("Failed to initialize SDL");
	}

	m_treenityEditor.CreateOpenGLContext();
	m_treenityEditor.SetEngineInterface(&m_engineActions);
	m_treenityEditor.SetProjectManager(&m_projectManager);

	// Initialize components and preallocate memory.
	RootForce::ComponentType::Initialize();
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::HealthComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerControl>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ControllerActions>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Physics>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::NetworkComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::LookAtBehavior>(20);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ThirdPersonBehavior>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Script>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Collision>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::CollisionResponder>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Animation>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TDMRuleSet>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerActionComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerPhysics>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::StateComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Shadowcaster>(1);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::DirectionalLight>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ClientComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ServerInformationComponent>(10);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Ragdoll>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::WaterCollider>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::AbilitySpawnComponent>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TryPickupComponent>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::SoundComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::TimerComponent>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::FollowComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::HomingComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::RayComponent>(1000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::DamageAndKnockback>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Scalable>(5000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::StatChange>(500);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::KillAnnouncement>(10);

	m_renderingSystem = new RootForce::RenderingSystem(&m_world);
	m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);
	m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);

	m_cameraSystem = new RootForce::CameraSystem(&m_world, &g_engineContext);
	m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem);

	m_transformInterpolationSystem = new RootForce::TransformInterpolationSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::TransformInterpolationSystem>(m_transformInterpolationSystem);

	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem);

	m_world.GetEntityImporter()->SetImporter(Importer);
	m_world.GetEntityExporter()->SetExporter(Exporter);
	//m_world.GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\ColorCube3.0.world");
	//m_world.GetEntityImporter()->Import("C:\\rarosu\\test12.level");
	//m_projectManager.Import("C:\\rarosu\\test14.level");
	m_projectManager.Import(QString((g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\ColorCubeNames.world").c_str()));

	CreateSkyBox();

	m_cameraEntity = m_world.GetEntityManager()->CreateEntity();
	RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(m_cameraEntity);
	RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(m_cameraEntity);
	cameraTransform->m_position = glm::vec3(0);
	camera->m_frustum.m_near = 0.1f;
	camera->m_frustum.m_far = 1000.0f;
	camera->m_frustum.m_fov = 45.0f;

	m_world.GetTagManager()->RegisterEntity("Camera", m_cameraEntity);
	m_world.GetGroupManager()->RegisterEntity("NonExport", m_cameraEntity);	

	g_engineContext.m_renderer->SetAmbientLight(glm::vec4(1,1,1,1));

	// Display treenity editor.
	m_treenityEditor.show();
}

TreenityMain::~TreenityMain()
{
	delete m_renderingSystem;
	delete m_cameraSystem;
	delete m_transformInterpolationSystem;

	SDL_Quit();

	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

bool TreenityMain::IsRunning()
{
	return m_treenityEditor.IsRunning();
}

void TreenityMain::HandleEvents()
{
	if (g_engineContext.m_inputSys != nullptr)
		g_engineContext.m_inputSys->Reset();

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if (g_engineContext.m_inputSys != nullptr)
			g_engineContext.m_inputSys->HandleInput(event);
	}
}

void TreenityMain::Update(float dt)
{
	m_world.SetDelta(dt);

	HandleEvents();

	auto msgs = m_world.GetMessages();
	for(auto itr = msgs.begin(); itr != msgs.end(); ++itr) 
	{
		switch (itr->m_type)
		{
			case ECS::MessageType::ENTITY_ADDED:
			{
				m_projectManager.EntityAdded(itr->m_entity);
				m_treenityEditor.EntityCreated(itr->m_entity);
			} break;

			case ECS::MessageType::ENTITY_REMOVED:
			{
				m_projectManager.EntityRemoved(itr->m_entity);
				m_treenityEditor.EntityRemoved(itr->m_entity);
			} break;

			case ECS::MessageType::COMPONENT_ADDED:
			{
				m_treenityEditor.ComponentCreated(itr->m_entity, itr->m_compType);
			} break;
			
			case ECS::MessageType::COMPONENT_REMOVED:
			{
				m_treenityEditor.ComponentRemoved(itr->m_entity, itr->m_compType);
			} break;

			case ECS::MessageType::TAG_ADDED:
			{
				m_treenityEditor.TagAdded(itr->m_entity, itr->m_tagGroupName);
			} break;

			case ECS::MessageType::TAG_REMOVED:
			{
				m_treenityEditor.TagRemoved(itr->m_entity, itr->m_tagGroupName);
			} break;

			case ECS::MessageType::ENTITY_ADDED_TO_GROUP:
			{
				m_treenityEditor.EntityAddedToGroup(itr->m_entity, itr->m_tagGroupName);
			} break;

			case ECS::MessageType::ENTITY_REMOVED_FROM_GROUP:
			{
				m_treenityEditor.EntityRemovedFromGroup(itr->m_entity, itr->m_tagGroupName);
			} break;
		}

		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Message Type %d - Entity ID: %d - Component Type: %d", itr->m_type, itr->m_entity->GetId(), itr->m_compType);
	}

	m_world.GetEntityManager()->CleanUp();

	g_engineContext.m_renderer->Clear();

	m_cameraSystem->Process();
	m_transformInterpolationSystem->Process();
	m_renderingSystem->Process();
	
	g_engineContext.m_renderer->Render();
	g_engineContext.m_renderer->Swap();
}

void TreenityMain::CreateSkyBox()
{
	// Setup skybox entity.
	m_skyBox = m_world.GetEntityManager()->CreateEntity();

	RootForce::Renderable* r = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(m_skyBox);
	RootForce::Transform* t = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(m_skyBox);

	t->m_scale = glm::vec3(-100);
	t->m_orientation.Roll(180);

	static glm::vec3 positions[8] = 
	{
		glm::vec3( -0.500000, -0.500000, 0.500000),
		glm::vec3(0.500000, -0.500000, 0.500000),
		glm::vec3(-0.500000, 0.500000, 0.500000),
		glm::vec3(0.500000, 0.500000, 0.500000),
		glm::vec3(-0.500000, 0.500000, -0.500000),
		glm::vec3(0.500000, 0.500000, -0.500000),
		glm::vec3(-0.500000, -0.500000, -0.500000),
		glm::vec3(0.500000, -0.500000, -0.500000)
	};

	static unsigned int indices[36] =
	{
		0, 1, 2, 
		2, 1, 3, 
		2, 3, 4, 
		4, 3, 5, 
		4, 5, 6, 
		6, 5, 7,
		6, 7, 0, 
		0, 7, 1, 
		1, 7, 3, 
		3, 7, 5, 
		6, 0, 4, 
		4, 0, 2
	};

	Render::Vertex1P vertices[8];
	for(int i = 0; i < 8; ++i)
	{
		vertices[i].m_pos = positions[i];
	}

	r->m_model = g_engineContext.m_resourceManager->CreateModel("skybox");
	r->m_model->m_meshes[0]->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
	r->m_model->m_meshes[0]->SetElementBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));
	r->m_model->m_meshes[0]->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
	r->m_model->m_meshes[0]->CreateVertexBuffer1P(&vertices[0], 8);
	r->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], 36);

	r->m_pass = RootForce::RenderPass::RENDERPASS_SKYBOX;
	r->m_renderFlags = Render::RenderFlags::RENDER_IGNORE_CASTSHADOW;
	r->m_material = g_engineContext.m_renderer->CreateMaterial("skybox");
	r->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Skybox");
	r->m_material->m_textures[Render::TextureSemantic::DIFFUSE] =  g_engineContext.m_resourceManager->LoadTexture("SkyBox", Render::TextureType::TEXTURE_CUBEMAP);

	m_world.GetTagManager()->RegisterEntity("Skybox", m_skyBox);
	m_world.GetGroupManager()->RegisterEntity("NonExport", m_skyBox);
}
