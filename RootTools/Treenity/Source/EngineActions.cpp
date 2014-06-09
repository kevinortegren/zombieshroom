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
	// Add non-editable entities.
	m_treenityMain->GetEditor()->InitializeTools(m_world);
	ECS::Entity* skybox = m_treenityMain->GetWorldSystem()->CreateSkyBox();
	CreateFreeFlyingCamera();

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
	QFileInfo fileInfo(p_filePath);
	QProgressDialog dialog(0,0);
	Utils::ShowProgressBar(&dialog, "Level: " + fileInfo.completeBaseName());

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
	m_world->GetGroupManager()->RegisterEntity("NonSelectable", m_cameraEntity);	


	// Setup aiming device.
	m_aimingDevice = m_world->GetEntityManager()->CreateEntity();

	RootForce::Transform* aimingDeviceTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_aimingDevice);

	m_world->GetTagManager()->RegisterEntity("AimingDevice", m_aimingDevice);
	m_world->GetGroupManager()->RegisterEntity("NonExport", m_aimingDevice);
	m_world->GetGroupManager()->RegisterEntity("NonSelectable", m_aimingDevice);	
}

void EngineActions::CreateTestSpawnpoint()
{
	m_testSpawnpoint = m_world->GetTagManager()->GetEntityByTag("TestSpawnpoint");
	RootForce::Transform* transform;
	RootForce::Renderable* renderable;

	if (m_testSpawnpoint == nullptr)
	{
		m_testSpawnpoint = m_world->GetEntityManager()->CreateEntity();

		transform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(m_testSpawnpoint);
	}
	else
	{
		transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_testSpawnpoint);
	}

	renderable = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(m_testSpawnpoint);
	renderable->m_material = g_engineContext.m_renderer->CreateMaterial("TestSpawnpoint");
	renderable->m_model = g_engineContext.m_resourceManager->LoadCollada("testchar");
	renderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture("WStexture", Render::TextureType::TextureType::TEXTURE_2D);
	renderable->m_material->m_textures[Render::TextureSemantic::GLOW] = g_engineContext.m_resourceManager->LoadTexture("WSGlowRed", Render::TextureType::TextureType::TEXTURE_2D);
	renderable->m_material->m_textures[Render::TextureSemantic::SPECULAR] = g_engineContext.m_resourceManager->LoadTexture("WSSpecular", Render::TextureType::TextureType::TEXTURE_2D);
	renderable->m_material->m_textures[Render::TextureSemantic::NORMAL] = g_engineContext.m_resourceManager->LoadTexture("WSNormal", Render::TextureType::TextureType::TEXTURE_2D);
	renderable->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_NormalMap");
	renderable->m_shadowTech = Render::ShadowTechnique::SHADOW_DYNAMIC;

	//m_world->GetGroupManager()->RegisterEntity("NonExport", m_testSpawnpoint);
	m_world->GetTagManager()->RegisterEntity("TestSpawnpoint", m_testSpawnpoint);
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
	ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Tools");
	std::vector<ECS::Entity*> m_entitiesToRemove;
	for(std::multimap<std::string, ECS::Entity*>::iterator itr = range.first; itr != range.second; ++itr)
	{
		m_entitiesToRemove.push_back((*itr).second);
	}
	for(ECS::Entity* e : m_entitiesToRemove)
	{
		m_world->GetEntityManager()->RemoveEntity(e);
	}
	m_world->GetEntityManager()->CleanUp();

	QCursor::setPos(QApplication::primaryScreen(), m_treenityMain->GetEditor()->GetUi().widget_canvas3D->geometry().center());
	m_editorMode = EditorMode::GAME;
	g_engineContext.m_inputSys->LockMouseToCenter(true);

	// Get the spawn position/orientation.
	RootForce::Transform* spawnTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(m_world->GetTagManager()->GetEntityByTag("TestSpawnpoint"));

	QProgressDialog dialog(0,0);
	Utils::ShowProgressBar(&dialog, "Entering play mode");

	ParallelPlayModeEnter();

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
	transform->m_scale = spawnTransform->m_scale;
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
	QProgressDialog dialog(0,0);
	Utils::ShowProgressBar(&dialog, "Exiting play mode");

	// Stop the animation system.
	m_treenityMain->GetAnimationSystem()->Synch();
	m_treenityMain->GetAnimationSystem()->Terminate();

	// Clear whatever happened within the game session.
	ClearScene();
	g_engineContext.m_physics->RemoveAll();
	g_networkEntityMap.clear();
	RootForce::Network::NetworkComponent::ResetSequenceForUser(0);
	m_treenityMain->ProcessWorldMessages();

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

	m_treenityMain->GetEditor()->ExitPlayMode();

	//Set params in terrain, quick bug fix. The real problem comes from an issue with import not setting tile factor param in renderable.
	ECS::Entity* terrainEnt = m_world->GetTagManager()->GetEntityByTag("Terrain");
	if(terrainEnt != nullptr)
	{
		RootForce::Renderable* terRend = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(terrainEnt);
		if(terRend != nullptr)
		{
			terRend->m_params[Render::Semantic::SIZEMIN] = &terRend->m_material->m_tileFactor;
		}
	}

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
	if (GetMode() == EditorMode::EDITOR)
	{
		ECS::Entity* cameraEntity = m_world->GetTagManager()->GetEntityByTag("Camera");
		RootForce::Script* script = m_world->GetEntityManager()->GetComponent<RootForce::Script>(cameraEntity);

		g_engineContext.m_script->SetFunction(script->Name, "Target");
		g_engineContext.m_script->AddParameterUserData(p_entity, sizeof(ECS::Entity*), "Entity");
		g_engineContext.m_script->ExecuteScript();
	}
}

