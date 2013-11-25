#include <NetworkManager.h>
#include <Client.h>
#include <Server.h>
#include <iostream>
namespace Network
{
	void NetworkManager::Startup()
	{

	}

	void NetworkManager::Shutdown()
	{

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
}

Network::NetworkInterface* GetNetworkInterface(RootEngine::SubsystemSharedContext p_context)
{ 
	static Network::NetworkManager s_nm; 
	return &s_nm;
}
