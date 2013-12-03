#include <External/Include/RakNet/MessageIdentifiers.h>
#include <External/Include/RakNet/BitStream.h>
#include "LocalServer.h"

namespace RootEngine
{
	namespace Network
	{
		LocalServer::LocalServer()
		{
		}
		LocalServer::~LocalServer()
		{
		}

		bool LocalServer::Send( Message p_message )
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

		void LocalServer::Host( USHORT p_port )
		{
			g_context.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Starting server on port %u.", p_port);
			m_numClients = 0;
			RakNet::SocketDescriptor sd(p_port, 0);

			m_peerInterface->Startup(MAX_CLIENTS, &sd, 1);
			m_peerInterface->SetMaximumIncomingConnections(MAX_CLIENTS);
		}

		void LocalServer::Update()
		{
			RakNet::Packet* packet;

			for(packet = m_peerInterface->Receive();
				packet;
				m_peerInterface->DeallocatePacket(packet), packet = m_peerInterface->Receive())
			{
				switch( packet->data[0] )
				{
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
				// A new client is approaching. To arms!
					if( m_numClients >= MAX_CLIENTS )
					{
						// server full
					}
					else
					{
						int id = 0;
						for(; id < MAX_CLIENTS; id++)
							if(m_client[id] == nullptr)
								break;
						m_client[id] = new Client();
						m_client[id]->GUID = packet->guid;
						m_client[id]->IsRemote = true;
						m_client[id]->SysAddress = packet->systemAddress;
					}
					break;
				case NON_RAKNET_MESSAGE_ID:
				// This is our own message, create a message struct from it and store in the buffer
					{
						Message* message = new Message;
						RakNet::BitStream bitstream(packet->data, packet->length, false);
						bitstream.IgnoreBytes(1); // skip reading data[0] again
						bitstream.Read(message->RecipientID);
						bitstream.Read(message->MessageID);
						bitstream.Read(message->DataSize);
						bitstream.Read((char*)message->Data, (unsigned int)message->DataSize);

						m_message.push_back(message);
					}
					break;
				case ID_DISCONNECTION_NOTIFICATION:
				// A client decided to quit. A TRAITOR!
				case ID_CONNECTION_LOST:
				// A client was lost to the eternal sea of internet. How sad.
					// ToDo: Handle disconnect and timeout
					// ? switch from Client*[] to std::map<RakNetGUID,Client*> for the sake of simplicity?
					break;
				default:
					break;
				}
			}
		}

	}
}