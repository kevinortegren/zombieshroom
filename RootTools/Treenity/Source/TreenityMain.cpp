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
#include <QStyleFactory>

#include <RootForce/Include/LuaAPI.h>
#include <RootEngine/Script/Include/ScriptManager.h>

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>

#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootTools/Treenity/Include/Log.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;
RootForce::Network::NetworkEntityMap g_networkEntityMap;
RootForce::Network::DeletedNetworkEntityList g_networkDeletedList;

#undef main
int main(int argc, char *argv[])
{
	std::string path(argv[0]);
	std::string rootforcename = "Treenity.exe";
	path = path.substr(0, path.size() - rootforcename.size());

	QApplication a(argc, argv);
	
	a.setStyle(QStyleFactory::create("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(53,53,53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(15,15,15));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53,53,53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(255,140,0));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	qApp->setPalette(darkPalette);

	qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");



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
	: m_engineActions(&m_world, this)
	, m_projectManager(&m_world)
	, m_previousEditorMode(EditorMode::EDITOR)
{
	g_world = &m_world;

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

	g_engineContext = libInitializeEngine(
		RootEngine::SubsystemInit::INIT_ALL, p_path);

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_TIMER) != 0) 
	{
		throw std::runtime_error("Failed to initialize SDL");
	}
	
	//Init Treenity QMainWindow and all UIs
    m_treenityEditor.Init();

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

	// Bind c++ functions and members to Lua.
	RootForce::LuaAPI::RegisterLuaTypes(g_engineContext.m_script->GetLuaState());

	g_engineContext.m_resourceManager->LoadScript("Global");
	g_engineContext.m_resourceManager->LoadScript("TimerEntity");
	g_engineContext.m_resourceManager->LoadScript("Push");
	g_engineContext.m_resourceManager->LoadScript("Identiray");
	g_engineContext.m_resourceManager->LoadScript("Player");
	g_engineContext.m_resourceManager->LoadScript("Explosion");
	g_engineContext.m_resourceManager->LoadScript("AbilitySpawnPoint");
	g_engineContext.m_resourceManager->LoadScript("XplodingMushroomPlanted");
	g_engineContext.m_resourceManager->LoadScript("ShroomExplosion");
	g_engineContext.m_resourceManager->LoadScript("TotemProjectile");

	// Initialize the systems.
	m_matchStateSystem = std::shared_ptr<RootForce::MatchStateSystem>(new RootForce::MatchStateSystem(g_world, &g_engineContext));

	m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(g_world));
	m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
	m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
	m_playerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

	m_worldSystem = std::shared_ptr<RootForce::WorldSystem>(new RootForce::WorldSystem(g_world, &g_engineContext));

	m_deserializationSystem = std::shared_ptr<RootForce::DeserializationSystem>(new RootForce::DeserializationSystem(g_world));

	m_abilitySpawnSystem = new RootForce::AbilitySpawnSystem(g_world, &g_engineContext, g_engineContext.m_resourceManager->GetWorkingDirectory());
	g_world->GetSystemManager()->AddSystem<RootForce::AbilitySpawnSystem>(m_abilitySpawnSystem);

	m_respawnSystem = new RootSystems::RespawnSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootSystems::RespawnSystem>(m_respawnSystem);

	m_controllerActionSystem = new RootForce::ControllerActionSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::ControllerActionSystem>(m_controllerActionSystem);

	m_physicsTransformCorrectionSystem = new RootForce::PhysicsTransformCorrectionSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::PhysicsTransformCorrectionSystem>(m_physicsTransformCorrectionSystem);

	m_physicsSystem = new RootForce::PhysicsSystem(g_world);
	m_physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
	m_physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
	g_world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem);

	m_scriptSystem = new RootForce::ScriptSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::ScriptSystem>(m_scriptSystem);

	m_collisionSystem = new RootForce::CollisionSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::CollisionSystem>(m_collisionSystem);

	m_shadowSystem = new RootForce::ShadowSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::ShadowSystem>(m_shadowSystem);

	m_renderingSystem = new RootForce::RenderingSystem(&m_world);
	m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);
	m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
	g_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem);

	m_pointLightSystem = new RootForce::PointLightSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem);

	m_particleSystem = new RootForce::ParticleSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem);

	m_cameraSystem = new RootForce::CameraSystem(&m_world, &g_engineContext);
	m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem);

	m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem);

	m_thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(m_thirdPersonBehaviorSystem);

	m_animationSystem = new RootForce::AnimationSystem(g_world);
	m_animationSystem->SetLoggingInterface(g_engineContext.m_logger);
	m_animationSystem->SetGameSharedContext(&g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::AnimationSystem>(m_animationSystem);

	m_ragdollSystem = new RootForce::RagdollSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::RagdollSystem>(m_ragdollSystem);

	m_waterSystem = new RootForce::WaterSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::WaterSystem>(m_waterSystem);

	m_actionSystem = new RootSystems::ActionSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootSystems::ActionSystem>(m_actionSystem);

	m_stateSystem = new RootSystems::StateSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootSystems::StateSystem>(m_stateSystem);

	m_directionalLightSystem = new RootForce::DirectionalLightSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::DirectionalLightSystem>(m_directionalLightSystem);

	m_tryPickupResetSystem = new RootForce::TryPickupResetSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::TryPickupResetSystem>(m_tryPickupResetSystem);

	m_soundSystem = new RootForce::SoundSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::SoundSystem>(m_soundSystem);

	m_botanySystem = new RootForce::BotanySystem(g_world, &g_engineContext);

	m_timerSystem = new RootForce::TimerSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::TimerSystem>(m_timerSystem);

	m_followSystem = new RootForce::FollowSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::FollowSystem>(m_followSystem);

	m_homingSystem = new RootForce::HomingSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::HomingSystem>(m_homingSystem);

	m_raySystem = new RootForce::RaySystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::RaySystem>(m_raySystem);

	m_transformInterpolationSystem = new RootForce::TransformInterpolationSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::TransformInterpolationSystem>(m_transformInterpolationSystem);

	m_networkDebugSystem = new RootForce::Network::NetworkDebugSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::Network::NetworkDebugSystem>(m_networkDebugSystem);

	m_waterDeathSystem = new RootForce::WaterDeathSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::WaterDeathSystem>(m_waterDeathSystem);

	m_scaleSystem = new RootForce::ScaleSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::ScaleSystem>(m_scaleSystem);

	// Set the importer/exporter
	m_world.GetEntityImporter()->SetImporter(Importer);
	m_world.GetEntityExporter()->SetExporter(Exporter);

	m_treenityEditor.CreateNewScene();

	// Display treenity editor.
	m_treenityEditor.show();

	g_engineContext.m_inputSys->LockMouseToCenter(false);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	// Selected mesh material.
	m_selectedEntityMaterial = g_engineContext.m_renderer->CreateMaterial("SelectedMaterial");
	m_selectedEntityMaterial->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_Selected");

	// Register listeners for global modifer keys.
	GlobalKeys::InitializeKeyMap();

	m_globalKeys.RegisterModifier(Qt::AltModifier);
	m_globalKeys.RegisterModifier(Qt::ShiftModifier);

}

