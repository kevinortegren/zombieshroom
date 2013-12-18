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
				NETWORK_DISCOVERY = 251,
				CONNECTION_REFUSED,
				CONNECTION_ACCEPTED,
				CONNECT,
				DISCONNECT
			};
		}

		struct Message
		{
			uint8_t MessageID;
			int8_t RecipientID;
			uint8_t SenderID;
			PacketReliability Reliability;
			uint32_t DataSize;
			uint8_t* Data;

			Message() 
			{ 
				Data = nullptr; 
			}
			
			Message(const Message& p_copy)
			{
				MessageID = p_copy.MessageID;
				RecipientID = p_copy.RecipientID;
				SenderID = p_copy.SenderID;
				Reliability = p_copy.Reliability;
				DataSize = p_copy.DataSize;
				Data = new uint8_t[DataSize];
				memcpy(Data, p_copy.Data, DataSize);
			}

			~Message() 
			{ 
				delete [] Data; 
			}

			Message& operator=(const Message& p_copy)
			{
				MessageID = p_copy.MessageID;
				RecipientID = p_copy.RecipientID;
				SenderID = p_copy.SenderID;
				Reliability = p_copy.Reliability;
				DataSize = p_copy.DataSize;
				Data = new uint8_t[DataSize];
				memcpy(Data, p_copy.Data, DataSize);

				return (*this);
			}
		};

		class ServerInterface abstract
		{
			virtual void Update() = 0;
			virtual bool Send(const Message& p_message) = 0;
			virtual Message* PollMessage() = 0;
			virtual void SendNetworkDiscoveryMessage(USHORT p_port) = 0;
			virtual void SetNetworkDiscoveryResponse( uint8_t* data, uint32_t size ) = 0;
		};

		const int8_t RECIPIENT_BROADCAST = -1;

		class Server abstract : public ServerInterface
		{
		public:
			Server(void);
			virtual ~Server(void) = 0;
			virtual void Update() = 0;
			virtual bool Send(const Message& p_message) = 0;
			Message* PollMessage();
			void SendNetworkDiscoveryMessage(USHORT p_port);
			void SetNetworkDiscoveryResponse( uint8_t* data, uint32_t size );
		protected:
			std::vector<Message*> m_message;
			bool Transmit(const Message& p_message, RakNet::RakNetGUID p_guid, bool p_broadcast); // ToDo: Find a more fitting name.
			RakNet::RakPeerInterface* m_peerInterface;

			void ParseNonRaknetPacket(RakNet::Packet* p_packet, uint8_t p_clientID);
			void ParseNetworkDiscoveryPacket(RakNet::Packet* p_packet);
		};
	}
}

