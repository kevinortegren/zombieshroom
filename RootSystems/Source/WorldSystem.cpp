#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/World.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	void WorldSystem::CreateWorld(const std::string& p_worldName)
	{
		// Setup static lights.
		g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		Render::DirectionalLight directionalLight;
		directionalLight.m_color = glm::vec4(0.8f);
		directionalLight.m_direction = glm::vec3(0, -1,-1);

		g_engineContext.m_renderer->AddDirectionalLight(directionalLight, 0);

		// Setup skybox entity.
		ECS::Entity* skybox = m_world->GetEntityManager()->CreateEntity();

		RootForce::Renderable* r = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(skybox);
		RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(skybox);
	
		t->m_scale = glm::vec3(-100);

		r->m_model = g_engineContext.m_resourceManager->LoadCollada("Primitives/box");
		r->m_material = g_engineContext.m_resourceManager->GetMaterial("Skybox");
		r->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Skybox");
		r->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("SkyBox", Render::TextureType::TEXTURE_CUBEMAP);

		m_world->GetTagManager()->RegisterEntity("Skybox", skybox);
		m_world->GetGroupManager()->RegisterEntity("NonExport", skybox);

		// Import level entities.
		m_world->GetEntityImporter()->Import(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\" + p_worldName + ".world");

		// Add static entities to physics.
		ECS::GroupManager::GroupRange range = m_world->GetGroupManager()->GetEntitiesInGroup("Static");
		for(auto itr = range.first; itr != range.second; ++itr)
		{
			ECS::Entity* entity = itr->second;

			Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(entity);
			Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);

			//collision->m_handle = g_engineContext.m_physics->AddStaticObjectToWorld(entity->GetId());
			collision->m_handle = g_engineContext.m_physics->CreateHandle(entity->GetId(), RootEngine::Physics::PhysicsType::TYPE_STATIC, false);
			g_engineContext.m_physics->BindMeshShape(*(collision->m_handle), collision->m_meshHandle,
				transform->m_position, transform->m_orientation.GetQuaternion(), transform->m_scale, 0.0f, true);
		}


		// Add camera entity.	
		ECS::Entity* cameraEntity = m_world->GetEntityManager()->CreateEntity();

		RootForce::Camera* camera = m_world->GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
		RootForce::Transform* cameraTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);
		RootForce::LookAtBehavior* cameraLookAt = m_world->GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
		RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world->GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
		
		camera->m_near = 0.1f;
		camera->m_far = 1000.0f;
		camera->m_fov = 45.0f;

		cameraLookAt->m_targetTag = "AimingDevice";
		cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
		
		cameraThirdPerson->m_targetTag = "AimingDevice";
		cameraThirdPerson->m_displacement = glm::vec3(0.0f, 4.0f, -8.0f);

		m_world->GetTagManager()->RegisterEntity("Camera", cameraEntity);
		m_world->GetGroupManager()->RegisterEntity("NonExport", cameraEntity);	
	}

	void WorldSystem::Process()
	{

	}
}
#endif