TreenityMain::~TreenityMain()
{
	delete m_renderingSystem;
	delete m_cameraSystem;
	delete m_transformInterpolationSystem;
	delete m_scriptSystem;
	delete m_controllerActionSystem;
	delete m_lookAtSystem;
	delete m_shadowSystem;
	delete m_pointLightSystem;
	delete m_directionalLightSystem;

	SDL_Quit();

	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

bool TreenityMain::IsRunning()
{
	return m_treenityEditor.IsRunning();
}

void TreenityMain::EnterPlayMode()
{
	
}

void TreenityMain::ExitPlayMode()
{

}

void TreenityMain::HandleEditorEvents()
{
	m_globalKeys.Update();

	if (g_engineContext.m_inputSys != nullptr)
	{
		g_engineContext.m_inputSys->Reset();
		g_engineContext.m_inputSys->SetMousePos(glm::ivec2(QCursor::pos().x(), QCursor::pos().y()));
	}

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		// Ignore mouse motions events while in editor mode.
		if(event.type == SDL_MOUSEMOTION)
			continue;

		if (g_engineContext.m_inputSys != nullptr)
			g_engineContext.m_inputSys->HandleInput(event);
	}
}

void TreenityMain::HandleIngameEvents()
{
	//m_globalKeys.Update();

	if (g_engineContext.m_inputSys != nullptr)
	{
		g_engineContext.m_inputSys->Reset();
	}

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if (g_engineContext.m_inputSys != nullptr)
			g_engineContext.m_inputSys->HandleInput(event);
	}
}

