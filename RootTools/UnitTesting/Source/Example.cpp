/*
	This is an example of how to write the unit tests.
*/
/*
#include <UnitTesting.h>

#include <RootEngine/Network/Include/NetworkManager.h>

#include <RootSystems/Include/Network/MessageHandler.h>

#include <RootForce/Include/RawMeshPrimitives.h>

TEST(TestTest, OfDoom) 
{
	EXPECT_NE(g_engineContext.m_network, nullptr);
	EXPECT_EQ(g_engineContext.m_network->GetNetworkSystem(), nullptr);
	g_engineContext.m_network->Initialize(RootEngine::Network::PeerType::LOCALSERVER);
	EXPECT_NE(g_engineContext.m_network->GetNetworkSystem(), nullptr);
}

TEST(TestTest, OfDoom2) 
{
	ECS::World* world = new ECS::World();
	EXPECT_NE(world, nullptr);
	RootForce::Network::MessageHandler messhand(world, g_engineContext.m_logger, g_engineContext.m_network, RootForce::Network::MessageHandler::DEDICATED, 5567);
	messhand.Update();
}
*/