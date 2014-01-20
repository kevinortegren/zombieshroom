#include <UnitTesting.h>
#include <RakNet/BitStream.h>
#include <Utility/ECS/Include/EntityManager.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

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

	RootForce::Network::NetworkComponent n1;
	RootForce::Network::NetworkComponent n2;
	RootForce::Network::NetworkComponent n3;
	RootForce::Network::NetworkComponent n4;

	n1.SetID(0, 0);
	n2.SetID(0, 1);
	n3.SetID(0, 1);
	n4.SetID(&n3);

	ASSERT_EQ(n1.ID.SequenceID, 0);
	ASSERT_EQ(n2.ID.SequenceID, 0);
	ASSERT_EQ(n3.ID.SequenceID, 1);
	ASSERT_EQ(n4.ID.SequenceID, 2);
}

template <typename T>
void SerializeAndDeserialize(T& m1, T& m2)
{
	RakNet::BitStream bs;
	m1.Serialize(true, &bs);
	m2.Serialize(false, &bs);
}

TEST(Network, MessageSerializationChat)
{
	Chat m1;
	Chat m2;
	m1.Type = Chat::TYPE_SERVER_MESSAGE;
	m1.Sender = 4;
	m1.Message = "Foo bar";

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(m1.Type, m2.Type);
	ASSERT_EQ(m1.Sender, m2.Sender);
	ASSERT_EQ(strcmp(m1.Message.C_String(), m2.Message.C_String()), 0);
}

TEST(Network, MessageSerializationUserConnected)
{
	UserConnected m1;
	UserConnected m2;
	m1.User = 4;
	m1.Name = "Juada";

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(m1.User, m2.User);
	ASSERT_EQ(strcmp(m1.Name.C_String(), m2.Name.C_String()), 0);
}

TEST(Network, MessageSerializationUserDisconnected)
{
	UserDisconnected m1;
	UserDisconnected m2;
	m1.User = 4;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(UserDisconnected)), 0);
}

TEST(Network, MessageSerializationUserInformation)
{
	UserInformation m1;
	UserInformation m2;
	m1.Name = "Jakljdjanf";

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(strcmp(m1.Name.C_String(), m2.Name.C_String()), 0);
}

TEST(Network, MessageSerializationPlayerCommand)
{
	PlayerCommand m1;
	PlayerCommand m2;
	m1.User = 4;
	m1.Action.MovePower = 1.0f;
	m1.Action.StrafePower = -1.0f;
	m1.Action.Jump = true;
	m1.Action.Angle = glm::vec2(1.5, 0.7);
	m1.Action.ActivateAbility = true;
	m1.Action.SelectedAbility = 2;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(PlayerCommand)), 0);
}

TEST(Network, MessageSerializationDestroyEntities)
{
	DestroyEntities m1;
	DestroyEntities m2;
	m1.ID.UserID = 4;
	m1.ID.ActionID = 2;
	m1.ID.SequenceID = 18;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(DestroyEntities)), 0);
}

TEST(Network, MessageSerializationSpawnUser)
{
	SpawnUser m1;
	SpawnUser m2;
	m1.User = 4;
	m1.SpawnPointIndex = 43;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(SpawnUser)), 0);
}

TEST(Network, MessageSerializationLoadMap)
{
	LoadMap m1;
	LoadMap m2;
	m1.MapName = "GreenhouseBig";

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(strcmp(m1.MapName.C_String(), m2.MapName.C_String()), 0);
}

TEST(Network, MessageSerializationLoadMapStatus)
{
	LoadMapStatus m1;
	LoadMapStatus m2;
	m1.Status = LoadMapStatus::STATUS_FAILED_MAP_NOT_FOUND;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(LoadMapStatus)), 0);
}

TEST(Network, MessageSerializationSetMaxPlayers)
{
	SetMaxPlayers m1;
	SetMaxPlayers m2;
	m1.MaxPlayers = 14;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(SetMaxPlayers)), 0);
}

TEST(Network, MessageSerializationSetGameMode)
{
	SetGameMode m1;
	SetGameMode m2;
	m1.GameMode = 17;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(SetGameMode)), 0);
}

TEST(Network, MessageSerializationSetMatchTime)
{
	SetMatchTime m1;
	SetMatchTime m2;
	m1.Seconds = 1870;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(SetMatchTime)), 0);
}

TEST(Network, MessageSerializationSetKillCount)
{
	SetKillCount m1;
	SetKillCount m2;
	m1.Count = 445;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(memcmp(&m1, &m2, sizeof(SetKillCount)), 0);
}

TEST(Network, MessageSerializationServerInformation)
{
	ServerInformation m1;
	ServerInformation m2;
	m1.ServerName = "KJdfkaj";
	m1.MapName = "HelloKitBig";
	m1.CurrentPlayers = 556;
	m1.MaxPlayers = 775;
	m1.PasswordProtected = true;
	m1.GameMode = 7;
	m1.MatchTimeSeconds = 7769;
	m1.KillCount = 47;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(strcmp(m1.ServerName.C_String(), m2.ServerName.C_String()), 0);
	ASSERT_EQ(strcmp(m1.MapName.C_String(), m2.MapName.C_String()), 0);
	ASSERT_EQ(m1.CurrentPlayers, m2.CurrentPlayers);
	ASSERT_EQ(m1.MaxPlayers, m2.MaxPlayers);
	ASSERT_EQ(m1.PasswordProtected, m2.PasswordProtected);
	ASSERT_EQ(m1.GameMode, m2.GameMode);
	ASSERT_EQ(m1.MatchTimeSeconds, m2.MatchTimeSeconds);
	ASSERT_EQ(m1.KillCount, m2.KillCount);
}