void EngineActions::DeleteEntity(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveEntity(p_entity);
}

ECS::Entity* EngineActions::GetEntityByTag( const std::string& p_tag )
{
	return m_world->GetTagManager()->GetEntityByTag(p_tag);
}

void EngineActions::DuplicateEntity( ECS::Entity* p_entity )
{
	//Create new entity
	ECS::Entity* newEntity = CreateEntity();
	//Set position, scale and rotation of new entity to same as old entity + a little offset in X (to avoid clipping and confusion)
	SetPosition(newEntity, GetPosition(p_entity) + glm::vec3(10, 0, 0));
	SetOrientation(newEntity, GetOrientation(p_entity));
	SetScale(newEntity, GetScale(p_entity));

	//Get all components
	RootForce::Renderable*		renderable		= m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(p_entity);
	RootForce::Physics*			physics			= m_world->GetEntityManager()->GetComponent<RootForce::Physics>(p_entity);
	RootForce::Collision*		collision		= m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	RootForce::Script*			script			= m_world->GetEntityManager()->GetComponent<RootForce::Script>(p_entity);
	RootForce::WaterCollider*	watercollider	= m_world->GetEntityManager()->GetComponent<RootForce::WaterCollider>(p_entity);
	RootForce::ParticleEmitter* particleEmitter = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(p_entity);

	//Check if there's a renderable and copy data
	if(renderable != nullptr)
	{
		AddRenderable(newEntity);
		SetRenderableModelName(newEntity, GetRenderableModelName(p_entity));
		SetRenderableMaterialName(newEntity, GetRenderableMaterialName(p_entity));
	}

	//Check if there's a renderable and copy data
	if(collision != nullptr)
	{
		if(physics != nullptr)
		{
			AddPhysics(newEntity, true);
			SetMass(newEntity, GetMass(p_entity));
		}
		else
			AddPhysics(newEntity, false);

		SetPhysicsType(newEntity, GetPhysicsType(p_entity));
		SetPhysicsShape(newEntity, GetPhysicsShape(p_entity));
		SetShapeHeight(newEntity, GetShapeHeight(p_entity));
		SetShapeRadius(newEntity, GetShapeRadius(p_entity));
		SetPhysicsMesh(newEntity, GetPhysicsMesh(p_entity));
		SetCollideWithStatic(newEntity, GetCollideWithStatic(p_entity));
		SetCollideWithWorld(newEntity, GetCollideWithWorld(p_entity));
		SetGravity(newEntity, GetGravity(p_entity));	
	}

	//Check if there's a script and copy data
	if(script != nullptr)
	{
		AddScript(newEntity);
		SetScript(newEntity, GetScript(p_entity));
	}

	//Check if there's a water collider and copy data
	if(watercollider != nullptr)
	{
		AddWaterCollider(newEntity);
		SetWaterColliderInterval(newEntity, GetWaterColliderInterval(p_entity));
		SetWaterColliderPower(newEntity, GetWaterColliderPower(p_entity));
		SetWaterColliderRadius(newEntity, GetWaterColliderRadius(p_entity));
	}

	if(particleEmitter != nullptr)
	{
		AddParticle(newEntity);
		SetParticleEmitter(newEntity, GetParticleEmitter(p_entity));
	}
}

