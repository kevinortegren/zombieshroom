#include "UnitTesting.h"
#include <RootEngine/Script/Include/RootScript.h>

TEST(MatchStateSystem, AwardPlayerKill) 
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	// Call the OnCreate script
	g_networkEntityMap.clear();
	RootForce::Network::NetworkComponent::s_sequenceIDMap.clear();
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("MatchState"), "OnCreate");
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedUserID::NONE);
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::NONE);
	g_engineContext.m_script->ExecuteScript();

	// Create a player entity
	g_engineContext.m_script->SetGlobalNumber("UserID", 0);
	g_engineContext.m_script->SetGlobalNumber("IsClient", false);
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::CONNECT);
	g_engineContext.m_script->ExecuteScript();

	// Get the newly created player
	ECS::Entity* player = world->GetTagManager()->GetEntityByTag("Player");
	RootForce::PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<RootForce::PlayerComponent>(player);

	{
		EXPECT_EQ(playerComponent->Deaths, 0);

		// Force the player to commit a suicide
		RootForce::MatchStateSystem::AwardPlayerKill(RootForce::Network::ReservedUserID::NONE, 0); // 0 is the ID of the test player

		EXPECT_EQ(playerComponent->Deaths, 1);
	}

	{
		// Create another player entity
		g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
		g_engineContext.m_script->AddParameterNumber(1);
		g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::CONNECT);
		g_engineContext.m_script->ExecuteScript();

		// Make the second player kill the first one, yay!
		RootForce::MatchStateSystem::AwardPlayerKill(1, 0); // 1 is the ID of the 2nd player, 0 is the ID of the 1st.

		RootForce::PlayerComponent* secondPlayerComponent = g_world->GetEntityManager()->GetComponent<RootForce::PlayerComponent>(
			g_networkEntityMap[
				RootForce::Network::NetworkEntityID(
					1,
					RootForce::Network::ReservedActionID::CONNECT,
					0
				)
			]
		);
		
		EXPECT_EQ(secondPlayerComponent->Score, 1);
		EXPECT_EQ(playerComponent->Deaths, 2);
	}

	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}

TEST(MatchStateSystem, GetScoreList) 
{
	ECS::World* world = CreateWorld();
	g_world = world;
	g_networkEntityMap.clear();

	// Call the OnCreate script
	g_networkEntityMap.clear();
	RootForce::Network::NetworkComponent::s_sequenceIDMap.clear();
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("MatchState"), "OnCreate");
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedUserID::NONE);
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::NONE);
	g_engineContext.m_script->ExecuteScript();

	// Create a player entity
	g_engineContext.m_script->SetGlobalNumber("UserID", 0);
	g_engineContext.m_script->SetGlobalNumber("IsClient", false);
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::CONNECT);
	g_engineContext.m_script->ExecuteScript();

	// Get the newly created player
	ECS::Entity* player = world->GetTagManager()->GetEntityByTag("Player");
	RootForce::PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<RootForce::PlayerComponent>(player);
	playerComponent->Name = "Test1";
	playerComponent->TeamID = 1;
	playerComponent->Score = 10;
	playerComponent->Deaths = 9;

	// Create another player entity
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
	g_engineContext.m_script->AddParameterNumber(1);
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::CONNECT);
	g_engineContext.m_script->ExecuteScript();

	RootForce::PlayerComponent* secondPlayerComponent = g_world->GetEntityManager()->GetComponent<RootForce::PlayerComponent>(
		g_networkEntityMap[
			RootForce::Network::NetworkEntityID(
				1,
				RootForce::Network::ReservedActionID::CONNECT,
				0
			)
		]
	);
	secondPlayerComponent->Name = "Test2";
	secondPlayerComponent->TeamID = 1;
	secondPlayerComponent->Score = 11;
	secondPlayerComponent->Deaths = 9;

	// Create another player entity
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
	g_engineContext.m_script->AddParameterNumber(2);
	g_engineContext.m_script->AddParameterNumber(RootForce::Network::ReservedActionID::CONNECT);
	g_engineContext.m_script->ExecuteScript();

	RootForce::PlayerComponent* thirdPlayerComponent = g_world->GetEntityManager()->GetComponent<RootForce::PlayerComponent>(
		g_networkEntityMap[
			RootForce::Network::NetworkEntityID(
				2,
				RootForce::Network::ReservedActionID::CONNECT,
				0
			)
		]
	);
	thirdPlayerComponent->Name = "Test3";
	thirdPlayerComponent->TeamID = 1;
	thirdPlayerComponent->Score = 11;
	thirdPlayerComponent->Deaths = 11;

	{
		std::string scoreList = RootForce::MatchStateSystem::GetScoreList();
		EXPECT_EQ(scoreList, "[[1,'Test2',11,9],[1,'Test3',11,11],[1,'Test1',10,9],]");
	}

	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}