void TreenityMain::ProcessWorldMessages()
{
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
}

void TreenityMain::Update(float dt)
{
	if (m_previousEditorMode == EditorMode::EDITOR && m_engineActions.GetMode() == EditorMode::GAME)
	{
		// Reset the time the loading for going in-game took.
		dt = 0.0f;
	}
	m_previousEditorMode = m_engineActions.GetMode();


	if (m_engineActions.GetMode() == EditorMode::EDITOR)
	{
		m_world.SetDelta(dt);

		HandleEditorEvents();

		ProcessWorldMessages();
		m_world.GetEntityManager()->CleanUp();

		m_worldSystem->Process();
		m_controllerActionSystem->Process();	
		m_lookAtSystem->Process();
		m_cameraSystem->Process();
		m_scriptSystem->Process();
		m_transformInterpolationSystem->Process();
		m_shadowSystem->Process();
		m_directionalLightSystem->Process();
		m_pointLightSystem->Process();
		m_renderingSystem->Process();
		
		if (!g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_LALT))
		RaySelect();

		RenderSelectedEntity();
		
		//g_engineContext.m_renderer->AddLine(debugCameraPos, debugCameraPos + (debugRay * 500.0f), glm::vec4(1,0,0,1));

		g_engineContext.m_renderer->Clear();
		g_engineContext.m_renderer->Render();
		g_engineContext.m_renderer->Swap();
	}
	else if (m_engineActions.GetMode() == EditorMode::GAME)
	{
		m_world.SetDelta(dt);

		HandleIngameEvents();

		ProcessWorldMessages();
		m_world.GetEntityManager()->CleanUp();

		// Update on player controls.
		m_playerControlSystem->Process();
		m_actionSystem->Process();

		// Update the physics.
		m_physicsTransformCorrectionSystem->Process();
		g_engineContext.m_physics->Update(m_world.GetDelta());
		m_physicsSystem->Process();

		// Update collision
		//m_collisionSystem->Process();

		// Update the scripts?
		//m_controllerActionSystem->Process();	
		//m_scriptSystem->Process();

		// Update the rendering.
		m_worldSystem->Process();
		m_actionSystem->UpdateAimingDevice(false);
		m_thirdPersonBehaviorSystem->Process();
		m_lookAtSystem->Process();
		m_cameraSystem->Process();
		m_transformInterpolationSystem->Process();
		m_shadowSystem->Process();
		m_directionalLightSystem->Process();
		m_pointLightSystem->Process();
		m_renderingSystem->Process();

		g_engineContext.m_renderer->Clear();
		g_engineContext.m_renderer->Render();
		g_engineContext.m_renderer->Swap();
	}
}

