#include <UnitTesting.h>
#include <RakNet/BitStream.h>
#include <Utility/ECS/Include/EntityManager.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>

TEST(Network, SerializeEntity)
{
	RakNet::BitStream bs;
	
	ECS::World* worldA = CreateWorld();
	ECS::World* worldB = CreateWorld();

	RootForce::Network::NetworkEntityMap mapA;
	RootForce::Network::NetworkEntityMap mapB;
	
	ECS::Entity* A;
	ECS::Entity* B;

	A = worldA->GetEntityManager()->CreateEntity();
	RootForce::Transform* tA = worldA->GetEntityManager()->CreateComponent<RootForce::Transform>(A);
	tA->m_position = glm::vec3(1.0f, 2.0f, 3.0f);
	tA->m_orientation.SetOrientation(glm::quat(1.0f, 2.0f, 3.0f, 4.0f));
	tA->m_scale = glm::vec3(1.0f, 2.0f, 3.0f);
	RootForce::Network::SerializeEntity(&bs, A, "Player", mapA, worldA);

	B = RootForce::Network::DeserializeEntity(&bs, mapB, worldB);

	ASSERT_EQ(worldB->GetEntityManager()->GetNumEntities(), 1);
	ASSERT_NE(worldB->GetEntityManager()->GetComponent<RootForce::Transform>(B), nullptr);
	
	RootForce::Transform* tB = worldB->GetEntityManager()->GetComponent<RootForce::Transform>(B);
	ASSERT_EQ(tB->m_position, tA->m_position);
	ASSERT_EQ(tB->m_orientation, tA->m_orientation);
	ASSERT_EQ(tB->m_scale, tA->m_scale);
}