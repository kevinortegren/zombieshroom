#pragma once

#include <RakNet/RakPeerInterface.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Network/LanList.h>

namespace RootForce
{
	namespace Network
	{
		class Client
		{
		public:
			Client(Logging* p_logger);
			~Client();

			bool Connect(const char* p_address, unsigned short p_port);
			void PingNetwork(RootSystems::LanList* p_list, unsigned short p_port);

			void Update();
		private:
			Logging* m_logger;
			RakNet::RakPeerInterface* m_peer;
		};
	}
}