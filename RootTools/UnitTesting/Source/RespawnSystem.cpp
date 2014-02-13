#include "UnitTesting.h"
#include <RootSystems/Include/RespawnSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootEngine/Script/Include/RootScript.h>

TEST(RespawnSystem, ProcessEmptyEntity)
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();
	RootSystems::RespawnSystem* system = new RootSystems::RespawnSystem(world);
	world->GetSystemManager()->AddSystem<RootSystems::RespawnSystem>(system);
	// Will test that an empty entity (an entity missing the necessary components does not crash the system
	system->Process();
	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}

TEST(RespawnSystem, ProcessEntity)
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	ECS::Entity* mockSpawn = world->GetEntityManager()->CreateEntity();
	RootForce::Transform* mockSpawnTransform = world->GetEntityManager()->CreateComponent<RootForce::Transform>(mockSpawn);
	mockSpawnTransform->m_position = glm::vec3(5, 5, 10);
	world->GetGroupManager()->RegisterEntity("SpawnPoint", mockSpawn);

	RootSystems::RespawnSystem* system = new RootSystems::RespawnSystem(world);
	world->GetSystemManager()->AddSystem<RootSystems::RespawnSystem>(system);

	// Call the OnCreate script
	g_engineContext.m_script->SetGlobalNumber("UserID", 0);
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
	//g_engineContext.m_script->AddParameterUserData(testity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->ExecuteScript();
	
	ECS::Entity* testity = world->GetTagManager()->GetEntityByTag("Player");

	RootForce::HealthComponent* health = world->GetEntityManager()->GetComponent<RootForce::HealthComponent>(testity);

	{
		health->Health = 0;
		health->IsDead = false;
		world->SetDelta(0.1f);
		system->Process();

		EXPECT_TRUE(health->IsDead);
		EXPECT_GT(health->RespawnDelay, 0.0f);
		EXPECT_LT(health->RespawnDelay, 3.0f);
	}

	{
		health->WantsRespawn = true;
		health->RespawnDelay = 0.0f;
		system->Process();
		EXPECT_FALSE(health->IsDead);
		EXPECT_EQ(health->Health, 100);
		EXPECT_FALSE(health->WantsRespawn);
	}

	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;

}