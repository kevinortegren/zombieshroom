#pragma once
#include <External/Include/RakNet/RakPeerInterface.h>
#include <External/Include/RakNet/PacketPriority.h>
#include <vector>
#include <stdint.h>
#include <RootEngine/Include/SubsystemSharedContext.h>

#define MAX_CLIENTS 12
#define NON_RAKNET_MESSAGE_ID (RakNet::MessageID)39
namespace RootEngine
{
	namespace Network
	{
		extern SubsystemSharedContext g_context;

		namespace InnerMessageID
		{
			enum InnerMessageID
			{
				CONNECT = 254,
				DISCONNECT
			};
		}

		struct Message
		{
			uint8_t MessageID;
			int8_t RecipientID;
			PacketReliability Reliability;
			uint64_t DataSize;
			uint8_t* Data;
		};
		class ServerInterface abstract
		{
			virtual void Update() = 0;
			virtual bool Send(Message p_message) = 0;
			virtual Message* PollMessage() = 0;
		};

		class Server abstract : public ServerInterface
		{
		public:
			Server(void);
			~Server(void);
			virtual void Update() = 0;
			virtual bool Send(Message p_message) = 0;
			Message* PollMessage();
		protected:
			std::vector<Message*> m_message;
			bool Transmit(Message p_message, RakNet::RakNetGUID p_guid, bool p_broadcast); // ToDo: Find a more fitting name.
			RakNet::RakPeerInterface* m_peerInterface;

			void ParseNonRaknetPacket(RakNet::Packet* p_packet);
		};
	}
}

