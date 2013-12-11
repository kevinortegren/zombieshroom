#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Components.h>
#include <Utility/ECS/Include/World.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	void PlayerSystem::CreatePlayer()
	{
		ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
		
		RootForce::Renderable* renderable = m_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(entity);
		RootForce::Transform* transform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);
		RootForce::PlayerControl* playerControl = m_world->GetEntityManager()->CreateComponent<RootForce::PlayerControl>(entity);
		RootForce::Player* player = m_world->GetEntityManager()->CreateComponent<RootForce::Player>(entity);
		RootForce::PhysicsAccessor* physics = m_world->GetEntityManager()->CreateComponent<RootForce::PhysicsAccessor>(entity);

		renderable->m_model = g_engineContext.m_resourceManager->LoadCollada("testchar");
		renderable->m_material.m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("DiffuseV1", Render::TextureType::TEXTURE_2D);
		renderable->m_material.m_normalMap = g_engineContext.m_resourceManager->LoadTexture("BumpV1", Render::TextureType::TEXTURE_2D);
		renderable->m_material.m_specularMap = g_engineContext.m_resourceManager->LoadTexture("SpecularV1", Render::TextureType::TEXTURE_2D);
		renderable->m_material.m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_NormalMap");

		playerControl->m_mouseSensitivity = 0.0f;
		playerControl->m_speed = 10.0f;

		player->m_abilities[0] = Abilitiy::ABILITY_TEST;
		player->m_selectedAbility = Abilitiy::ABILITY_TEST;

		float* pos = &transform->m_position.x;
		auto bullshit = transform->m_orientation.GetQuaterion().w;
		float* rot = &bullshit;

		physics->m_handle = g_engineContext.m_physics->AddPlayerObjectToWorld("testchar0", entity->GetId(), pos, rot, 5, 10.0f, 0.0f, 0.1f);

		m_world->GetTagManager()->RegisterEntity("Player", entity);
	}

	void PlayerSystem::Process()
	{

	}
}