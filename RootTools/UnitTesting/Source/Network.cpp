#include <UnitTesting.h>
#include <RakNet/BitStream.h>
#include <Utility/ECS/Include/EntityManager.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Components.h>

using namespace RootForce::Network;
using namespace RootForce::NetworkMessage;

#define SerializeComponentSetup(T)      \
	ECS::World* worldA = CreateWorld();	\
	ECS::World* worldB = CreateWorld();	\
	ECS::Entity* A;					    \
	ECS::Entity* B;						\
	T* cA;								\
	T* cB;								\
	A = worldA->GetEntityManager()->CreateEntity();		\
	B = worldB->GetEntityManager()->CreateEntity();		\
	cA = worldA->GetEntityManager()->CreateComponent<T>(A);	\
	cB = worldB->GetEntityManager()->CreateComponent<T>(B)

#define SerializeComponentSerialize()	\
	RakNet::BitStream bs;				\
	Serialize(true, &bs, cA);			\
	Serialize(false, &bs, cB)


TEST(Network, SerializeTransform)
{
	SerializeComponentSetup(RootForce::Transform);

	cA->m_position = glm::vec3(1.0f, 2.0f, 3.0f);
	cA->m_orientation.SetOrientation(glm::quat(1.0f, 2.0f, 3.0f, 4.0f));
	cA->m_scale = glm::vec3(1.0f, 2.0f, 3.0f);

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->m_position, cB->m_position);
	ASSERT_EQ(cA->m_orientation.GetQuaternion(), cB->m_orientation.GetQuaternion());
	ASSERT_EQ(cA->m_scale, cB->m_scale);
}

TEST(Network, SerializeHealthComponent)
{
	SerializeComponentSetup(RootForce::HealthComponent);
	
	cA->Health = 15;
	cA->IsDead = true;
	cA->LastDamageSourceID = 38;
	cA->RespawnDelay = 2.0f;
	cA->WantsRespawn = true;

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->Health, cB->Health);
	ASSERT_EQ(cA->IsDead, cB->IsDead);
	ASSERT_EQ(cA->LastDamageSourceID, cB->LastDamageSourceID);
	ASSERT_EQ(cA->RespawnDelay, cB->RespawnDelay);
	ASSERT_EQ(cA->WantsRespawn, cB->WantsRespawn);
}

TEST(Network, SerializePhysics)
{
	SerializeComponentSetup(RootForce::Physics);
	
	cA->m_mass = 3.0f;

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->m_mass, cB->m_mass);
}

TEST(Network, SerializeNetworkComponent)
{
	SerializeComponentSetup(RootForce::Network::NetworkComponent);
	
	cA->ID.UserID = 14;
	cA->ID.ActionID = 56;
	cA->ID.SequenceID = 37;
	//cA->AssociatedInNetworkEntityMap = true;	// Local value

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->ID.UserID, cB->ID.UserID);
	ASSERT_EQ(cA->ID.ActionID, cB->ID.ActionID);
	ASSERT_EQ(cA->ID.SequenceID, cB->ID.SequenceID);
	//ASSERT_EQ(cA->AssociatedInNetworkEntityMap, cB->AssociatedInNetworkEntityMap);
}

TEST(Network, SerializeLookAtBehavior)
{
	SerializeComponentSetup(RootForce::LookAtBehavior);
	
	cA->m_targetTag = "TinyTom";
	cA->m_displacement = glm::vec3(1.0f, 2.0f, 3.0f);

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->m_targetTag, cB->m_targetTag);
	ASSERT_EQ(cA->m_displacement, cB->m_displacement);
}

TEST(Network, SerializeScript)
{
	SerializeComponentSetup(RootForce::Script);
	
	cA->Name = "Destroy.lua";

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->Name, cB->Name);
}

TEST(Network, SerializePlayerComponent)
{
	SerializeComponentSetup(RootForce::PlayerComponent);
	
	cA->AbilityScripts[0] = "Ability.lua";
	cA->AbilityScripts[1] = "";
	cA->AbilityScripts[2] = "Test.lua";
	cA->SelectedAbility = 1;
	cA->Deaths = 99;
	cA->Score = 1;
	cA->Name = "Bill";
	cA->TeamID = 7;

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->AbilityScripts[0], cB->AbilityScripts[0]);
	ASSERT_EQ(cA->AbilityScripts[1], cB->AbilityScripts[1]);
	ASSERT_EQ(cA->AbilityScripts[2], cB->AbilityScripts[2]);
	ASSERT_EQ(cA->SelectedAbility, cB->SelectedAbility);
	ASSERT_EQ(cA->Deaths, cB->Deaths);
	ASSERT_EQ(cA->Score, cB->Score);
	ASSERT_EQ(cA->Name, cB->Name);
	ASSERT_EQ(cA->TeamID, cB->TeamID);
}

