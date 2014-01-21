#include <UnitTesting.h>
#include <Utility/ECS/Include/EntityManager.h>
#include <RootSystems/Include/Components.h>

TEST(ECS, GettingAllComponents)
{
	// Initialize a world and some components
	ECS::World* world = CreateWorld();

	// Create entities with different permutations of components
	ECS::Entity* e12 = world->GetEntityManager()->CreateEntity();
	ECS::Entity* e1 = world->GetEntityManager()->CreateEntity();
	ECS::Entity* e2 = world->GetEntityManager()->CreateEntity();
	
	world->GetEntityManager()->CreateComponent<RootForce::Transform>(e12);
	world->GetEntityManager()->CreateComponent<RootForce::HealthComponent>(e12);
	world->GetEntityManager()->CreateComponent<RootForce::Transform>(e1);
	world->GetEntityManager()->CreateComponent<RootForce::HealthComponent>(e2);

	// Retrieve all components
	auto r12 = world->GetEntityManager()->GetAllComponents(e12);
	auto r1 = world->GetEntityManager()->GetAllComponents(e1);
	auto r2 = world->GetEntityManager()->GetAllComponents(e2);

	// Make sure we get the correct number of components
	ASSERT_EQ(r12.size(), 2);
	ASSERT_EQ(r1.size(), 1);
	ASSERT_EQ(r2.size(), 1);

	// Make sure the components are of the proper type
	ASSERT_EQ(r12[0].first, RootForce::Transform::GetTypeId());
	ASSERT_EQ(r12[1].first, RootForce::HealthComponent::GetTypeId());
	ASSERT_EQ(r1[0].first, RootForce::Transform::GetTypeId());
	ASSERT_EQ(r2[0].first, RootForce::HealthComponent::GetTypeId());
}

TEST(ECS, NullComponent)
{
	// Initialize a world and some components
	ECS::World* world = CreateWorld();

	// Create an entity
	ECS::Entity* e = world->GetEntityManager()->CreateEntity();
	
	// Assert that trying to get a component returns null
	ASSERT_EQ(world->GetEntityManager()->GetComponent<RootForce::Transform>(e), nullptr);

	// Add component and test again
	world->GetEntityManager()->CreateComponent<RootForce::Transform>(e);

	ASSERT_NE(world->GetEntityManager()->GetComponent<RootForce::Transform>(e), nullptr);
}