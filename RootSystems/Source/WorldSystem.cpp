#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/World.h>

namespace RootForce
{
#ifndef COMPILE_LEVEL_EDITOR
	void WorldSystem::LoadWorld(const std::string& p_worldName)
	{
		// Import entities, groups, tags and storage.
		m_world->GetEntityImporter()->Import(m_engineContext->m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\" + p_worldName + ".world");
		
		// Parse ambient data.
		glm::vec3 ambient = m_world->GetStorage()->GetValueAsVec3("Ambient");
		SetAmbientLight(ambient);

		// Create constant entities.
		CreateSun();
		CreateSkyBox();
		CreatePlayerCamera();

		// Add collisions shapes for the group "Static".
		AddStaticEntitiesToPhysics();

		// Spatial divide world and split the meshes.
		m_quadTree.Init(m_engineContext, m_world);
	}
#endif

	void WorldSystem::SetAmbientLight(glm::vec3 p_ambient)
	{
		m_engineContext->m_renderer->SetAmbientLight(glm::vec4(p_ambient, 1.0f));
		m_world->GetStorage()->SetValue("Ambient", p_ambient);
	}

	void WorldSystem::CreateSun()
	{
		// Setup sun entity.
		ECS::Entity* sun = m_world->GetEntityManager()->CreateEntity();
		
		RootForce::Transform* sunTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(sun);
		RootForce::DirectionalLight* sunLight = m_world->GetEntityManager()->CreateComponent<RootForce::DirectionalLight>(sun);
		RootForce::Shadowcaster* sunShadowcaster = m_world->GetEntityManager()->CreateComponent<RootForce::Shadowcaster>(sun);

		sunLight->m_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		sunTransform->m_orientation.LookAt(glm::vec3(0.61f, -0.46f, 0.63f), glm::vec3(0.0f, 1.0f, 0.0f));
		sunTransform->m_position = -300.0f * sunTransform->m_orientation.GetFront();
		
		sunShadowcaster->m_levels = 1;

		m_world->GetTagManager()->RegisterEntity("Sun", sun);
		m_world->GetGroupManager()->RegisterEntity("NonExport", sun);
	}

	void WorldSystem::CreateSkyBox()
	{
		// Setup skybox entity.
		ECS::Entity* skybox = m_world->GetEntityManager()->CreateEntity();

		RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(skybox);
		RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(skybox);
	
		t->m_scale = glm::vec3(-100, -100, -100);
		t->m_orientation.Roll(180);

		r->m_model = m_engineContext->m_resourceManager->LoadCollada("Primitives/box");
		r->m_pass = RootForce::RenderPass::RENDERPASS_SKYBOX;
		r->m_renderFlags = Render::RenderFlags::RENDER_IGNORE_CASTSHADOW;
		r->m_material = m_engineContext->m_resourceManager->GetMaterial("Skybox");
		r->m_material->m_effect = m_engineContext->m_resourceManager->LoadEffect("Skybox");
		r->m_material->m_textures[Render::TextureSemantic::DIFFUSE] =  m_engineContext->m_resourceManager->LoadTexture("SkyBox", Render::TextureType::TEXTURE_CUBEMAP);

		m_world->GetTagManager()->RegisterEntity("Skybox", skybox);
		m_world->GetGroupManager()->RegisterEntity("NonExport", skybox);
	}

#ifndef COMPILE_LEVEL_EDITOR
	void WorldSystem::AddStaticEntitiesToPhysics()
	{
		// Add static entities to physics.
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Static");
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = itr->second;

			Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(entity);
			Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);

			//collision->m_handle = m_engineContext->m_physics->AddStaticObjectToWorld(entity->GetId());
			collision->m_handle = m_engineContext->m_physics->CreateHandle(entity, RootEngine::Physics::PhysicsType::TYPE_STATIC, false);
			m_engineContext->m_physics->BindMeshShape(*(collision->m_handle), collision->m_meshHandle,
				transform->m_position, transform->m_orientation.GetQuaternion(), transform->m_scale, 0.0f, true);
		}
	}
#endif

	void WorldSystem::CreatePlayerCamera()
	{
		// Add camera entity.	
		ECS::Entity* cameraEntity = m_world->GetEntityManager()->CreateEntity();

		RootForce::Camera* camera = m_world->GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
		RootForce::Transform* cameraTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);

		RootForce::LookAtBehavior* cameraLookAt = m_world->GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
		RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world->GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
		
		float aspectRatio = (float)m_engineContext->m_renderer->GetWidth() / m_engineContext->m_renderer->GetHeight();

		camera->m_frustum = Frustum(45.0f, 1.0f, 5000.0f, aspectRatio);

		cameraLookAt->m_targetTag = "AimingDevice";
		cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
		
		cameraThirdPerson->m_targetTag = "AimingDevice";
		cameraThirdPerson->m_displacement = glm::vec3(0.0f, -1.0f, -2.0f);
		cameraThirdPerson->m_distance = 8.0f;

		m_world->GetTagManager()->RegisterEntity("Camera", cameraEntity);
		m_world->GetGroupManager()->RegisterEntity("NonExport", cameraEntity);

		// Add testcamera entity.	
		ECS::Entity* testCameraEntity = m_world->GetEntityManager()->CreateEntity();

		RootForce::Camera* testCamera = m_world->GetEntityManager()->CreateComponent<RootForce::Camera>(testCameraEntity);
		RootForce::Transform* testCameraTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(testCameraEntity);
		testCameraTransform->m_position = glm::vec3(0.0f, 30.0f, 0.0f);
		aspectRatio = (float)m_engineContext->m_renderer->GetWidth() / m_engineContext->m_renderer->GetHeight();

		testCamera->m_frustum = Frustum(45.0f, 1.0f, 1000.0f, aspectRatio);

		m_world->GetTagManager()->RegisterEntity("TestCamera", testCameraEntity);
		m_world->GetGroupManager()->RegisterEntity("NonExport", testCameraEntity);
	}

	void WorldSystem::Process()
	{	
		ECS::Entity* testCameraEntity = m_world->GetTagManager()->GetEntityByTag("TestCamera");
		Transform* testCameraTransform = m_world->GetEntityManager()->GetComponent<Transform>(testCameraEntity);
		testCameraTransform->m_orientation.Yaw(45.0f * m_world->GetDelta());

		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");

		RootForce::Frustum* frustrum = &m_world->GetEntityManager()->GetComponent<RootForce::Camera>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_frustum;
		
		m_quadTree.Render(frustrum, m_quadTree.GetRoot());
	}

	void WorldSystem::ShowDebug(bool p_value)
	{
		m_showDebug = p_value;
	}
}