TEST(Network, SerializeTDMRuleSet)
{
	SerializeComponentSetup(RootForce::TDMRuleSet);
	
	cA->ScoreLimit = 65;
	cA->TeamScore[0] = 15;
	cA->TeamScore[1] = 64;
	cA->TeamScore[2] = 17;
	cA->TimeLeft = 15.0f;

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->ScoreLimit, cB->ScoreLimit);
	ASSERT_EQ(cA->TeamScore[0], cB->TeamScore[0]);
	ASSERT_EQ(cA->TeamScore[1], cB->TeamScore[1]);
	ASSERT_EQ(cA->TeamScore[2], cB->TeamScore[2]);
	ASSERT_EQ(cA->TimeLeft, cB->TimeLeft);
}

TEST(Network, SerializePlayerPhysics)
{
	SerializeComponentSetup(RootForce::PlayerPhysics);
	
	cA->MovementSpeed = 11.0f;
	cA->JumpForce = 20.0f;

	SerializeComponentSerialize();
	
	ASSERT_EQ(cA->MovementSpeed, cB->MovementSpeed);
	ASSERT_EQ(cA->JumpForce, cB->JumpForce);
}

TEST(Network, SerializeComponentExisting)
{
	ECS::World* worldA = CreateWorld();
	ECS::World* worldB = CreateWorld();
	ECS::Entity* A;
	ECS::Entity* B;
	RootForce::Transform* cA;
	RootForce::Transform* cB;
	A = worldA->GetEntityManager()->CreateEntity();
	B = worldB->GetEntityManager()->CreateEntity();
	cA = worldA->GetEntityManager()->CreateComponent<RootForce::Transform>(A);
	cB = worldB->GetEntityManager()->CreateComponent<RootForce::Transform>(B);

	cA->m_position = glm::vec3(1.0f, 2.0f, 3.0f);
	cA->m_orientation.SetOrientation(glm::quat(1.0f, 2.0f, 3.0f, 4.0f));
	cA->m_scale = glm::vec3(1.0f, 2.0f, 3.0f);

	RakNet::BitStream bs;
	SerializeComponent(&bs, cA, RootForce::ComponentType::TRANSFORM);
	DeserializeComponent(&bs, B, worldB->GetEntityManager());

	ASSERT_EQ(cA->m_position, cB->m_position);
	ASSERT_EQ(cA->m_orientation.GetQuaternion(), cB->m_orientation.GetQuaternion());
	ASSERT_EQ(cA->m_scale, cB->m_scale);
}

TEST(Network, SerializeComponentNonExisting)
{
	ECS::World* worldA = CreateWorld();
	ECS::World* worldB = CreateWorld();
	ECS::Entity* A;
	ECS::Entity* B;
	RootForce::Transform* cA;
	RootForce::Transform* cB;
	A = worldA->GetEntityManager()->CreateEntity();
	B = worldB->GetEntityManager()->CreateEntity();
	cA = worldA->GetEntityManager()->CreateComponent<RootForce::Transform>(A);
	//cB = worldB->GetEntityManager()->CreateComponent<RootForce::Transform>(B); // Deserialize should be able to create the component if not existing.

	cA->m_position = glm::vec3(1.0f, 2.0f, 3.0f);
	cA->m_orientation.SetOrientation(glm::quat(1.0f, 2.0f, 3.0f, 4.0f));
	cA->m_scale = glm::vec3(1.0f, 2.0f, 3.0f);

	RakNet::BitStream bs;
	SerializeComponent(&bs, cA, RootForce::ComponentType::TRANSFORM);
	DeserializeComponent(&bs, B, worldB->GetEntityManager());
	cB = worldB->GetEntityManager()->GetComponent<RootForce::Transform>(B);

	ASSERT_NE(cB, nullptr);
	ASSERT_EQ(cA->m_position, cB->m_position);
	ASSERT_EQ(cA->m_orientation.GetQuaternion(), cB->m_orientation.GetQuaternion());
	ASSERT_EQ(cA->m_scale, cB->m_scale);
}

