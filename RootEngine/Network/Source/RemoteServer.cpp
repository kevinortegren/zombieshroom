#include <External/Include/RakNet/MessageIdentifiers.h>
#include "RemoteServer.h"

namespace RootEngine
{
	namespace Network
	{
		RemoteServer::RemoteServer()
		{
		}
		RemoteServer::~RemoteServer()
		{
		}

		bool RemoteServer::Send(const Message& p_message)
		{
			return Transmit(p_message, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}

		bool RemoteServer::ConnectTo( const char* p_ip , USHORT p_port)
		{
			RakNet::SocketDescriptor sd;
			m_peerInterface = RakNet::RakPeerInterface::GetInstance();
			m_peerInterface->Startup(1, &sd, 1);

			RakNet::ConnectionAttemptResult result = m_peerInterface->Connect( p_ip, p_port, 0, 0 );
			if(result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED)
				return true;
			return false;
		}

		void RemoteServer::Update()
		{
			RakNet::Packet* packet;

			for(packet = m_peerInterface->Receive();
				packet;
				m_peerInterface->DeallocatePacket(packet), packet = m_peerInterface->Receive())
			{

				switch( packet->data[0] )
				{
					case ID_CONNECTION_REQUEST_ACCEPTED:
					{
						Message* message = new Message;
						message->MessageID = InnerMessageID::CONNECTION_ACCEPTED;
						message->RecipientID = 0;
						message->Reliability = PacketReliability::RELIABLE_ORDERED;
						message->Data = nullptr;
						message->DataSize = 0;
						m_message.push(message);
					} break;

					case ID_CONNECTION_ATTEMPT_FAILED:
					{
						Message* message = new Message;
						message->MessageID = InnerMessageID::CONNECTION_REFUSED;
						message->RecipientID = 0;
						message->Reliability = PacketReliability::RELIABLE_ORDERED;
						message->Data = nullptr;
						message->DataSize = 0;
						m_message.push(message);
					} break;

					case NON_RAKNET_MESSAGE_ID:
						ParseNonRaknetPacket(packet, 0);
						break;

					case ID_DISCONNECTION_NOTIFICATION:
						// A client decided to quit. A TRAITOR!
					case ID_CONNECTION_LOST:
					{
						Message* message = new Message;
						message->MessageID = InnerMessageID::DISCONNECT;
						message->RecipientID = 0;
						message->Reliability = PacketReliability::RELIABLE_ORDERED;
						message->Data = new uint8_t[1];
						message->Data[0] = 0;
						message->DataSize = 1;
						m_message.push(message);
						break;
					}
				}
			}
		}

	}
}