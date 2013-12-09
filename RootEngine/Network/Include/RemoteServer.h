#pragma once
#include "Server.h"
namespace RootEngine
{
	namespace Network
	{
		class RemoteServerInterface abstract
		{
			virtual bool ConnectTo( const char* p_ip , USHORT p_port) = 0;
		};

		class RemoteServer : public Server , public RemoteServerInterface
		{
		public:
			RemoteServer();
			~RemoteServer();
			void Update();
			bool Send( const Message& p_message );
			bool ConnectTo( const char* p_ip , USHORT p_port);
		};
	}
}