#pragma once
#include "Server.h"
#include "Client.h"
namespace RootEngine
{
	namespace Network
	{
		class LocalServer : public Server
		{
		public:
			LocalServer();
			~LocalServer();
			bool Send(Message p_message);
		private:
			Client* m_client[MAX_CLIENTS];
		};
	}
}