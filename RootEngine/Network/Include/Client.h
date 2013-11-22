#pragma once
#include "Networker.h"
namespace Network
{
	class Client :
		public Networker
	{
	public:
		Client(void);
		~Client(void);

		void ConnectTo( const char* p_ip, USHORT p_port = DEFAULT_PORT );
		void ProcessPacket( RakNet::Packet* p_packet );
	};
}

