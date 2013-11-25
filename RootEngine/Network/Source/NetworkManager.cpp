#include "NetworkManager.h"
#include "Client.h"
#include "Server.h"
#include <iostream>
#include <gtest/gtest.h>
namespace RootEngine
{
	namespace Network
	{
		SubsystemSharedContext g_context;
		NetworkManager* NetworkManager::s_networkManager = nullptr;
		void NetworkManager::Startup()
		{

		}

		void NetworkManager::Shutdown()
		{
			delete m_networkSys;
			delete s_networkManager;
		}

		void NetworkManager::Initialize(PeerType::PeerType p_peerType)
		{
			switch( p_peerType )
			{
			case PeerType::SERVER:
				m_networkSys = new Server();
				break;
			case PeerType::CLIENT:
				m_networkSys = new Client();
				break;
			default:
				// Log error
				break;
			}
		}

		NetworkManager* NetworkManager::GetInstance()
		{
			if(!s_networkManager)
				s_networkManager = new NetworkManager();

			return s_networkManager;
		}

		
		
	}
}

RootEngine::Network::NetworkInterface* CreateNetwork(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::Network::g_context = p_context;

	return RootEngine::Network::NetworkManager::GetInstance();
}

TEST(NETWORK, NETWORK_SEND)
{
	EXPECT_TRUE(RootEngine::Network::NetworkManager::GetInstance()->GetNetworkSystem()->Send( "I AM A TEST/POTATOE GOD" ));
}
