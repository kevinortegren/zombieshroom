#ifndef COMPILE_LEVEL_EDITOR
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/World.h>

namespace RootForce
{
	void PlayerSystem::CreatePlayer(int p_teamID)
	{
		ECS::EntityManager* entityManager = m_world->GetEntityManager();
		ECS::Entity* entity = entityManager->CreateEntity();
		
		RootForce::Renderable* renderable = entityManager->CreateComponent<RootForce::Renderable>(entity);
		RootForce::Transform* transform = entityManager->CreateComponent<RootForce::Transform>(entity);
		RootForce::PlayerControl* playerControl = entityManager->CreateComponent<RootForce::PlayerControl>(entity);
		RootForce::PlayerPhysics* playphys = entityManager->CreateComponent<RootForce::PlayerPhysics>(entity);
		RootForce::HealthComponent* health = entityManager->CreateComponent<RootForce::HealthComponent>(entity);
		RootForce::ScoreComponent* score = entityManager->CreateComponent<RootForce::ScoreComponent>(entity);
		RootForce::UserAbility* ability = entityManager->CreateComponent<RootForce::UserAbility>(entity);
		RootForce::Identity* identity = entityManager->CreateComponent<RootForce::Identity>(entity);

		RootForce::Physics* physics = entityManager->CreateComponent<RootForce::Physics>(entity);
		RootForce::Collision* collision = entityManager->CreateComponent<RootForce::Collision>(entity);
		RootForce::CollisionResponder* collisionResponder = entityManager->CreateComponent<RootForce::CollisionResponder>(entity);
		RootForce::Script* script = entityManager->CreateComponent<RootForce::Script>(entity);
		RootForce::Animation* animation = entityManager->CreateComponent<RootForce::Animation>(entity);

		RootForce::PlayerActionComponent* action = entityManager->CreateComponent<RootForce::PlayerActionComponent>(entity);

		renderable->m_model = m_engineContext->m_resourceManager->LoadCollada("testchar");
		
		renderable->m_params[Render::Semantic::BONES] = renderable->m_model->m_animations[0]->GetBones().data();

		renderable->m_pass = RootForce::RenderPass::RENDERPASS_DYNAMIC;
		renderable->m_material =  m_engineContext->m_resourceManager->GetMaterial("testchar");
		renderable->m_material->m_diffuseMap =  m_engineContext->m_resourceManager->LoadTexture("WStexture", Render::TextureType::TEXTURE_2D);
		renderable->m_material->m_normalMap =  m_engineContext->m_resourceManager->LoadTexture("WSNormal", Render::TextureType::TEXTURE_2D);
		renderable->m_material->m_specularMap =  m_engineContext->m_resourceManager->LoadTexture("WSSpecular", Render::TextureType::TEXTURE_2D);
		renderable->m_material->m_effect =  m_engineContext->m_resourceManager->LoadEffect("Mesh_NormalMap_Anim");

		transform->m_position = glm::vec3(0, 10, 0);

		playerControl->m_mouseSensitivity = 0.3f;
		playphys->MovementSpeed = 10.0f;
		playphys->JumpForce = 20.0f;

		action->Jump = false;
		action->MovePower = 0;
		action->StrafePower = 0;
		action->Angle = glm::vec2(0);
		action->ActivateAbility = false;

		ability->Abilities[0] = Ability::ABILITY_TEST;
		ability->SelectedAbility = Ability::ABILITY_TEST;

		physics->m_mass = 5.0f;
		collision->m_meshHandle = "testchar0";
		collision->m_handle = m_engineContext->m_physics->AddPlayerObjectToWorld(collision->m_meshHandle , entity->GetId(),
			transform->m_position, transform->m_orientation.GetQuaternion(), physics->m_mass, 10.0f, 0.0f, 0.1f, &collisionResponder->m_collidedEntityId);

		script->m_name = m_engineContext->m_resourceManager->LoadScript("Player");
	
		m_world->GetTagManager()->RegisterEntity("Player", entity);
		m_world->GetGroupManager()->RegisterEntity("NonExport", entity);

		//Create player aiming device
		ECS::Entity* aimingDevice = entityManager->CreateEntity();
		m_world->GetTagManager()->RegisterEntity("AimingDevice", aimingDevice);
		m_world->GetGroupManager()->RegisterEntity("NonExport", aimingDevice);

		RootForce::Transform* aimingDeviceTransform = entityManager->CreateComponent<RootForce::Transform>(aimingDevice);

		health->Health = 100;
		health->IsDead = false;
		health->WantsRespawn = true;
		score->Deaths = 0;
		score->Score = 0;
		identity->TeamID = p_teamID;
	}

	void PlayerSystem::Process()
	{

	}
}
#endif
