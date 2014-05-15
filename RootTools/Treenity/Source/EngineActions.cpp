#include <RootTools/Treenity/Include/EngineActions.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/CollisionSystem.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/Script.h>
#include <RootSystems/Include/CameraSystem.h>
#include <RootSystems/Include/ControllerActions.h>

#include <sstream>

#include <Utility/ECS/Include/World.h>
#include <RootEngine/Script/Include/ScriptManager.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootTools/Treenity/Include/TreenityMain.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

#include <RootTools/Treenity/Include/Utils.h>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

EngineActions::EngineActions(ECS::World* p_world, TreenityMain* p_treenityMain)
	: m_world(p_world), m_treenityMain(p_treenityMain), m_editorMode(EditorMode::EDITOR)
{

}

void EngineActions::NewScene()
{
	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Existing entities: %d", m_world->GetEntityManager()->GetNumEntities());

	ClearScene();

	AddDefaultEntities();
}

void EngineActions::ClearScene()
{
	m_treenityMain->GetEditor()->ClearSelection();
	m_world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	m_world->GetTagManager()->UnregisterAll();
	m_world->GetGroupManager()->UnregisterAll();
	m_world->GetSystemManager()->Clear();
	m_world->GetStorage()->ClearStorage();
	m_treenityMain->ProcessWorldMessages();
	m_world->GetEntityManager()->CleanUp();
	m_world->GetEntityManager()->ClearRecyledEntitiesStack();
}

void EngineActions::AddDefaultEntities()
{
	m_treenityMain->GetEditor()->InitializeTools(m_world);

	// Process entities from world import.
    m_treenityMain->ProcessWorldMessages();
 
    // Add non-editable entities.
	ECS::Entity* skybox = m_treenityMain->GetWorldSystem()->CreateSkyBox();

	CreateFreeFlyingCamera();
	
	m_world->GetEntityManager()->CleanUp();

	// Add editable entities.
    ECS::Entity* sun = m_treenityMain->GetWorldSystem()->CreateSun();
	CreateTestSpawnpoint();
	CreateWater();

	m_treenityMain->ProcessWorldMessages();
	m_world->GetEntityManager()->CleanUp();
	
	m_treenityMain->GetEditor()->RenameEntity(sun, "Sun");
	m_treenityMain->GetEditor()->RenameEntity(m_testSpawnpoint, "Test Spawnpoint");
	m_treenityMain->GetEditor()->RenameEntity(m_water, "Water");
}

// Can only be called after a world has been imported !!
void EngineActions::InitializeScene()
{
	m_treenityMain->GetWorldSystem()->BuildStaticShadowMesh();
	m_treenityMain->GetWorldSystem()->SetAmbientLight(m_world->GetStorage()->GetValueAsVec4("Ambient"));

	//m_treenityMain->GetWorldSystem()->CalculateWorldAABB();
	//m_treenityMain->GetShadowSystem()->SetQuadTree(m_treenityMain->GetWorldSystem()->GetQuadTree());

	// Add new entities.
	m_treenityMain->ProcessWorldMessages();
	m_world->GetEntityManager()->CleanUp();
}

void EngineActions::LoadScene( const QString& p_filePath )
{
	
	QProgressDialog prog( "Loading", "", 0, 0, nullptr,  0);
	prog.setCancelButton(0);
	prog.show();
	qApp->processEvents(QEventLoop::AllEvents);

    ClearScene();
    //Utils::RunWithProgressBar(QtConcurrent::run(m_treenityMain->GetProjectManager(), &ProjectManager::Import, p_filePath));
    m_treenityMain->GetProjectManager()->Import(p_filePath);
    AddDefaultEntities();
    InitializeScene();
	m_treenityMain->GetEditor()->GetUi().statusBar->showMessage("Level loaded: " + p_filePath, 5000);
}

