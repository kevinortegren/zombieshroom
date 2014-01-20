#include "UnitTesting.h"
#include <RootSystems/Include/ActionSystem.h>
#include <RootSystems/Include/PlayerSystem.h>

TEST(ActionSystem, ProcessEmptyEntity) 
{
	ECS::World* world = CreateWorld();

	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();
	RootSystems::ActionSystem* system = new RootSystems::ActionSystem(world, &g_engineContext);
	world->GetSystemManager()->AddSystem<RootSystems::ActionSystem>(system, "ActionSystem");
	// Will test that an empty entity (an entity missing the necessary components does not crash the system
	system->Process();
	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}

TEST(ActionSystem, ProcessEntity) 
{
	ECS::World* world = CreateWorld();
	g_world = world;

	RootSystems::ActionSystem* system = new RootSystems::ActionSystem(world, &g_engineContext);
	RootForce::PhysicsSystem* pSystem = new RootForce::PhysicsSystem(world); 
	pSystem->SetPhysicsInterface(g_engineContext.m_physics);
	pSystem->SetLoggingInterface(g_engineContext.m_logger);
	world->GetSystemManager()->AddSystem<RootSystems::ActionSystem>(system, "ActionSystem");
	world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(pSystem, "PhysicsSystem");

	RootForce::PlayerSystem(world, &g_engineContext).CreatePlayer(0);
	g_engineContext.m_resourceManager->LoadScript("AbilityTest");
	ECS::Entity* testity = world->GetTagManager()->GetEntityByTag("Player");

	RootForce::PlayerActionComponent* action = world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(testity);
	RootForce::HealthComponent* health = world->GetEntityManager()->GetComponent<RootForce::HealthComponent>(testity);
	RootForce::Player* player = world->GetEntityManager()->GetComponent<RootForce::Player>(testity);
	RootForce::Transform* transform = world->GetEntityManager()->GetComponent<RootForce::Transform>(testity);
	g_engineContext.m_physics->CreatePlane(glm::vec3(0,1,0),glm::vec3(0,-50,0)); 
	
	health->RespawnDelay = 3.0f;
	action->MovePower = 1;
	// Will test that an empty entity (an entity missing the necessary components does not crash the system

	{
		health->IsDead = true;
		action->Jump = true;
		action->ActivateAbility = true;

		system->Process();
		g_engineContext.m_physics->Update(0.01f);
		pSystem->Process();

		EXPECT_TRUE(health->WantsRespawn);
		EXPECT_FALSE(action->Jump);
		EXPECT_FALSE(action->ActivateAbility);
		health->IsDead = false;
	}

	//Make sure we are not falling when testing the positioning
	for(int i = 0; i < 1000; i++)
	{
		g_engineContext.m_physics->Update(0.01f);
		pSystem->Process();
	}

	{
		action->MovePower = 0;
		action->StrafePower = 0;
		glm::vec3 pos = transform->m_position;

		system->Process();
		g_engineContext.m_physics->Update(0.01f);
		pSystem->Process();

		EXPECT_EQ(transform->m_position.x, pos.x);
		EXPECT_EQ(transform->m_position.y, pos.y);
		EXPECT_EQ(transform->m_position.z, pos.z);
	}

	{
		action->MovePower = 1;
		action->StrafePower = -1;
		glm::vec3 pos = transform->m_position;

		system->Process();
		g_engineContext.m_physics->Update(0.01f);
		pSystem->Process();

		EXPECT_NE(transform->m_position.x, pos.x);
		EXPECT_EQ(transform->m_position.y, pos.y);
		EXPECT_NE(transform->m_position.z, pos.z);
	}

	{
		glm::vec3 pos = transform->m_position;
		action->Jump = true;

		system->Process();
		g_engineContext.m_physics->Update(0.01f);
		pSystem->Process();

		EXPECT_GT(transform->m_position.y, pos.y);
		EXPECT_FALSE(action->Jump);
	}

	{
		action->ActivateAbility = true;
		system->Process();
		g_engineContext.m_physics->Update(0.01f);
		pSystem->Process();
		EXPECT_FALSE(action->ActivateAbility);
	}

	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}