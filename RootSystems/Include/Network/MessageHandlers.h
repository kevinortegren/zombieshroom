#pragma once

#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>

namespace RootForce
{
	namespace Network
	{
		class MessageHandler
		{
		public:
			virtual ~MessageHandler();

			virtual void ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet) = 0;
		};


		class ClientMessageHandler : public MessageHandler
		{
		public:
			void ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		};

		class ServerMessageHandler : public MessageHandler
		{
		public:
			void ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		};
	}
}