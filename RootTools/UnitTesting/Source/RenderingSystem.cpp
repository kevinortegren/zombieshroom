#include "UnitTesting.h"
#include <RootSystems/Include/RenderingSystem.h>

TEST(RenderingSystem, ProcessEmptyEntity) 
{
	ECS::World* world = CreateWorld();

	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();
	RootForce::RenderingSystem* system = new RootForce::RenderingSystem(world);
	//system->Init();
	world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(system, "RenderingSystem");
	// Will test that an empty entity (an entity missing the necessary components does not crash the system
	system->Process();
	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}