TEST(Network, SerializeEntityExisting)
{
	ECS::World* worldA = CreateWorld();
	ECS::World* worldB = CreateWorld();
	NetworkEntityMap mapA;
	NetworkEntityMap mapB;
	ECS::Entity* A;
	ECS::Entity* B;
	RootForce::Transform* cA;
	RootForce::Transform* cB;
	A = worldA->GetEntityManager()->CreateEntity();
	B = worldB->GetEntityManager()->CreateEntity();
	
	NetworkEntityID id;
	id.UserID = 4;
	id.ActionID = 15;
	id.SequenceID = 33;
	mapA[id] = A;
	mapB[id] = B;


	g_world = worldA;
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("TestEntity"), "OnCreate");
	g_engineContext.m_script->AddParameterUserData(A, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->AddParameterNumber(id.UserID);
	g_engineContext.m_script->AddParameterNumber(id.ActionID);
	g_engineContext.m_script->ExecuteScript();

	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("TestEntity"), "AddClientComponents");
	g_engineContext.m_script->AddParameterUserData(A, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->ExecuteScript();

	cA = worldA->GetEntityManager()->GetComponent<RootForce::Transform>(A);
	cA->m_position = glm::vec3(1.0f, 2.0f, 3.0f);
	cA->m_orientation.SetOrientation(glm::quat(1.0f, 2.0f, 3.0f, 4.0f));
	cA->m_scale = glm::vec3(1.0f, 2.0f, 3.0f);


	g_world = worldB;
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("TestEntity"), "OnCreate");
	g_engineContext.m_script->AddParameterUserData(B, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->AddParameterNumber(id.UserID);
	g_engineContext.m_script->AddParameterNumber(id.ActionID);
	g_engineContext.m_script->ExecuteScript();

	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("TestEntity"), "AddClientComponents");
	g_engineContext.m_script->AddParameterUserData(B, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->ExecuteScript();

	cB = worldB->GetEntityManager()->GetComponent<RootForce::Transform>(B);


	RakNet::BitStream bs;
	ASSERT_TRUE(SerializeEntity(&bs, A, worldA->GetEntityManager(), mapA));
	ASSERT_NE(DeserializeEntity(&bs, worldB->GetEntityManager(), mapB), nullptr);

	ASSERT_EQ(cA->m_position, cB->m_position);
	ASSERT_EQ(cA->m_orientation.GetQuaternion(), cB->m_orientation.GetQuaternion());
	ASSERT_EQ(cA->m_scale, cB->m_scale);
}


TEST(Network, SerializeEntityNonExisting)
{
	ECS::World* worldA = CreateWorld();
	ECS::World* worldB = CreateWorld();
	NetworkEntityMap mapA;
	NetworkEntityMap mapB;
	ECS::Entity* A;
	ECS::Entity* B;
	RootForce::Transform* cA;
	RootForce::Transform* cB;
	A = worldA->GetEntityManager()->CreateEntity();
	
	NetworkEntityID id;
	id.UserID = 4;
	id.ActionID = 15;
	id.SequenceID = 33;
	mapA[id] = A;


	g_world = worldA;
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("TestEntity"), "OnCreate");
	g_engineContext.m_script->AddParameterUserData(A, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->AddParameterNumber(id.UserID);
	g_engineContext.m_script->AddParameterNumber(id.ActionID);
	g_engineContext.m_script->ExecuteScript();

	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("TestEntity"), "AddClientComponents");
	g_engineContext.m_script->AddParameterUserData(A, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->ExecuteScript();

	cA = worldA->GetEntityManager()->GetComponent<RootForce::Transform>(A);
	cA->m_position = glm::vec3(1.0f, 2.0f, 3.0f);
	cA->m_orientation.SetOrientation(glm::quat(1.0f, 2.0f, 3.0f, 4.0f));
	cA->m_scale = glm::vec3(1.0f, 2.0f, 3.0f);

	
	RakNet::BitStream bs;
	ASSERT_TRUE(SerializeEntity(&bs, A, worldA->GetEntityManager(), mapA));

	g_world = worldB;
	B = DeserializeEntity(&bs, worldB->GetEntityManager(), mapB);
	ASSERT_NE(B, nullptr);
	cB = worldB->GetEntityManager()->GetComponent<RootForce::Transform>(B);
	ASSERT_NE(cB, nullptr);

	ASSERT_EQ(cA->m_position, cB->m_position);
	ASSERT_EQ(cA->m_orientation.GetQuaternion(), cB->m_orientation.GetQuaternion());
	ASSERT_EQ(cA->m_scale, cB->m_scale);
}


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
	m1.IsYou = true;
	m1.Name = "Juada";

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(m1.User, m2.User);
	ASSERT_EQ(m1.IsYou, m2.IsYou);
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
	m1.IsDedicated = true;
	m1.GameMode = 7;
	m1.MatchTimeSeconds = 7769;
	m1.KillCount = 47;

	SerializeAndDeserialize(m1, m2);
	ASSERT_EQ(strcmp(m1.ServerName.C_String(), m2.ServerName.C_String()), 0);
	ASSERT_EQ(strcmp(m1.MapName.C_String(), m2.MapName.C_String()), 0);
	ASSERT_EQ(m1.CurrentPlayers, m2.CurrentPlayers);
	ASSERT_EQ(m1.MaxPlayers, m2.MaxPlayers);
	ASSERT_EQ(m1.PasswordProtected, m2.PasswordProtected);
	ASSERT_EQ(m1.IsDedicated, m2.IsDedicated);
	ASSERT_EQ(m1.GameMode, m2.GameMode);
	ASSERT_EQ(m1.MatchTimeSeconds, m2.MatchTimeSeconds);
	ASSERT_EQ(m1.KillCount, m2.KillCount);
}