void EngineActions::CreateFreeFlyingCamera()
{
	m_cameraEntity = m_world->GetEntityManager()->CreateEntity();

	// Setup camera entity.
	RootForce::Camera* camera = m_world->GetEntityManager()->CreateComponent<RootForce::Camera>(m_cameraEntity);
	RootForce::Transform* cameraTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_cameraEntity);
	RootForce::ControllerActions* controllerActions = m_world->GetEntityManager()->CreateComponent<RootForce::ControllerActions>(m_cameraEntity);
	RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world->GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(m_cameraEntity);

	RootForce::LookAtBehavior* cameraLookAt = m_world->GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(m_cameraEntity);
	cameraLookAt->m_targetTag = "AimingDevice";
	cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);

	cameraThirdPerson->m_targetTag = "AimingDevice";
	cameraThirdPerson->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraThirdPerson->m_distance = 10.0f;

	RootForce::Script* script = m_world->GetEntityManager()->CreateComponent<RootForce::Script>(m_cameraEntity);
	script->Name = g_engineContext.m_resourceManager->LoadScript("FreeFlyingMaya");
	
	g_engineContext.m_script->SetFunction(script->Name, "Setup");
	g_engineContext.m_script->AddParameterUserData(m_cameraEntity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->ExecuteScript();

	cameraTransform->m_position = glm::vec3(0);
	camera->m_frustum.m_near = 0.1f;
	camera->m_frustum.m_far = 5000.0f;
	camera->m_frustum.m_fov = 45.0f;

	m_world->GetTagManager()->RegisterEntity("Camera", m_cameraEntity);
	m_world->GetGroupManager()->RegisterEntity("NonExport", m_cameraEntity);	

	// Setup aiming device.
	m_aimingDevice = m_world->GetEntityManager()->CreateEntity();

	RootForce::Transform* aimingDeviceTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_aimingDevice);

	m_world->GetTagManager()->RegisterEntity("AimingDevice", m_aimingDevice);
	m_world->GetGroupManager()->RegisterEntity("NonExport", m_aimingDevice);
}

void EngineActions::CreateTestSpawnpoint()
{
	m_testSpawnpoint = m_world->GetTagManager()->GetEntityByTag("TestSpawnpoint");
	if (m_testSpawnpoint == nullptr)
	{
		m_testSpawnpoint = m_world->GetEntityManager()->CreateEntity();

		RootForce::Transform* transform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_testSpawnpoint);
		RootForce::Renderable* renderable = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(m_testSpawnpoint);

		renderable->m_material = g_engineContext.m_renderer->CreateMaterial("TestSpawnpoint");
		renderable->m_model = g_engineContext.m_resourceManager->LoadCollada("testchar");
		renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture("WStexture", Render::TextureType::TextureType::TEXTURE_2D);
		renderable->m_material->m_textures[Render::TextureSemantic::GLOW] = g_engineContext.m_resourceManager->LoadTexture("WSGlowRed", Render::TextureType::TextureType::TEXTURE_2D);
		renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] = g_engineContext.m_resourceManager->LoadTexture("WSSpecular", Render::TextureType::TextureType::TEXTURE_2D);
		renderable->m_material->m_textures[Render::TextureSemantic::NORMAL] = g_engineContext.m_resourceManager->LoadTexture("WSNormal", Render::TextureType::TextureType::TEXTURE_2D);
		renderable->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_NormalMap");

		//m_world->GetGroupManager()->RegisterEntity("NonExport", m_testSpawnpoint);
		m_world->GetTagManager()->RegisterEntity("TestSpawnpoint", m_testSpawnpoint);
	}

	// Add a shadow technique to the spawn point (which is not exported).
	RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_testSpawnpoint);
	renderable->m_shadowTech = Render::ShadowTechnique::SHADOW_DYNAMIC;
}

void EngineActions::CreateWater()
{
	m_water = m_world->GetTagManager()->GetEntityByTag("Water");
	if (m_water == nullptr)
	{
		m_water = m_treenityMain->GetWaterSystem()->CreateDefaultWater();
	}

	m_treenityMain->GetWaterSystem()->CreateWater();
}

// Mode switching
void EngineActions::EnterPlayMode()
{
	m_editorMode = EditorMode::GAME;
	g_engineContext.m_inputSys->LockMouseToCenter(true);

	// Get the spawn position/orientation.
	RootForce::Transform* spawnTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("TestSpawnpoint"));

	Utils::RunWithProgressBar(QtConcurrent::run(this, &EngineActions::ParallelPlayModeEnter));

	// Create a player.
	g_engineContext.m_script->SetGlobalNumber("UserID", 0);
	g_engineContext.m_script->SetGlobalBoolean("IsClient", true);

	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnCreate");
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::CONNECT);
	g_engineContext.m_script->ExecuteScript();

	ECS::Entity* playerEntity = m_world->GetTagManager()->GetEntityByTag("Player");

	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnTeamSelect");
	g_engineContext.m_script->AddParameterUserData(playerEntity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->AddParameterNumber(2);
	g_engineContext.m_script->ExecuteScript();

	// Set the player's position to the test spawnpoint.
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(playerEntity);
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(playerEntity);
	transform->m_position = spawnTransform->m_position;
	transform->m_orientation = spawnTransform->m_orientation;
	g_engineContext.m_physics->SetPosition(*collision->m_handle, transform->m_position);
	g_engineContext.m_physics->SetOrientation(*collision->m_handle, transform->m_orientation.GetQuaternion());

	// Startup the animation system.
	m_treenityMain->GetAnimationSystem()->Start();

	// Process the messages.
	m_treenityMain->ProcessWorldMessages();
	m_world->GetEntityManager()->CleanUp();
	
	// Focus the 3D canvas.
	m_treenityMain->GetEditor()->GetUi().widget_canvas3D->setFocus(Qt::FocusReason::NoFocusReason);

	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Entered play mode");	
}

