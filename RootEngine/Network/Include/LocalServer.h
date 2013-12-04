#pragma once
#include "Server.h"
namespace RootEngine
{
	namespace Network
	{
		struct Client
		{
			bool IsRemote;
			RakNet::SystemAddress SysAddress;
			RakNet::RakNetGUID GUID;
		};

		class LocalServer : public Server
		{
		public:
			LocalServer();
			~LocalServer();
			bool Send(Message p_message);
			void Host( USHORT p_port);
			void Update();
		private:
			Client* m_client[MAX_CLIENTS];
			uint8_t m_numClients;
		};
	}
}