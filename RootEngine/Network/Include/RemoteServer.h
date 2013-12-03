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
		private:
		};
	}
}