void EngineActions::ExitPlayMode()
{
	QProgressDialog prog( "Loading", "", 0, 0, nullptr,  0);
	prog.setCancelButton(0);
	prog.show();
	qApp->processEvents(QEventLoop::AllEvents);

	// Stop the animation system.
	m_treenityMain->GetAnimationSystem()->Terminate();

	// Clear whatever happened within the game session.
	ClearScene();
	g_engineContext.m_physics->RemoveAll();
	g_networkEntityMap.clear();
	RootForce::Network::NetworkComponent::ResetSequenceForUser(0);

	// Restore the old world state.
	std::map<ECS::Entity*, std::string> entityNames;
	std::stringstream ss(m_editorLevelState);
	m_world->GetEntityImporter()->Import(ss, &entityNames);
	m_treenityMain->GetProjectManager()->SetEntityNames(entityNames);
	AddDefaultEntities();
	InitializeScene();

	//SDL_SetRelativeMouseMode(SDL_FALSE);
	g_engineContext.m_inputSys->LockMouseToCenter(false);
	m_editorMode = EditorMode::EDITOR;

	g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Exited play mode");
}

EditorMode::EditorMode EngineActions::GetMode()
{
	return m_editorMode;
}

void EngineActions::ParallelPlayModeEnter()
{
	// Save the current world state.
	m_editorLevelState = m_world->GetEntityExporter()->Export(&m_treenityMain->GetProjectManager()->GetEntityNames());

	// Remove the test spawnpoint, the editor camera and the editor spawnpoint.
	m_world->GetEntityManager()->RemoveEntity(m_cameraEntity);
	m_world->GetEntityManager()->RemoveEntity(m_aimingDevice);
	m_world->GetEntityManager()->RemoveEntity(m_testSpawnpoint);

	// Create a camera.
	m_treenityMain->GetWorldSystem()->CreatePlayerCamera();

	// Initialize the physics.
	m_treenityMain->GetWorldSystem()->AddStaticEntitiesToPhysics();
	g_engineContext.m_physics->EnableDebugDraw(true);
	
}

// Entity
ECS::Entity* EngineActions::CreateEntity()
{
	ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* transform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);

	return entity;
}

void EngineActions::TargetEntity(ECS::Entity* p_entity)
{
	ECS::Entity* cameraEntity = m_world->GetTagManager()->GetEntityByTag("Camera");
	RootForce::Script* script = m_world->GetEntityManager()->GetComponent<RootForce::Script>(cameraEntity);

	g_engineContext.m_script->SetFunction(script->Name, "Target");
	g_engineContext.m_script->AddParameterUserData(p_entity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->ExecuteScript();
}

void EngineActions::DeleteEntity(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveEntity(p_entity);
}

// Transform
void EngineActions::SetPosition(ECS::Entity* p_entity, const glm::vec3& p_position)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_position = p_position;
}

void EngineActions::TranslateEntity(ECS::Entity* p_entity, const glm::vec3& p_translation)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_position += p_translation;
}

void EngineActions::SetOrientation(ECS::Entity* p_entity, const RootForce::Orientation& p_orientation)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_orientation = p_orientation;
}

void EngineActions::SetScale(ECS::Entity* p_entity, const glm::vec3& p_scale)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	transform->m_scale = p_scale;
}

const glm::vec3& EngineActions::GetPosition(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity)->m_position;
}

RootForce::Orientation& EngineActions::GetOrientation(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity)->m_orientation;
}

const glm::vec3& EngineActions::GetScale(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity)->m_scale;
}

// Renderable
void EngineActions::AddRenderable(ECS::Entity* p_entity)
{
	RootForce::Renderable* renderable = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(p_entity);
	renderable->m_model = g_engineContext.m_resourceManager->LoadCollada("Primitives/box");
	renderable->m_material = g_engineContext.m_renderer->CreateMaterial("TestSpawnpoint");
}

void EngineActions::RemoveRenderable(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Renderable>(p_entity);
}

std::string EngineActions::GetRenderableModelName( ECS::Entity* p_entity )
{
	auto model = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(p_entity)->m_model;
	return g_engineContext.m_resourceManager->ResolveStringFromModel(model);
}

