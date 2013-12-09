#pragma once
#include "Server.h"
namespace RootEngine
{
	namespace Network
	{
		typedef void (*PacketCallback)(int8_t p_userID, uint8_t p_packetID );
		struct Client
		{
			bool IsRemote;
			RakNet::SystemAddress SysAddress;
			RakNet::RakNetGUID GUID;
		};

		class LocalServerInterface abstract
		{
			virtual void Host( USHORT p_port, bool p_isDedicated = true) = 0;
		};

		class LocalServer : public Server, public LocalServerInterface
		{
		public:
			LocalServer();
			~LocalServer();
			bool Send(const Message& p_message);
			void Host( USHORT p_port, bool p_isDedicated = true);
			void Update();
		private:
			Client* m_client[MAX_CLIENTS+1]; // 0 is not to be used, as ID 0 equals server
			uint8_t m_numClients;
		};
	}
}