//Other
const ECS::World* EngineActions::GetWorld()
{
	return m_world;
}

//Water settings
void EngineActions::SetWaterParam( std::stringstream* p_ss )
{
	m_treenityMain->GetWaterSystem()->ParseCommands(p_ss);
}

void EngineActions::SetWaterColor( const glm::vec3& p_color )
{
	m_treenityMain->GetWaterSystem()->SetColor(p_color);
}

float EngineActions::GetWaterSpeed()
{
	return m_treenityMain->GetWaterSystem()->GetSpeed();
}

float EngineActions::GetWaterDepth()
{
	return m_treenityMain->GetWaterSystem()->GetDepth();
}

bool EngineActions::GetWaterNormals()
{
	return m_treenityMain->GetWaterSystem()->GetNormals();
}

bool EngineActions::GetWaterReflections()
{
	return m_treenityMain->GetWaterSystem()->GetReflections();
}

bool EngineActions::GetWaterRefractions()
{
	return m_treenityMain->GetWaterSystem()->GetRefractions();
}

bool EngineActions::GetWaterPaused()
{
	return m_treenityMain->GetWaterSystem()->GetPaused();
}

glm::vec3 EngineActions::GetWaterColor()
{
	return m_treenityMain->GetWaterSystem()->GetColor();
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
	renderable->m_model = g_engineContext.m_resourceManager->LoadCollada("Primitives/sphere");
	renderable->m_material = g_engineContext.m_renderer->CreateMaterial("DefaultMaterial");
	renderable->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh");
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

std::string EngineActions::GetMaterialEffectName(Render::Material* p_material) 
{
	return g_engineContext.m_resourceManager->ResolveStringFromEffect(p_material->m_effect);
}

Render::Material* EngineActions::GetMaterial(const std::string& p_string)
{
	return g_engineContext.m_renderer->CreateMaterial(p_string);
}

Render::TextureInterface* EngineActions::GetTexture(const std::string& p_path)
{
	return g_engineContext.m_resourceManager->LoadTexture(p_path, Render::TextureType::TEXTURE_2D);
}

std::string EngineActions::GetTextureName(Render::TextureInterface* p_texture)
{
	return g_engineContext.m_resourceManager->ResolveStringFromTexture(p_texture);
}

Render::EffectInterface* EngineActions::GetEffect(const std::string& p_path)
{
	return g_engineContext.m_resourceManager->LoadEffect(p_path);
}

std::string EngineActions::GetContentPath()
{
	return g_engineContext.m_resourceManager->GetWorkingDirectory();
}

//Physics

void EngineActions::ReconstructPhysicsObject(ECS::Entity* p_entity, bool p_dynamic, bool p_collideWithWorld, bool p_collideWithStatic, bool p_mass, RootEngine::Physics::PhysicsShape::PhysicsShape p_shape, float p_radius, float p_height, const std::string& p_meshHandle, bool p_visualize)
{
	RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	glm::vec3 gravity;

	if (collision->m_handle != nullptr)
	{
		gravity = g_engineContext.m_physics->GetGravity(*collision->m_handle);
		g_engineContext.m_physics->RemoveObject(*collision->m_handle);
	}
	collision->m_handle = g_engineContext.m_physics->CreateHandle(p_entity, p_dynamic ? RootEngine::Physics::PhysicsType::TYPE_DYNAMIC : RootEngine::Physics::PhysicsType::TYPE_STATIC, false);

	//g_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::PINK_PRINT, "Recreating physics object associated with entity %d with new handle %d", p_entity->GetId(), *collision->m_handle);

	switch (p_shape)
	{
		case RootEngine::Physics::PhysicsShape::SHAPE_SPHERE:
			g_engineContext.m_physics->BindSphereShape(*collision->m_handle, transform->m_position, transform->m_orientation.GetQuaternion(), p_radius, p_mass, p_collideWithWorld, p_collideWithStatic, p_visualize);
		break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CONE:
			g_engineContext.m_physics->BindConeShape(*collision->m_handle, transform->m_position, transform->m_orientation.GetQuaternion(), p_height, p_radius, p_mass, p_collideWithWorld, p_collideWithStatic, p_visualize);
		break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CYLINDER:
			g_engineContext.m_physics->BindCylinderShape(*collision->m_handle, transform->m_position, transform->m_orientation.GetQuaternion(), p_height, p_radius, p_mass, p_collideWithWorld, p_collideWithStatic, p_visualize);
		break;

		case RootEngine::Physics::PhysicsShape::SHAPE_CUSTOM_MESH:
			{
				std::string newMeshHandle = p_meshHandle;
				if(newMeshHandle == "")
				{
					RootForce::Renderable* rend = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(p_entity);
					if(rend && rend->m_model)
					{
						newMeshHandle = g_engineContext.m_resourceManager->ResolveStringFromModel(rend->m_model);

						if(newMeshHandle == "")
							newMeshHandle = "Primitives/box";
						//else
							//newMeshHandle += "0";
					}
					else
					{
						newMeshHandle = "Primitives/box";
					}					
				}
				else
				{
					// Load the model if necessary
					g_engineContext.m_resourceManager->LoadCollada(newMeshHandle);
				}

				g_engineContext.m_physics->BindMeshShape(*collision->m_handle, newMeshHandle + "0", transform->m_position, transform->m_orientation.GetQuaternion(), transform->m_scale, p_mass, p_collideWithWorld, p_collideWithStatic, p_visualize);

			}
		break;
	}

	//g_engineContext.m_physics->ToggleCollisionFlag(*collision->m_handle, btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	RootForce::Physics* physComp = m_world->GetEntityManager()->GetComponent<RootForce::Physics>(p_entity);
	if (p_dynamic)
	{
		if (physComp == nullptr)
		{
			RootForce::Physics* physics = m_world->GetEntityManager()->CreateComponent<RootForce::Physics>(p_entity);

			physics->m_mass = p_mass;
			//g_engineContext.m_physics->SetGravity(*collision->m_handle, glm::vec3(0, -9.82f, 0));
		}

		g_engineContext.m_physics->SetGravity(*collision->m_handle, gravity);
	}
	else
	{
		m_world->GetEntityManager()->RemoveComponent<RootForce::Physics>(p_entity);
	}
}

void EngineActions::AddPhysics( ECS::Entity* p_entity, bool p_dynamic )
{
	RootForce::Transform* trans = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(p_entity);
	RootForce::Collision* collision = m_world->GetEntityManager()->CreateComponent<RootForce::Collision>(p_entity);
	ReconstructPhysicsObject(p_entity, p_dynamic, true, true, 1.0f, RootEngine::Physics::PhysicsShape::SHAPE_SPHERE, 1.0f, 0.0f, "", false);
}

void EngineActions::RemovePhysics( ECS::Entity* p_entity )
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::Collision>(p_entity);
	m_world->GetEntityManager()->RemoveComponent<RootForce::Physics>(p_entity);
}

