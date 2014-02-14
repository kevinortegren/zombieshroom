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
		glm::vec4 ambient = m_world->GetStorage()->GetValueAsVec4("Ambient");
		//glm::vec4 ambient = glm::vec4(0.1f);
		SetAmbientLight(ambient);

		// Create constant entities.
		CreateSkyBox();
		CreatePlayerCamera();

		// Add collisions shapes for the group "Static".
		AddStaticEntitiesToPhysics();

		// Spatial divide world and split the meshes.
		m_quadTree.Init(m_engineContext, m_world);
	}
#endif

	void WorldSystem::SetAmbientLight(glm::vec4 p_ambient)
	{
		m_engineContext->m_renderer->SetAmbientLight(p_ambient);
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
		
		sunShadowcaster->m_directionalLightSlot = 0;

		m_world->GetTagManager()->RegisterEntity("Sun", sun);
		//m_world->GetGroupManager()->RegisterEntity("NonExport", sun);
	}


	void WorldSystem::CreateSkyBox()
	{
		// Setup skybox entity.
		ECS::Entity* skybox = m_world->GetEntityManager()->CreateEntity();

		RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(skybox);
		RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(skybox);

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

		r->m_model = m_engineContext->m_resourceManager->CreateModel("skybox");
		r->m_model->m_meshes[0]->SetVertexBuffer(m_engineContext->m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
		r->m_model->m_meshes[0]->SetElementBuffer(m_engineContext->m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));
		r->m_model->m_meshes[0]->SetVertexAttribute(m_engineContext->m_renderer->CreateVertexAttributes());
		r->m_model->m_meshes[0]->CreateVertexBuffer1P(&vertices[0], 8);
		r->m_model->m_meshes[0]->CreateIndexBuffer(&indices[0], 36);
	
		r->m_pass = RootForce::RenderPass::RENDERPASS_SKYBOX;
		r->m_renderFlags = Render::RenderFlags::RENDER_IGNORE_CASTSHADOW;
		r->m_material = m_engineContext->m_renderer->CreateMaterial("skybox");
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

#endif

	void WorldSystem::Process()
	{	
		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");

		RootForce::Frustum* frustrum = &m_world->GetEntityManager()->GetComponent<RootForce::Camera>(entity)->m_frustum;
		
		// Cull static geometry.
		m_quadTree.m_culledEntities.clear();
		m_quadTree.Cull(frustrum, m_quadTree.GetRoot());

		for(auto itr = m_quadTree.m_culledEntities.begin(); itr != m_quadTree.m_culledEntities.end(); ++itr)
		{
			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_quadTree.m_entities[(*itr)]);

			Render::RenderJob job;
			job.m_mesh = renderable->m_model->m_meshes[0];
			job.m_shadowMesh = renderable->m_model->m_meshes[0];
			job.m_material = renderable->m_material;
			job.m_flags = renderable->m_renderFlags;
			job.m_renderPass = renderable->m_pass;
			job.m_params = renderable->m_params;

			m_engineContext->m_renderer->AddRenderJob(job);
		}
	}

	void WorldSystem::ShowDebug(bool p_value)
	{
		m_showDebug = p_value;
	}

	QuadTree* WorldSystem::GetQuadTree()
	{
		return &m_quadTree;
	}
}

