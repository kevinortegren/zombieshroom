#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/World.h>



namespace RootForce
{
	void WorldSystem::CreateWorld(const std::string& p_worldName)
	{
		// Setup static lights.
		m_engineContext->m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		ECS::Entity* sun = m_world->GetEntityManager()->CreateEntity();
		RootForce::Transform* sunTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(sun);
		RootForce::DirectionalLight* sunLight = m_world->GetEntityManager()->CreateComponent<RootForce::DirectionalLight>(sun);
		sunLight->m_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		sunTransform->m_position = glm::vec3(0.0f, 0.0f, 100.0f);
		sunTransform->m_orientation.LookAt(glm::vec3(0.61f, -0.46f, 0.63f), glm::vec3(0.0f, 1.0f, 0.0f));
		RootForce::Shadowcaster* sunShadowcaster = m_world->GetEntityManager()->CreateComponent<RootForce::Shadowcaster>(sun);
		sunShadowcaster->m_resolution = 512;
		sunShadowcaster->m_levels = 1;

		glm::vec3 fro = sunTransform->m_orientation.GetFront();
		m_engineContext->m_logger->LogText(LogTag::GAME, LogLevel::DEBUG_PRINT, "Sun direction: %f, %f, %f", fro.x, fro.y, fro.z);

		Render::DirectionalLight directionalLight;
		directionalLight.m_color = sunLight->m_color;
		directionalLight.m_direction = sunTransform->m_orientation.GetFront();

		m_engineContext->m_renderer->AddDirectionalLight(directionalLight, 0);

		// Setup skybox entity.
		ECS::Entity* skybox = m_world->GetEntityManager()->CreateEntity();

		RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(skybox);
		RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(skybox);
	
		t->m_scale = glm::vec3(-100, -100, -100);
		t->m_orientation.Roll(180);

		r->m_model = m_engineContext->m_resourceManager->LoadCollada("Primitives/box");
		r->m_pass = RootForce::RenderPass::RENDERPASS_SKYBOX;
		r->m_material = m_engineContext->m_resourceManager->GetMaterial("Skybox");
		r->m_material->m_effect = m_engineContext->m_resourceManager->LoadEffect("Skybox");
		r->m_material->m_diffuseMap = m_engineContext->m_resourceManager->LoadTexture("SkyBox", Render::TextureType::TEXTURE_CUBEMAP);
		
		m_world->GetTagManager()->RegisterEntity("Skybox", skybox);
		m_world->GetGroupManager()->RegisterEntity("NonExport", skybox);

		// Import level entities.
		m_world->GetEntityImporter()->Import(m_engineContext->m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\" + p_worldName + ".world");

		// Add static entities to physics.
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Static");
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = itr->second;

			Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(entity);
			Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);

			//collision->m_handle = m_engineContext->m_physics->AddStaticObjectToWorld(entity->GetId());
			collision->m_handle = m_engineContext->m_physics->CreateHandle(entity->GetId(), RootEngine::Physics::PhysicsType::TYPE_STATIC, false);
			m_engineContext->m_physics->BindMeshShape(*(collision->m_handle), collision->m_meshHandle,
				transform->m_position, transform->m_orientation.GetQuaternion(), transform->m_scale, 0.0f, true);
		}


		// Add camera entity.	
		ECS::Entity* cameraEntity = m_world->GetEntityManager()->CreateEntity();

		RootForce::Camera* camera = m_world->GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
		RootForce::Transform* cameraTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);
		RootForce::LookAtBehavior* cameraLookAt = m_world->GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
		RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world->GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
		
		float apsectRatio = (float)m_engineContext->m_renderer->GetWidth() / m_engineContext->m_renderer->GetHeight();

		camera->m_frustrum = Frustum(45.0f, 0.1f, 1000.0f, apsectRatio);

		cameraLookAt->m_targetTag = "AimingDevice";
		cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
		
		cameraThirdPerson->m_targetTag = "AimingDevice";
		cameraThirdPerson->m_displacement = glm::vec3(0.0f, 4.0f, -8.0f);

		m_world->GetTagManager()->RegisterEntity("Camera", cameraEntity);
		m_world->GetGroupManager()->RegisterEntity("NonExport", cameraEntity);	

		m_quadTree.Init(m_engineContext, m_world);
	}

	void WorldSystem::Process()
	{
		if(m_showDebug)
		{
			m_quadTree.RenderDebug();

			ECS::Entity* e = m_world->GetTagManager()->GetEntityByTag("Player");

			auto t = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(e);

			QuadNode* q = m_quadTree.PickRoot(glm::vec2(t->m_position.x,t->m_position.z));
			if(q != nullptr)
			{
				q->GetBounds().DebugDraw(m_engineContext->m_renderer, glm::vec3(0,0,1));
			}
		}

		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Camera");

		RootForce::Frustum* frustrum = &m_world->GetEntityManager()->GetComponent<RootForce::Camera>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_frustrum;
		RootForce::Transform* transform = m_world->GetEntityManager()->GetComponent<RootForce::Transform>(entity);
		
		glm::mat4x4 model;
		model = glm::translate(glm::mat4(1.0f), transform->m_position);
		model = glm::rotate(model, transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());

		frustrum->DrawLines(model, m_engineContext->m_renderer);

		CullNode(&m_world->GetEntityManager()->GetComponent<RootForce::Camera>(m_world->GetTagManager()->GetEntityByTag("Camera"))->m_frustrum, m_quadTree.GetRoot());
	}

	void WorldSystem::CullNode(RootForce::Frustum* p_frustrum, QuadNode* p_node)
	{
		if(p_frustrum->CheckBoxEx(p_node->GetBounds()))
		{
			if(p_node->GetChilds().size() == 0)
			{
				p_node->GetBounds().DebugDraw(m_engineContext->m_renderer, glm::vec3(0,1,1));
			}
			else
			{
				for(unsigned int i = 0; i < p_node->GetChilds().size(); ++i)
				{
					CullNode( p_frustrum, p_node->GetChilds().at(i)); 
				}
			}
		}
	}

	void WorldSystem::ShowDebug(bool p_value)
	{
		m_showDebug = p_value;
	}

}
#endif
