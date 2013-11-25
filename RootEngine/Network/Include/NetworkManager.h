#pragma once

#include "NetworkInterface.h"
#include "Networker.h"
namespace Network
{
	class NetworkManager : public NetworkInterface
	{
	public:
		void Startup();
		void Shutdown();

		void Initialize(PeerType::PeerType p_peerType);
		Networker* GetNetworkSystem() { return m_networkSys; }

	private:
		Networker* m_networkSys;

	};
}