RootEngine::Physics::PhysicsType::PhysicsType EngineActions::GetPhysicsType(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return (RootEngine::Physics::PhysicsType::PhysicsType) g_engineContext.m_physics->GetType(*collision->m_handle);
}

bool EngineActions::GetCollideWithWorld(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return g_engineContext.m_physics->GetCollideWithWorld(*collision->m_handle);
}

bool EngineActions::GetCollideWithStatic(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return g_engineContext.m_physics->GetCollideWithStatic(*collision->m_handle);
}

glm::vec3 EngineActions::GetGravity(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return g_engineContext.m_physics->GetGravity(*collision->m_handle);
}

float EngineActions::GetMass(ECS::Entity* p_entity)
{
	RootForce::Physics* physComp = m_world->GetEntityManager()->GetComponent<RootForce::Physics>(p_entity);
	if (physComp != nullptr)
		return physComp->m_mass;
	return 0.0f;
}

RootEngine::Physics::PhysicsShape::PhysicsShape EngineActions::GetPhysicsShape(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return g_engineContext.m_physics->GetShape(*collision->m_handle);
}

float EngineActions::GetShapeRadius(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return g_engineContext.m_physics->GetRadius(*collision->m_handle);
}

float EngineActions::GetShapeHeight(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return g_engineContext.m_physics->GetHeight(*collision->m_handle);
}

std::string EngineActions::GetPhysicsMesh(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	std::string mesh = g_engineContext.m_physics->GetPhysicsModelHandle(*collision->m_handle);
	return mesh.substr(0, mesh.size() - 1);
}

bool EngineActions::GetCollisionVisualized(ECS::Entity* p_entity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	return g_engineContext.m_physics->IsVisualizeEnabled(*collision->m_handle);
}

