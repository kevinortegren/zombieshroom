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
			m_peerInterface->Shutdown(0);
		}

		bool LocalServer::Send( const Message& p_message )
		{
			if(p_message.RecipientID == -1)
			{
				if(!m_client[1]->IsRemote)
					Transmit(p_message, m_client[1]->GUID, false);
					//m_message.push_back(new Message(p_message)); //TODO make sure this is necessary
				return Transmit(p_message, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			else if (p_message.RecipientID == 0)
			{
				// TODO: This is ugly. But need Sender ID for messages sent by local client.
				Message* m = new Message(p_message);
				m->SenderID = 1;
				m_message.push_back(m);
				return true;
			}
			else
			{
				if(!m_client[p_message.RecipientID]->IsRemote)
				{
					m_message.push_back(new Message(p_message)); //TODO make sure this is necessary
					return true;
				}
				else
					return Transmit(p_message, m_client[p_message.RecipientID]->GUID, false);
			}
			return false;
		}

		bool LocalServer::Host( USHORT p_port, bool p_isDedicated )
		{
			m_numClients = 0;
			RakNet::SocketDescriptor sd(p_port, 0);
			m_peerInterface = RakNet::RakPeerInterface::GetInstance();
			RakNet::StartupResult result = m_peerInterface->Startup(MAX_CLIENTS, &sd, 1);
			if( result != RakNet::StartupResult::RAKNET_STARTED )
			{
				g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::FATAL_ERROR, "Could not start a server on port: %u. Error #%u", p_port, result);
				return false;
			}

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
				message->SenderID = 1;
				message->RecipientID = 0;
				message->Reliability = PacketReliability::RELIABLE_ORDERED;
				message->Data = new uint8_t[1];
				message->Data[0] = 1;
				message->DataSize = 1;
				m_message.push_back(message);

				message = new Message;
				message->MessageID = InnerMessageID::CONNECTION_ACCEPTED;
				message->SenderID = 0;
				message->RecipientID = 0;
				message->Reliability = PacketReliability::RELIABLE_ORDERED;
				message->Data = nullptr;
				message->DataSize = 0;
				m_message.push_back(message);
			}
			g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Server started on port %u.", p_port);
			return true;
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
						g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Client refused: server full. Client IP: %s.", packet->systemAddress.ToString());
					}
					else
					{
						for(clientID = 1; clientID < MAX_CLIENTS+1; clientID++)
							if(m_client[clientID] == nullptr)
								break;
						m_client[clientID] = new Client();
						m_client[clientID]->GUID = packet->guid;
						m_client[clientID]->IsRemote = true;
						m_client[clientID]->SysAddress = packet->systemAddress;
						m_numClients++;

						Message* message = new Message;
						message->MessageID = InnerMessageID::CONNECT;
						message->SenderID = 0;
						message->RecipientID = 0;
						message->Reliability = PacketReliability::RELIABLE_ORDERED;
						message->Data = new uint8_t[1];
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
						message->SenderID = 0;
						message->Data = new uint8_t[1];
						message->Data[0] = clientID;
						message->DataSize = 1;
						m_message.push_back(message);
					}
					break;
				case ID_UNCONNECTED_PING:
				// Network discovery. A network has been detected. Yay!
					g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "A LAN-discovery request has been received from %s.", packet->systemAddress.ToString());
					break;
				case ID_UNCONNECTED_PONG:
				// Network discovery has been answered! Praise to the LAN-god!
					ParseNetworkDiscoveryPacket(packet);
					break;
				default:
					break;
				}
			}
		}

		bool LocalServer::IsClientLocal(size_t p_index) const
		{
			if (m_client[p_index] == nullptr)
				return false;
			return !m_client[p_index]->IsRemote;
		}

		std::vector<uint8_t> LocalServer::GetConnectedClients() const
		{
			std::vector<uint8_t> connectedClients;
			for (uint8_t i = 1; i < MAX_CLIENTS + 1; ++i)
			{
				if (m_client[i] != nullptr)
					connectedClients.push_back(i);
			}

			return connectedClients;
		}

	}
}