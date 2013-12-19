#pragma once

#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Network/LanList.h>
#include <RootSystems/Include/ChatSystem.h>

namespace RootForce
{
	namespace Network
	{
		class MessageHandler
		{
		public:
			MessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger);
			virtual ~MessageHandler();

			virtual void ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet) = 0;
		protected:
			RakNet::RakPeerInterface* m_peer;
			Logging* m_logger;
		};


		class ClientMessageHandler : public MessageHandler
		{
		public:
			ClientMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger);

			void SetLanList(RootSystems::LanList* p_list);
			void SetChatSystem(RootForce::ChatSystem* p_chatSystem);

			void ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		private:
			RootSystems::LanList* m_list;
			RootForce::ChatSystem* m_chatSystem;
		};

		class ServerMessageHandler : public MessageHandler
		{
		public:
			ServerMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger);

			void ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		};
	}
}