#include <UnitTesting.h>
#include <RakNet/BitStream.h>
#include <Utility/ECS/Include/EntityManager.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootSystems/Include/Network/Messages.h>

using namespace RootForce::Network;
using namespace RootForce::NetworkMessage;

TEST(Network, SerializeEntity)
{
	ECS::World* worldA = CreateWorld();
	ECS::World* worldB = CreateWorld();

	ECS::Entity* A;
	ECS::Entity* B;
}

/*
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
	ASSERT_TRUE(tB->m_position == tA->m_position);
	ASSERT_TRUE(tB->m_orientation.GetQuaternion() == tA->m_orientation.GetQuaternion());
	ASSERT_TRUE(tB->m_scale == tA->m_scale);
}
*/

TEST(Network, SequenceIDs)
{
	RootForce::Network::NetworkComponent::s_sequenceIDMap.clear();

	RootForce::Network::NetworkComponent n1(0, 0);
	RootForce::Network::NetworkComponent n2(0, 1);
	RootForce::Network::NetworkComponent n3(0, 1);
	RootForce::Network::NetworkComponent n4(&n3);

	ASSERT_EQ(n1.ID.SequenceID, 0);
	ASSERT_EQ(n2.ID.SequenceID, 0);
	ASSERT_EQ(n3.ID.SequenceID, 1);
	ASSERT_EQ(n4.ID.SequenceID, 2);
}

template <typename T>
bool SerializeAndCompare(T& m1)
{
	RakNet::BitStream bs;
	m1.Serialize(true, &bs);

	T m2;
	m2.Serialize(false, &bs);

	return memcmp(&m1, &m2, sizeof(T)) == 0;
}

TEST(Network, MessageSerializationChat)
{
	Chat m1;
	m1.Type = Chat::TYPE_SERVER_MESSAGE;
	m1.Sender = 4;
	m1.Message = "Foo bar";

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationUserConnected)
{
	UserConnected m1;
	m1.User = 4;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationUserDisconnected)
{
	UserDisconnected m1;
	m1.User = 4;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationPlayerCommand)
{
	PlayerCommand m1;
	m1.User = 4;
	m1.Action.MovePower = 1.0f;
	m1.Action.StrafePower = -1.0f;
	m1.Action.Jump = true;
	m1.Action.Angle = glm::vec2(1.5, 0.7);
	m1.Action.ActivateAbility = true;
	m1.Action.SelectedAbility = 2;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationDestroyEntities)
{
	DestroyEntities m1;
	m1.ID.UserID = 4;
	m1.ID.ActionID = 2;
	m1.ID.SequenceID = 18;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationSpawnUser)
{
	SpawnUser m1;
	m1.User = 4;
	m1.SpawnPointIndex = 43;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationLoadMap)
{
	LoadMap m1;
	m1.MapName = "GreenhouseBig";

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationLoadMapStatus)
{
	LoadMapStatus m1;
	m1.Status = LoadMapStatus::STATUS_FAILED_MAP_NOT_FOUND;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationSetMaxPlayers)
{
	SetMaxPlayers m1;
	m1.MaxPlayers = 14;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationSetGameMode)
{
	SetGameMode m1;
	m1.GameMode = 17;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationSetMatchTime)
{
	SetMatchTime m1;
	m1.Seconds = 1870;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationSetKillCount)
{
	SetKillCount m1;
	m1.Count = 445;

	ASSERT_TRUE(SerializeAndCompare(m1));
}

TEST(Network, MessageSerializationServerInformation)
{
	ServerInformation m1;
	m1.MapName = "HelloKitBig";
	m1.MaxPlayers = 775;
	m1.GameMode = 7;
	m1.MatchTimeSeconds = 7769;
	m1.KillCount = 47;

	ASSERT_TRUE(SerializeAndCompare(m1));
}