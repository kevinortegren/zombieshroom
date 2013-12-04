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

		bool RemoteServer::Send(Message p_message)
		{
			return Transmit(p_message, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}

		bool RemoteServer::ConnectTo( const char* p_ip , USHORT p_port)
		{
			RakNet::SocketDescriptor sd;
			m_peerInterface = RakNet::RakPeerInterface::GetInstance();
			m_peerInterface->Startup(1, &sd, 1);

			g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Connecting to: %s:%u", p_ip, p_port);
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
				case NON_RAKNET_MESSAGE_ID:
					ParseNonRaknetPacket(packet);
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					// A client decided to quit. A TRAITOR!
				case ID_CONNECTION_LOST:
					Message* message = new Message;
					message->MessageID = InnerMessageID::DISCONNECT;
					message->RecipientID = 0;
					message->Reliability = PacketReliability::RELIABLE_ORDERED;
					message->Data = (uint8_t*)malloc(1);
					message->Data[0] = 0;
					message->DataSize = 1;
					m_message.push_back(message);
					break;
				}
			}
		}

	}
}