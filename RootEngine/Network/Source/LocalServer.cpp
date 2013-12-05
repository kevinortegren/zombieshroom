#include <External/Include/RakNet/MessageIdentifiers.h>
#include <External/Include/RakNet/BitStream.h>
#include "LocalServer.h"

namespace RootEngine
{
	namespace Network
	{
		LocalServer::LocalServer()
		{
			for(int i = 0; i < MAX_CLIENTS + 1; i++)
				m_client[i] = nullptr;
		}
		LocalServer::~LocalServer()
		{
			delete[] &m_client;
		}

		bool LocalServer::Send( const Message& p_message )
		{
			if(p_message.RecipientID == -1)
			{
				return Transmit(p_message, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			else
			{
				return Transmit(p_message, m_client[p_message.RecipientID]->GUID, false);
			}
			return false;
		}

		void LocalServer::Host( USHORT p_port, bool p_isDedicated )
		{
			m_numClients = 0;
			RakNet::SocketDescriptor sd(p_port, 0);
			m_peerInterface = RakNet::RakPeerInterface::GetInstance();
			m_peerInterface->Startup(MAX_CLIENTS, &sd, 1);
			m_peerInterface->SetMaximumIncomingConnections(MAX_CLIENTS);

			if(!p_isDedicated)
			{
				Client* client = new Client;
				client->IsRemote = false;
				client->GUID = m_peerInterface->GetMyGUID(); //m_peerInterface->GetGuidFromSystemAddress("127.0.0.1");
				client->SysAddress = sd.hostAddress;
				m_client[1] = client;
				m_numClients++;
				
				Message* message = new Message;
				message->MessageID = InnerMessageID::CONNECT;
				message->RecipientID = 0;
				message->Reliability = PacketReliability::RELIABLE_ORDERED;
				message->Data = (uint8_t*)malloc(1); // TODO: Change maybe, perhaps. Problems...
				message->Data[0] = 1;
				message->DataSize = 1;
				m_message.push_back(message);
			}
		}

		void LocalServer::Update()
		{
			RakNet::Packet* packet;

			for(packet = m_peerInterface->Receive();
				packet;
				m_peerInterface->DeallocatePacket(packet), packet = m_peerInterface->Receive())
			{
				int8_t clientID = 1;
				for(; clientID < MAX_CLIENTS+1; clientID++)
					if(m_client[clientID] && packet->guid == m_client[clientID]->GUID)
						break;

				switch( packet->data[0] )
				{
				case ID_NEW_INCOMING_CONNECTION:
				// A new client is approaching. To arms!
					if( m_numClients >= MAX_CLIENTS )
					{
						// server full
						g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Client refused: server full. Client IP: %u.%u.%u.%u:%u.",
							packet->systemAddress.address.addr4.sin_addr.S_un.S_un_b.s_b1,
							packet->systemAddress.address.addr4.sin_addr.S_un.S_un_b.s_b2,
							packet->systemAddress.address.addr4.sin_addr.S_un.S_un_b.s_b3,
							packet->systemAddress.address.addr4.sin_addr.S_un.S_un_b.s_b4,
							packet->systemAddress.address.addr4.sin_port);
					}
					else
					{
						int id = 1;
						for(; id < MAX_CLIENTS+1; id++)
							if(m_client[id] == nullptr)
								break;
						m_client[id] = new Client();
						m_client[id]->GUID = packet->guid;
						m_client[id]->IsRemote = true;
						m_client[id]->SysAddress = packet->systemAddress;
						m_numClients++;

						Message* message = new Message;
						message->MessageID = InnerMessageID::CONNECT;
						message->RecipientID = 0;
						message->Reliability = PacketReliability::RELIABLE_ORDERED;
						message->Data = (uint8_t*)malloc(1);
						message->Data[0] = clientID;
						message->DataSize = 1;
						m_message.push_back(message);
					}
					break;
				case NON_RAKNET_MESSAGE_ID:
				// This is our own message, create a message struct from it and store in the buffer
					ParseNonRaknetPacket(packet, clientID);
					break;
				case ID_DISCONNECTION_NOTIFICATION:
				// A client decided to quit. A TRAITOR!
				case ID_CONNECTION_LOST:
				// A client was lost to the eternal sea of internet. How sad.
					{
						delete m_client[clientID];
						m_client[clientID] = nullptr;
						m_numClients--;

						Message* message = new Message;
						message->MessageID = InnerMessageID::DISCONNECT;
						message->RecipientID = 0;
						message->Reliability = PacketReliability::RELIABLE_ORDERED;
						message->Data = (uint8_t*)malloc(1);
						message->Data[0] = clientID;
						message->DataSize = 1;
						m_message.push_back(message);
					}
					break;
				default:
					break;
				}
			}
		}

	}
}