#pragma once
#include <External/Include/RakNet/RakPeerInterface.h>
#include "Server.h"
namespace RootEngine
{
	namespace Network
	{
		class Client
		{
		public:
			Client(RakNet::SystemAddress p_sysAddress);
			~Client(void);
			bool Send( Message p_message );
			bool IsRemote() { return m_peerInterface != nullptr; }
		private:
			RakNet::RakPeerInterface* m_peerInterface;
			RakNet::SystemAddress m_sysAddress;
		};
	}
}