void EngineActions::SetPhysicsType(ECS::Entity* p_entity, bool p_dynamic)
{
	ReconstructPhysicsObject(p_entity, p_dynamic, GetCollideWithWorld(p_entity), GetCollideWithStatic(p_entity), GetMass(p_entity), GetPhysicsShape(p_entity), GetShapeRadius(p_entity), GetShapeHeight(p_entity), GetPhysicsMesh(p_entity), GetCollisionVisualized(p_entity));
}

void EngineActions::SetCollideWithWorld(ECS::Entity* p_entity, bool p_collide)
{
	ReconstructPhysicsObject(p_entity, GetPhysicsType(p_entity), p_collide, GetCollideWithStatic(p_entity), GetMass(p_entity), GetPhysicsShape(p_entity), GetShapeRadius(p_entity), GetShapeHeight(p_entity), GetPhysicsMesh(p_entity), GetCollisionVisualized(p_entity));
}

void EngineActions::SetCollideWithStatic(ECS::Entity* p_entity, bool p_collide)
{
	ReconstructPhysicsObject(p_entity, GetPhysicsType(p_entity), GetCollideWithWorld(p_entity), p_collide, GetMass(p_entity), GetPhysicsShape(p_entity), GetShapeRadius(p_entity), GetShapeHeight(p_entity), GetPhysicsMesh(p_entity), GetCollisionVisualized(p_entity));
}

void EngineActions::SetGravity(ECS::Entity* p_entity, const glm::vec3& p_gravity)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	g_engineContext.m_physics->SetGravity(*collision->m_handle, p_gravity);
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

void EngineActions::SetPhysicsShape(ECS::Entity* p_entity, RootEngine::Physics::PhysicsShape::PhysicsShape p_shape)
{
	ReconstructPhysicsObject(p_entity, GetPhysicsType(p_entity), GetCollideWithWorld(p_entity), GetCollideWithStatic(p_entity), GetMass(p_entity), p_shape, GetShapeRadius(p_entity), GetShapeHeight(p_entity), GetPhysicsMesh(p_entity), GetCollisionVisualized(p_entity));
}

void EngineActions::SetShapeRadius(ECS::Entity* p_entity, float p_radius)
{
	ReconstructPhysicsObject(p_entity, GetPhysicsType(p_entity), GetCollideWithWorld(p_entity), GetCollideWithStatic(p_entity), GetMass(p_entity), GetPhysicsShape(p_entity), p_radius, GetShapeHeight(p_entity), GetPhysicsMesh(p_entity), GetCollisionVisualized(p_entity));
}

void EngineActions::SetShapeHeight(ECS::Entity* p_entity, float p_height)
{
	ReconstructPhysicsObject(p_entity, GetPhysicsType(p_entity), GetCollideWithWorld(p_entity), GetCollideWithStatic(p_entity), GetMass(p_entity), GetPhysicsShape(p_entity), GetShapeRadius(p_entity), p_height, GetPhysicsMesh(p_entity), GetCollisionVisualized(p_entity));
}

void EngineActions::SetPhysicsMesh(ECS::Entity* p_entity, const std::string& p_mesh)
{
	ReconstructPhysicsObject(p_entity, GetPhysicsType(p_entity), GetCollideWithWorld(p_entity), GetCollideWithStatic(p_entity), GetMass(p_entity), RootEngine::Physics::PhysicsShape::SHAPE_CUSTOM_MESH, GetShapeRadius(p_entity), GetShapeHeight(p_entity), p_mesh, GetCollisionVisualized(p_entity));
}

void EngineActions::SetCollisionVisualized(ECS::Entity* p_entity, bool p_visualize)
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	if (g_engineContext.m_physics->GetType(*collision->m_handle) == RootEngine::Physics::PhysicsType::TYPE_STATIC ||
		g_engineContext.m_physics->GetType(*collision->m_handle) == RootEngine::Physics::PhysicsType::TYPE_DYNAMIC)
	{
		ReconstructPhysicsObject(p_entity, GetPhysicsType(p_entity), GetCollideWithWorld(p_entity), GetCollideWithStatic(p_entity), GetMass(p_entity), GetPhysicsShape(p_entity), GetShapeRadius(p_entity), GetShapeHeight(p_entity), GetPhysicsMesh(p_entity), p_visualize);
	}
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
	m_world->GetEntityManager()->GetComponent<RootForce::Script>(p_entity)->Name = g_engineContext.m_resourceManager->ForceLoadScript(p_script);
}

