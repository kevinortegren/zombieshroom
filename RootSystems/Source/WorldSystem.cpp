#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/ControllerActionSystem.h>
#include <RootEngine/Script/Include/RootScript.h>

namespace RootForce
{
#ifndef COMPILE_LEVEL_EDITOR
	void WorldSystem::LoadWorld(const std::string& p_worldName)
	{
		// Import entities, groups, tags and storage.
		m_world->GetEntityImporter()->Import(m_engineContext->m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\" + p_worldName + ".world");
		
		BuildStaticShadowMesh();

		// Parse ambient data.
		glm::vec4 ambient = m_world->GetStorage()->GetValueAsVec4("Ambient");
		SetAmbientLight(ambient);

		// Create constant entities.
		CreateSkyBox();
		CreateSun();

		CreatePlayerCamera();

		// Put the static entities into a spatial quad tree.
		m_quadTree.Initialize(m_engineContext, m_world, "Static", "Static_Split", true);

		// Adds static entities.
		AddStaticEntitiesToPhysics();

		//LOL THIS IS NOT THE PLACE
		g_engineContext.m_resourceManager->LoadSoundAudio("CC-BY3.0/death_crack11.wav", 0x00200011);
	}
#endif

	void WorldSystem::CalculateWorldAABB()
	{
		m_quadTree.Initialize(m_engineContext, m_world, "Static", "Static_Split", true);
	}

	void WorldSystem::BuildStaticShadowMesh()
	{
		if(m_staticMesh != nullptr)
			g_engineContext.m_renderer->RemoveMesh(m_staticMesh);

		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Static");

		std::vector<Render::Vertex1P> vertices;
		std::vector<unsigned int> indices;
		
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = (*itr).second;
			
			// Render data.
			RootForce::Renderable* renderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(entity);
			auto mesh = renderable->m_model->m_meshes[0];

			// Transform.
			RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
			glm::mat4x4 transformMatrix;
			transformMatrix = glm::translate(glm::mat4(1.0f), transform->m_position);
			transformMatrix = glm::rotate(transformMatrix, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			transformMatrix = glm::scale(transformMatrix, transform->m_scale);

			unsigned offset = vertices.size();

			// Parse vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVertexBuffer()->GetBufferId());
			unsigned char* data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			for(unsigned i = 0; i < mesh->GetVertexBuffer()->GetBufferSize(); i += mesh->GetVertexBuffer()->GetElementSize())
			{
				Render::Vertex1P v;
				Render::Vertex1P1N1UV1T1BT vtemp;
				
				memcpy(&vtemp, &data[i], mesh->GetVertexBuffer()->GetElementSize());

				v.m_pos = vtemp.m_pos;

				// Transform vertex to world space.
				glm::vec4 tf = transformMatrix * glm::vec4(v.m_pos, 1.0f);
				v.m_pos = glm::vec3(tf.x, tf.y, tf.z);

				vertices.push_back(std::move(v));
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);

			if(mesh->GetElementBuffer() != nullptr)
			{
				// Parse index data.
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementBuffer()->GetBufferId());
				data = (unsigned char*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

				for(unsigned i = 0; i < mesh->GetElementBuffer()->GetBufferSize(); i += mesh->GetElementBuffer()->GetElementSize() * 3)
				{
					int i0, i1, i2;

					memcpy(&i0, &data[i], sizeof(int));
					memcpy(&i1, &data[i + 4], sizeof(int));
					memcpy(&i2, &data[i + 8], sizeof(int));

					indices.push_back(i0 + offset);
					indices.push_back(i1 + offset);
					indices.push_back(i2 + offset);

				}
				glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
			}	


		}

		if(vertices.size() > 0)
		{
			m_staticMesh = g_engineContext.m_renderer->CreateMesh();
			m_staticMesh->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
			m_staticMesh->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
			m_staticMesh->CreateVertexBuffer1P(&vertices[0], vertices.size());
#ifndef COMPILE_LEVEL_EDITOR
			m_staticMesh->SetElementBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER));	
			m_staticMesh->CreateIndexBuffer(&indices[0], indices.size());
#endif

			m_staticMesh->SetPrimitiveType(GL_TRIANGLES);

			m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Creating static mesh vertices: %d indices %d", vertices.size(), indices.size());
		}
		else
		{
			m_staticMesh = nullptr;
		}
	}

	void WorldSystem::SubdivideTree()
	{
		m_quadTree.BeginDivide(2500, true, true);
	}

	void WorldSystem::SetAmbientLight(glm::vec4 p_ambient)
	{
		m_engineContext->m_renderer->SetAmbientLight(p_ambient);
		m_world->GetStorage()->SetValue("Ambient", p_ambient);
	}

	ECS::Entity* WorldSystem::CreateSun()
	{
		ECS::Entity* sun = m_world->GetTagManager()->GetEntityByTag("Sun");
		if(sun == nullptr)
		{
			// Setup sun entity.
			sun = m_world->GetEntityManager()->CreateEntity();
		
			RootForce::Transform* sunTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(sun);
			RootForce::DirectionalLight* sunLight = m_world->GetEntityManager()->CreateComponent<RootForce::DirectionalLight>(sun);
			RootForce::Shadowcaster* sunShadowcaster = m_world->GetEntityManager()->CreateComponent<RootForce::Shadowcaster>(sun);

			sunLight->m_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
			sunTransform->m_orientation.LookAt(glm::vec3(0.61f, -0.46f, 0.63f), glm::vec3(0.0f, 1.0f, 0.0f));
			sunTransform->m_position = -300.0f * sunTransform->m_orientation.GetFront();
		
			sunShadowcaster->m_directionalLightSlot = 0;

			m_world->GetTagManager()->RegisterEntity("Sun", sun);
		}

		RootForce::Transform* sunTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(sun);
		RootForce::DirectionalLight* sunLight = m_world->GetEntityManager()->GetComponent<RootForce::DirectionalLight>(sun);

		RootForce::Renderable* r = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_world->GetTagManager()->GetEntityByTag("Skybox"));

		struct sunstruct
		{
			glm::vec4 direction;
			glm::vec4 color;
		};
		sunstruct sundata;
		sundata.direction = glm::vec4(-sunTransform->m_orientation.GetFront(), 0.0f);
		sundata.color = sunLight->m_color;

		r->m_material->m_effect->GetTechniques()[0]->m_perTechniqueBuffer->BufferData(1, sizeof(sundata), &sundata, GL_DYNAMIC_DRAW);

		return sun;
	}

	ECS::Entity* WorldSystem::CreateSkyBox()
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
		r->m_forward = false;
		r->m_material = m_engineContext->m_renderer->CreateMaterial("skybox");
		r->m_material->m_effect = m_engineContext->m_resourceManager->LoadEffect("Skybox");
		
		m_world->GetTagManager()->RegisterEntity("Skybox", skybox);
		m_world->GetGroupManager()->RegisterEntity("NonExport", skybox);

		return skybox;
	}