std::string EngineActions::GetRenderableMaterialName( ECS::Entity* p_entity )
{
	auto material = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(p_entity)->m_material;
	if(material)
	{
		return g_engineContext.m_renderer->GetStringFromMaterial(material);
	}
	return "";
}

void EngineActions::SetRenderableModelName( ECS::Entity* p_entity, std::string p_modelName )
{
	RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(p_entity);
	renderable->m_model = g_engineContext.m_resourceManager->LoadCollada(p_modelName);
}

void EngineActions::SetRenderableMaterialName( ECS::Entity* p_entity, std::string p_materialName )
{
	RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(p_entity);
	renderable->m_material = g_engineContext.m_renderer->CreateMaterial(p_materialName);
}

//Physics
float EngineActions::GetMass(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Physics>(p_entity)->m_mass;
}

glm::vec3& EngineActions::GetVelocity( ECS::Entity* p_entity )
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Physics>(p_entity)->m_velocity;
}

void EngineActions::SetMass( ECS::Entity* p_entity, float p_mass)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);

	if(collision->m_handle == nullptr)
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::FATAL_ERROR, "EngineActions: No collision component! Cannot set mass!");
		return;
	}

	g_engineContext.m_physics->SetMass(*collision->m_handle, p_mass);

	RootForce::Physics* physics = m_world->GetEntityManager()->GetComponent<RootForce::Physics>(p_entity);
	physics->m_mass = p_mass;
}

void EngineActions::SetVelocity( ECS::Entity* p_entity, glm::vec3& p_velocity )
{
	RootForce::Physics* physics = m_world->GetEntityManager()->GetComponent<RootForce::Physics>(p_entity);
	physics->m_velocity = p_velocity;
}

void EngineActions::AddPhysics( ECS::Entity* p_entity )
{
	RootForce::Physics* physics = m_world->GetEntityManager()->CreateComponent<RootForce::Physics>(p_entity);
	RootForce::Collision* collision = m_world->GetEntityManager()->CreateComponent<RootForce::Collision>(p_entity);
	RootForce::Transform* trans = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);

	collision->m_handle = g_engineContext.m_physics->CreateHandle(p_entity, RootEngine::Physics::PhysicsType::TYPE_DYNAMIC, false);
	g_engineContext.m_physics->SetGravity(*collision->m_handle, glm::vec3(0, -9.82f, 0));
	physics->m_mass = 1;
	g_engineContext.m_physics->BindSphereShape(*collision->m_handle, trans->m_position, glm::quat(0,0,0,1), 1, physics->m_mass, true, true);
}

void EngineActions::RemovePhysics( ECS::Entity* p_entity )
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Physics>(p_entity);
}

void EngineActions::AddWaterCollider( ECS::Entity* p_entity )
{
	m_world->GetEntityManager()->CreateComponent<RootForce::WaterCollider>(p_entity);
}

void EngineActions::RemoveWaterCollider( ECS::Entity* p_entity )
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::WaterCollider>(p_entity);
}

int EngineActions::GetWaterColliderRadius(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::WaterCollider>(p_entity)->m_radius;
}

float EngineActions::GetWaterColliderInterval(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::WaterCollider>(p_entity)->m_disturbInterval;
}

float EngineActions::GetWaterColliderPower(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::WaterCollider>(p_entity)->m_disturbPower;
}

void EngineActions::SetWaterColliderRadius(ECS::Entity* p_entity, int p_val )
{
	 m_world->GetEntityManager()->GetComponent<RootForce::WaterCollider>(p_entity)->m_radius = p_val;
}

void EngineActions::SetWaterColliderInterval(ECS::Entity* p_entity, float p_val )
{
	m_world->GetEntityManager()->GetComponent<RootForce::WaterCollider>(p_entity)->m_disturbInterval = p_val;
}

void EngineActions::SetWaterColliderPower(ECS::Entity* p_entity, float p_val )
{
	m_world->GetEntityManager()->GetComponent<RootForce::WaterCollider>(p_entity)->m_disturbPower = p_val;
}

void EngineActions::AddScript( ECS::Entity* p_entity )
{
	m_world->GetEntityManager()->CreateComponent<RootForce::Script>(p_entity);
}

void EngineActions::RemoveScript( ECS::Entity* p_entity )
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Script>(p_entity);
}

std::string EngineActions::GetScript( ECS::Entity* p_entity )
{
	return m_world->GetEntityManager()->GetComponent<RootForce::Script>(p_entity)->Name;
}

void EngineActions::SetScript( ECS::Entity* p_entity, std::string p_script )
{
	m_world->GetEntityManager()->GetComponent<RootForce::Script>(p_entity)->Name = g_engineContext.m_resourceManager->LoadScript(p_script);
}