void TreenityMain::RenderSelectedEntity()
{
	for(auto itr = m_treenityEditor.GetSelection().begin(); itr != m_treenityEditor.GetSelection().end(); ++itr)
	{
		ECS::Entity* entity = (*itr);

		RootForce::Transform* transform = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(entity);
		RootForce::Renderable* renderable = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(entity);

		if(renderable == nullptr)
			continue;
		if(renderable->m_model == nullptr)
			continue;
		if(renderable->m_material == nullptr)
			continue;

		m_renderingSystem->m_matrices[entity].m_model = glm::translate(glm::mat4(1.0f), transform->m_interpolatedPosition);
		m_renderingSystem->m_matrices[entity].m_model = glm::rotate(m_renderingSystem->m_matrices[entity].m_model, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
		m_renderingSystem->m_matrices[entity].m_model = glm::scale(m_renderingSystem->m_matrices[entity].m_model, transform->m_scale);
		m_renderingSystem->m_matrices[entity].m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_renderingSystem->m_matrices[entity].m_model))));

		Render::RenderJob job;	
		job.m_mesh = renderable->m_model->m_meshes[0];
		job.m_material = m_selectedEntityMaterial;
		job.m_forward = renderable->m_forward;
		job.m_refractive = renderable->m_refractive;
		job.m_params[Render::Semantic::MODEL] = &m_renderingSystem->m_matrices[entity].m_model;
		job.m_renderPass = RootForce::RenderPass::RENDERPASS_EDITOR;
		job.m_position = transform->m_interpolatedPosition;

		g_engineContext.m_renderer->AddRenderJob(job);
		
		Debug(&renderable->m_model->m_obb, m_renderingSystem->m_matrices[entity].m_model, glm::vec3(0,1,0));

	}
}

void TreenityMain::RaySelect()
{
    if(g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::LEFT) == RootEngine::InputManager::KeyState::DOWN_EDGE)
    {
        // Get camera entity
        ECS::Entity* cameraEntity = m_world.GetTagManager()->GetEntityByTag("Camera"); 
        glm::vec3 cameraPos = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(cameraEntity)->m_position;
 
        // Construct ray.
        const glm::vec3& ray = glm::normalize(ConstructRay());
 
        debugRay = ray;
        debugCameraPos = cameraPos;
 
        float closestDist = 999999.0f;
        ECS::Entity* closestEntity = nullptr;
 
        std::vector<ECS::Entity*> entities = m_world.GetEntityManager()->GetAllEntities();
        for(auto itr = entities.begin(); itr != entities.end(); ++itr)
        {
            if((*itr)->GetFlag() == 0)
                continue;
 
            if(m_world.GetTagManager()->GetEntityByTag("Skybox") == (*itr))
                continue;
 
            if(m_world.GetTagManager()->GetEntityByTag("Camera") == (*itr))
                continue;
 
            if(m_world.GetTagManager()->GetEntityByTag("AimingDevice") == (*itr))
                continue;
 
            glm::vec3 entityPos = m_world.GetEntityManager()->GetComponent<RootForce::Transform>((*itr))->m_position;
 
            glm::mat4x4 transform = m_renderingSystem->m_matrices[(*itr)].m_model;
 
            RootForce::Renderable* renderable = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>((*itr));
            if(renderable != nullptr)
            {
                float t = 999999.0f;
                if(RayVsOBB(cameraPos, ray, &renderable->m_model->m_obb, transform, t))
                {
                    float newt = 9999999.0f;
                    if(RayVsTriangle(cameraPos, ray, renderable->m_model, transform, newt))
                    {
                        if(newt < closestDist)
                        {
                            closestEntity = (*itr);
                            closestDist = newt;
                            g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "OBB hit on %d", (*itr)->GetId());
                        }
                    }
                }
            }
            else
            {
                static float radius = 5.0f;
 
                float t = 999999.0f;
                RayVsSphere(cameraPos, ray, entityPos, radius, t);
 
                if(t < closestDist)
                {
                    closestEntity = (*itr);
                    closestDist = t;
                    g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Sphere hit on %d", (*itr)->GetId());
 
                }
            }
        }
 
        if(closestEntity != nullptr)
        {
            g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Ray Hit Entity %d", closestEntity->GetId());
 
            if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_LSHIFT))
            {
                m_treenityEditor.AddToSelection(closestEntity);
            }
            else
            {
                m_treenityEditor.Select(closestEntity);
            }
        }
        else
        {
            m_treenityEditor.Select(nullptr);
        }
    }
}