#ifndef COMPILE_LEVEL_EDITOR
	void WorldSystem::AddStaticEntitiesToPhysics()
	{
		/*
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
		*/
	}


	void WorldSystem::CreatePlayerCamera()
	{
		// Add camera entity.	
		ECS::Entity* cameraEntity = m_world->GetEntityManager()->CreateEntity();

		RootForce::Camera* camera = m_world->GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
		RootForce::Transform* cameraTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);

		//RootForce::LookAtBehavior* cameraLookAt = m_world->GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
		//RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world->GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
		//
		float aspectRatio = (float)m_engineContext->m_renderer->GetWidth() / m_engineContext->m_renderer->GetHeight();

		camera->m_frustum = Frustum(45.0f, 1.0f, 5000.0f, aspectRatio);

		//cameraLookAt->m_targetTag = "AimingDevice";
		//cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
		//
		//cameraThirdPerson->m_targetTag = "AimingDevice";
		//cameraThirdPerson->m_displacement = glm::vec3(0.0f, -1.0f, -2.0f);
		//cameraThirdPerson->m_distance = 8.0f;

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
		if(m_staticMesh != nullptr)
		{
			Render::ShadowJob job;
			job.m_technique = Render::ShadowTechnique::SHADOW_OPAQUE;
			job.m_mesh = m_staticMesh;

			g_engineContext.m_renderer->AddShadowJob(job);
		}

		RootForce::Renderable* skyboxRenderable = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(m_world->GetTagManager()->GetEntityByTag("Skybox"));

		ECS::Entity* sun = m_world->GetTagManager()->GetEntityByTag("Sun");
		if(sun != nullptr)
		{
			RootForce::Transform* sunTransform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(sun);
			RootForce::DirectionalLight* sunLight = m_world->GetEntityManager()->GetComponent<RootForce::DirectionalLight>(sun);

			glm::vec4 dir = glm::vec4(-sunTransform->m_orientation.GetFront(), 0.0f);

			skyboxRenderable->m_material->m_effect->GetTechniques()[0]->m_perTechniqueBuffer->BufferSubData(0, sizeof(dir), &dir);
			skyboxRenderable->m_material->m_effect->GetTechniques()[0]->m_perTechniqueBuffer->BufferSubData(sizeof(dir), sizeof(sunLight->m_color), &sunLight->m_color);
		}

	/*
#ifndef COMPILE_LEVEL_EDITOR
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
			job.m_renderPass = renderable->m_pass;
			job.m_params = renderable->m_params;

			m_engineContext->m_renderer->AddRenderJob(job);
		}
#endif
		*/
	}

	void WorldSystem::ShowDebug(bool p_value)
	{
		m_showDebug = p_value;
	}

	QuadTree* WorldSystem::GetQuadTree()
	{
		return &m_quadTree;
	}

	WorldSystem::~WorldSystem()
	{
	}

}

