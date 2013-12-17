#pragma once

#include <RakNet/RakPeerInterface.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <string>

namespace RootForce
{
	namespace Network
	{
		class Server
		{
		public:
			Server(Logging* p_logger, const std::string& p_name, unsigned short p_port, unsigned int p_maxConnections = 12);
			~Server();
			
			RootSystems::ServerInfo GetServerInfo() const;
			void SetServerInfo(const RootSystems::ServerInfo& p_info);

			void Update();
		private:
			Logging* m_logger;

			RootSystems::ServerInfo m_info;
			RakNet::RakPeerInterface* m_peer;

			void UpdatePingResponse();
		};
	}
}