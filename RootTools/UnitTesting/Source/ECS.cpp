#include <UnitTesting.h>
#include <Utility/ECS/Include/EntityManager.h>

class TestComponent1 : public ECS::Component<TestComponent1> {};
class TestComponent2 : public ECS::Component<TestComponent2> {};

TEST(ECS, GettingAllComponents)
{
	// Initialize a world and some components
	ECS::World* world = CreateWorld();

	TestComponent1::SetTypeId(0);
	TestComponent2::SetTypeId(1);

	// Create entities with different permutations of components
	ECS::Entity* e12 = world->GetEntityManager()->CreateEntity();
	ECS::Entity* e1 = world->GetEntityManager()->CreateEntity();
	ECS::Entity* e2 = world->GetEntityManager()->CreateEntity();
	
	world->GetEntityManager()->CreateComponent<TestComponent1>(e12);
	world->GetEntityManager()->CreateComponent<TestComponent2>(e12);
	world->GetEntityManager()->CreateComponent<TestComponent1>(e1);
	world->GetEntityManager()->CreateComponent<TestComponent2>(e2);

	// Retrieve all components
	auto r12 = world->GetEntityManager()->GetAllComponents(e12);
	auto r1 = world->GetEntityManager()->GetAllComponents(e1);
	auto r2 = world->GetEntityManager()->GetAllComponents(e2);

	// Make sure we get the correct number of components
	ASSERT_EQ(r12.size(), 2);
	ASSERT_EQ(r1.size(), 1);
	ASSERT_EQ(r2.size(), 1);

	// Make sure the components are of the proper type
	ASSERT_EQ(r12[0].first, 0);
	ASSERT_EQ(r12[1].first, 1);
	ASSERT_EQ(r1[0].first, 0);
	ASSERT_EQ(r2[0].first, 1);
}