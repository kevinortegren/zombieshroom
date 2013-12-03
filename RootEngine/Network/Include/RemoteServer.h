#pragma once
#include "Server.h"
namespace RootEngine
{
	namespace Network
	{
		class RemoteServer : public Server
		{
		public:
			RemoteServer();
			~RemoteServer();
			 bool Send( Message p_message );
			 bool ConnectTo( const char* p_ip , USHORT p_port);
		private:
			RakNet::RakPeerInterface* m_peerInterface;
		};
	}
}