//Collision responder
void EngineActions::AddCollisionResponder( ECS::Entity* p_entity )
{
	RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(p_entity);
	if (collision != nullptr)
	{
		RootForce::CollisionResponder* responder = m_world->GetEntityManager()->CreateComponent<RootForce::CollisionResponder>(p_entity);
		g_engineContext.m_physics->SetCollisionContainer(*collision->m_handle, &responder->m_collisions);
	}
	else
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::WARNING, "Cannot add collision responder with collision component.");
		Utils::Write("Cannot add collision responder with collision component.");
	}
}

void EngineActions::RemoveCollisionResponder( ECS::Entity* p_entity )
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::CollisionResponder>(p_entity);
}

void EngineActions::AddParticle(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p_entity);
}

void EngineActions::RemoveParticle(ECS::Entity* p_entity)
{
	m_world->GetEntityManager()->RemoveComponent<RootForce::ParticleEmitter>(p_entity);
}

void EngineActions::SetParticleEmitter(ECS::Entity* p_entity, const std::string& p_name)
{
	RootForce::ParticleEmitter* particle = m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(p_entity);

	for(unsigned i = 0; i < particle->m_particleSystems.size(); i++)
		g_engineContext.m_renderer->FreeParticleSystem(particle->m_systems[i]);

	particle->m_systems.clear();

	particle->m_particleSystems = g_engineContext.m_resourceManager->LoadParticleEmitter(p_name, false);
	particle->m_name = p_name;

	for(unsigned i = 0; i < particle->m_particleSystems.size(); i++)
		particle->m_systems.push_back(g_engineContext.m_renderer->CreateParticleSystem());
}

std::string	EngineActions::GetParticleEmitter(ECS::Entity* p_entity)
{
	return m_world->GetEntityManager()->GetComponent<RootForce::ParticleEmitter>(p_entity)->m_name;
}

//Special entities
ECS::Entity* EngineActions::CreateTerrainEntity( int p_width, int p_height )
{
	ECS::Entity* terrainEnt = CreateEntity();

	RootForce::Renderable* terraintRend = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(terrainEnt);

	terraintRend->m_material	= g_engineContext.m_renderer->CreateMaterial("supersecretterrainmaterial");

	terraintRend->m_material->m_tileFactor = 12.0f;
	terraintRend->m_params[Render::Semantic::SIZEMIN] = &terraintRend->m_material->m_tileFactor;

	//Set textures to renderable
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP]	= g_engineContext.m_resourceManager->LoadTexture("CaveMountainColor", Render::TextureType::TEXTURE_2D);

	//terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP]	= g_engineContext.m_resourceManager->CreateTexture("TerrainBlendMap");//g_engineContext.m_resourceManager->LoadTexture("CaveMountainColor", Render::TextureType::TEXTURE_2D);
	//terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP]->CreateEmptyTexture(256, 256, Render::TextureFormat::TextureFormat::TEXTURE_RGBA );
	//terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTUREMAP]->SetAccess(GL_READ_WRITE);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]	= g_engineContext.m_resourceManager->LoadTexture("DirtAndStones", Render::TextureType::TEXTURE_2D);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]	= g_engineContext.m_resourceManager->LoadTexture("Grass", Render::TextureType::TEXTURE_2D);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]	= g_engineContext.m_resourceManager->LoadTexture("CliffWall", Render::TextureType::TEXTURE_2D);
	//Set textures as repeat
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	terraintRend->m_material->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	terraintRend->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_Blend");

	if(g_engineContext.m_resourceManager->GetModel("TerrainModelTerrain"))
	{
		terraintRend->m_model = g_engineContext.m_resourceManager->GetModel("TerrainModelTerrain");
		return terrainEnt;
	}

	std::vector<Render::Vertex1P1N1UV> vertices;
	terraintRend->m_model = g_engineContext.m_resourceManager->CreateModel("TerrainModelTerrain");

	for (float x = 0.0f; x < (float)p_width; ++x)
	{
		for (float z = 0.0f; z < (float)p_width; ++z)
		{
			Render::Vertex1P1N1UV v;
			v.m_pos		= glm::vec3(x-(float)p_width/2.0f, 0.0f, z-(float)p_width/2.0f);
			v.m_normal	= glm::vec3(0.0f, 1.0f, 0.0f);
			v.m_UV		= glm::vec2(x/(float)p_width, z/(float)p_width);
			vertices.push_back(v);
			terraintRend->m_model->m_positions.push_back(v.m_pos);
		}
	}

	for (unsigned int x = 0; x < (unsigned int)(p_width-1); ++x)
	{
		for (unsigned int z = 0; z < (unsigned int)(p_width-1); ++z)
		{
			unsigned int  start = x * p_width + z;
			terraintRend->m_model->m_indices.push_back(start);
			terraintRend->m_model->m_indices.push_back(start + 1);
			terraintRend->m_model->m_indices.push_back(start + p_width + 1);
			terraintRend->m_model->m_indices.push_back(start + p_width + 1);
			terraintRend->m_model->m_indices.push_back(start + p_width);
			terraintRend->m_model->m_indices.push_back(start);
		}
	}

	terraintRend->m_model->m_meshes[0]->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
	terraintRend->m_model->m_meshes[0]->SetElementBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));
	terraintRend->m_model->m_meshes[0]->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
	terraintRend->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(&vertices[0], vertices.size());
	terraintRend->m_model->m_meshes[0]->CreateIndexBuffer(&terraintRend->m_model->m_indices[0], terraintRend->m_model->m_indices.size());

	g_engineContext.m_resourceManager->CreatePhysicsMesh(terraintRend->m_model);

	terraintRend->m_model->m_obb.m_maxX = (float)p_width/2.0f;
	terraintRend->m_model->m_obb.m_maxZ = (float)p_width/2.0f;
	terraintRend->m_model->m_obb.m_maxY = 2.0f;
	terraintRend->m_model->m_obb.m_minX = -(float)p_width/2.0f;
	terraintRend->m_model->m_obb.m_minZ = -(float)p_width/2.0f;
	terraintRend->m_model->m_obb.m_minY = -2.0f;

	m_world->GetTagManager()->RegisterEntity("Terrain", terrainEnt);

	return terrainEnt;
}

