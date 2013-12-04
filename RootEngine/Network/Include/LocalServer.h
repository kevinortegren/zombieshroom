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

		class LocalServer : public Server
		{
		public:
			LocalServer();
			~LocalServer();
			bool Send(Message p_message);
			void Host( USHORT p_port, bool p_isDedicated = true);
			void Update();
			void SetPacketCallback(PacketCallback p_callback) { m_callback = p_callback; };
		private:
			Client* m_client[MAX_CLIENTS+1]; // 0 is not to be used, as ID 0 equals server
			uint8_t m_numClients;
			PacketCallback m_callback;
		};
	}
}