const glm::vec3& TreenityMain::ConstructRay()
{
 
    // Get mouse pos relative to window
    glm::ivec2 position;
    SDL_GetMouseState(&position.x, &position.y);
 
    // Calculate NDC coords
    float x = (2.0f * position.x) / (float)g_engineContext.m_renderer->GetWidth() - 1.0f;
    float y = (2.0f * -position.y) / (float)g_engineContext.m_renderer->GetHeight() + 1.0f;
 
    glm::mat4 inverseProjection = glm::inverse(g_engineContext.m_renderer->GetProjectionMatrix());
    glm::mat4 inverseView = glm::inverse(g_engineContext.m_renderer->GetViewMatrix());
 
    // View space coords
    glm::vec4 rayView = inverseProjection * glm::vec4(x, y, -1.0f, 1.0f);
    rayView.z = -1.0f;
    rayView.w = 0.0f;
 
    // World space coords
    glm::vec4 rW = inverseView * rayView;
    const glm::vec3& rayWorld = glm::normalize(glm::vec3(rW.x, rW.y, rW.z));
 
    return rayWorld;
}

bool TreenityMain::RayVsSphere(const glm::vec3& cameraPos, const glm::vec3& ray, const glm::vec3& center, float radius, float& t)
{
    glm::vec3 entityPos = center;
    glm::vec3 direction = entityPos - cameraPos;
    
    float radiusSq = radius*radius;
 
    float tca = glm::dot(direction, ray);
    if(tca < 0)
    {
        return false;
    }
    float d2 = glm::dot(direction, direction) - tca * tca;
    if(d2 > radiusSq)
    {
        return false;
    }
    float thc = glm::sqrt(radiusSq - d2);
    float t0 = tca - thc;
 
    t = t0;
 
    return true;
}

bool TreenityMain::RayVsOBB(const glm::vec3& cameraPos, const glm::vec3& ray, RootEngine::OBB* obb, const glm::mat4x4& transform, float& t)
{
    float tMin = -99999.9f;
    float tMax = 99999.9f;
 
    // OBB Positions.
    glm::vec4 positions[8];
    positions[0] = transform * glm::vec4(obb->m_minX, obb->m_minY, obb->m_minZ, 1.0f);
    positions[1] = transform * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_minZ, 1.0f);
    positions[2] = transform * glm::vec4(obb->m_minX, obb->m_minY, obb->m_maxZ, 1.0f);
    positions[3] = transform * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_maxZ, 1.0f);
    positions[4] = transform * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_minZ, 1.0f);
    positions[5] = transform * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_minZ, 1.0f);
    positions[6] = transform * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_maxZ, 1.0f);
    positions[7] = transform * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_maxZ, 1.0f);
 
    glm::vec4 normalX = positions[1] - positions[0];
    glm::vec4 normalY = positions[4] - positions[0];
    glm::vec4 normalZ = positions[2] - positions[0];
 
    glm::vec4 center = positions[0] + (normalX / 2) + (normalY / 2) + (normalZ / 2); 
 
    glm::vec3 normals[3];
    normals[0] = glm::normalize(glm::vec3(normalX.x, normalX.y, normalX.z));
    normals[1] = glm::normalize(glm::vec3(normalY.x, normalY.y, normalY.z));
    normals[2] = glm::normalize(glm::vec3(normalZ.x, normalZ.y, normalZ.z));
 
    float distances[3];
    distances[0] = glm::length(normalX) / 2;
    distances[1] = glm::length(normalY) / 2;
    distances[2] = glm::length(normalZ) / 2;
 
    glm::vec3 direction = glm::vec3(center.x, center.y, center.z) - cameraPos;
 
    for(int i=0; i<3; i++)
    {
        float e = glm::dot(normals[i], direction);
        float f = glm::dot(normals[i], ray);
 
        if(abs(f) > 0.000001f)
        {
            float t1 = (e + distances[i]) / f;
            float t2 = (e - distances[i]) / f;
 
            if(t1>t2)
            {
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }
 
            if(t1 > tMin) tMin = t1;
            if(t2 < tMax) tMax = t2;
 
            if(tMin > tMax) return false;
            if(tMax < 0) return false;
        }
 
        else if(-e-distances[i] > 0 || -e+distances[i] < 0) return false;     
    }
 
    if(tMin > 0) 
 
    {
        t = tMin;
        return true;
    }
    else
    {
        t = tMax;
        return true;
    }
 
    return false;
}
 