void EngineActions::UpdateTerrainPhysicsMesh()
{
	ECS::Entity* terrainEnt = m_world->GetTagManager()->GetEntityByTag("Terrain");
	if(terrainEnt == nullptr)
		return;

	RootForce::Renderable* terRend = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(terrainEnt);
	if(terRend == nullptr)
		return;

	RootEngine::Model* terrModel = terRend->m_model;
	if(terrModel == nullptr)
		return;

	Render::MeshInterface* terrMesh = terrModel->m_meshes[0];
	if(terrMesh == nullptr)
		return;

	Render::Vertex1P1N1UV* vertexData = static_cast<Render::Vertex1P1N1UV*>(terrMesh->GetVertexBuffer()->MapBuffer(GL_READ_WRITE));
		
		if(vertexData == nullptr)
		{
			g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Could not map buffer!");
			return;
		}

		unsigned numElements = terrMesh->GetVertexBuffer()->GetNumElements();
		
		terrModel->m_positions.clear();
		terrModel->m_positions.shrink_to_fit();
		terrModel->m_positions.reserve(numElements);

		//Copy all the new positions to the model position vector
		for (unsigned i = 0; i < numElements; ++i)
		{
			terrModel->m_positions.push_back(vertexData[i].m_pos);
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Model UV: x %f  y %f", vertexData[i].m_UV.x, vertexData[i].m_UV.y); 
		}

		if(terrMesh->GetVertexBuffer()->UnmapBuffer() == false)
			g_engineContext.m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Could not unmap buffer!");

		if(terrModel->m_physicsMeshes[0] == nullptr)
			return;

		terrModel->m_physicsMeshes[0]->Init(terrModel->m_positions, (int)terrModel->m_positions.size(), terrModel->m_indices, (int)terrModel->m_indices.size(), (int)terrModel->m_indices.size() / 3);

		RootForce::Collision* collision = m_world->GetEntityManager()->GetComponent<RootForce::Collision>(terrainEnt);
		if(collision == nullptr)
			return;

		ReconstructPhysicsObject(terrainEnt, GetPhysicsType(terrainEnt), GetCollideWithWorld(terrainEnt), GetCollideWithStatic(terrainEnt), GetMass(terrainEnt), GetPhysicsShape(terrainEnt), GetShapeRadius(terrainEnt), GetShapeHeight(terrainEnt), GetPhysicsMesh(terrainEnt), GetCollisionVisualized(terrainEnt));
}