#pragma once

#include "NetworkInterface.h"
#include "Networker.h"

class NetworkManager : public NetworkInterface
{
public:
	void Initialize(PeerType::PeerType p_peerType);
	Networker* GetNetworkSystem() { return m_networkSys; }

private:
	Networker* m_networkSys;

};
