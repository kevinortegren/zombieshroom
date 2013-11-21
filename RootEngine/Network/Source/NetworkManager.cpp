#include <NetworkManager.h>
#include <Client.h>
#include <Server.h>
#include <iostream>

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

NetworkInterface* APIENTRY GetNetworkInterface() { static NetworkManager s_nm; return &s_nm; }