bool TreenityMain::RayVsTriangle(const glm::vec3& cameraPos, const glm::vec3& ray, const RootEngine::Model* model, const glm::mat4x4& transform, float& t)
{
    std::cout << model->m_indices.size() << std::endl;
    std::cout << model->m_positions.size() << std::endl;
 
    glm::mat4x4 inverseWorld = glm::inverse(transform);
 
    glm::vec4 rayLocal = inverseWorld * glm::vec4(ray.x, ray.y, ray.z, 0.0f);
    glm::vec4 cameraLocal = inverseWorld * glm::vec4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
 
    for(size_t i = 0; i < model->m_indices.size(); i += 3)
    {
        glm::vec3 result;
 
        glm::vec3 A = model->m_positions[model->m_indices[i]];
        glm::vec3 B = model->m_positions[model->m_indices[i+1]];
        glm::vec3 C = model->m_positions[model->m_indices[i+2]];
 
        glm::vec3 e1 = B - A;
        glm::vec3 e2 = C - A;
 
        glm::vec3 q = glm::cross(glm::vec3(rayLocal.x, rayLocal.y, rayLocal.z), e2);
        float a = glm::dot(e1, q);
        float f = 1.0f / a;
 
        glm::vec3 s = glm::vec3(cameraLocal.x, cameraLocal.y, cameraLocal.z) - A;
        result.x = f * glm::dot(s, q);
 
        glm::vec3 j = glm::cross(s, e1);
        result.y = f * glm::dot(glm::vec3(rayLocal.x, rayLocal.y, rayLocal.z), j);
        result.z = f * glm::dot(e2, j);
 
        if(result.x > 0 && result.x < 1.0f && result.y > 0 && result.y < 1.0f && (result.x + result.y) > 0 && (result.x + result.y) < 1.0f)
        {
            t = result.z;
            return true;
        }
    }
 
    return false;
}
 
void TreenityMain::Debug(RootEngine::OBB* obb, const glm::mat4x4& p_space, const glm::vec3& p_color)
{
    glm::vec4 positions[8];
    positions[0] = p_space * glm::vec4(obb->m_minX, obb->m_minY, obb->m_minZ, 1.0f);
    positions[1] = p_space * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_minZ, 1.0f);
    positions[2] = p_space * glm::vec4(obb->m_minX, obb->m_minY, obb->m_maxZ, 1.0f);
    positions[3] = p_space * glm::vec4(obb->m_maxX, obb->m_minY, obb->m_maxZ, 1.0f);
    positions[4] = p_space * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_minZ, 1.0f);
    positions[5] = p_space * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_minZ, 1.0f);
    positions[6] = p_space * glm::vec4(obb->m_minX, obb->m_maxY, obb->m_maxZ, 1.0f);
    positions[7] = p_space * glm::vec4(obb->m_maxX, obb->m_maxY, obb->m_maxZ, 1.0f);
 
    unsigned int indices[] = 
    { 
        0, 2, 2, 3, 3, 1, 1, 0,
        4, 6, 6, 7, 7, 5, 5, 4,
        0, 4, 2, 6, 3, 7, 1, 5 
    };
 
    for(int i = 0; i < 24; i += 2)
    {
        glm::vec3 pos1, pos2;
        pos1 = glm::swizzle<glm::X, glm::Y, glm::Z>(positions[indices[i]]);
        pos2 = glm::swizzle<glm::X, glm::Y, glm::Z>(positions[indices[i+1]]);
        g_engineContext.m_renderer->AddLine(pos1, pos2, glm::vec4(p_color.x, p_color.y, p_color.z, 1.0f));
    }
}