#pragma once
#include "Networker.h"

#define MAX_CLIENTS 12
namespace Network
{
	class Server :
		public Networker
	{
	public:
		Server(void);
		~Server(void);

		void Host( USHORT p_port = DEFAULT_PORT );
		void ProcessPacket( RakNet::Packet* p_packet );
	};
}

