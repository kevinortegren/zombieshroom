#include <Network/MessageHandler.h>
#include <Network/Messages.h>
#include <Network/ServerInfo.h>
#include <RootEngine/Network/Include/LocalServer.h>
#include <RootEngine/Network/Include/RemoteServer.h>
#include <algorithm>

namespace RootForce
{
	namespace Network
	{
		






		MessageHandler::MessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::NetworkInterface* p_networkInterface, ServerType p_type, int16_t port, const char* address)
			: m_world(p_world)
			, m_logger(p_logger)
		{
			switch (p_type)
			{
				case MessageHandler::LOCAL:
					p_networkInterface->Initialize(RootEngine::Network::PeerType::LOCALSERVER);
					m_server = p_networkInterface->GetNetworkSystem();
					if(!dynamic_cast<RootEngine::Network::LocalServer*>(m_server)->Host(port, false))
					{
						return;
					}

					m_server->SendNetworkDiscoveryMessage(port);
					{
						RootSystems::ServerInfo info;
						strcpy(info.Name, "Local server");
						info.MaxPlayers = 12;
						info.NumPlayers = 2;
						info.PasswordProtected = false;
						m_server->SetNetworkDiscoveryResponse((uint8_t*)&info, sizeof(info));
					}
					
					m_clientMessageHandler = new ClientMessageHandler(p_world, m_logger, m_server);
					m_serverMessageHandler = new ServerMessageHandler(p_world, m_logger, dynamic_cast<RootEngine::Network::LocalServer*>(m_server));

					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Started local server on port: %u", port);
					break;
				case MessageHandler::REMOTE:
				{
					p_networkInterface->Initialize(RootEngine::Network::PeerType::REMOTESERVER);
					m_server = p_networkInterface->GetNetworkSystem();

					bool connectionSuccess = dynamic_cast<RootEngine::Network::RemoteServer*>(m_server)->ConnectTo(address, port);
					m_server->SendNetworkDiscoveryMessage(port);

					if (connectionSuccess)
					{
						m_clientMessageHandler = new ClientMessageHandler(p_world, m_logger, m_server);
						m_serverMessageHandler = nullptr;

						m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Started remote server connection to: %s:%u", address, port);
					}
					else
					{
						m_logger->LogText(LogTag::NETWORK, LogLevel::FATAL_ERROR, "Failed to start remote server connection to: %s:%u", address, port);
						throw std::runtime_error("");
					}	
				} break;
				case MessageHandler::DEDICATED:
					p_networkInterface->Initialize(RootEngine::Network::PeerType::LOCALSERVER);
					m_server = p_networkInterface->GetNetworkSystem();
					dynamic_cast<RootEngine::Network::LocalServer*>(m_server)->Host(port, true);

					{
						RootSystems::ServerInfo info;
						strcpy(info.Name, "Dedicated server");
						info.MaxPlayers = 16;
						info.NumPlayers = 0;
						info.PasswordProtected = true;
						m_server->SetNetworkDiscoveryResponse((uint8_t*)&info, sizeof(info));
					}

					m_clientMessageHandler = nullptr;
					m_serverMessageHandler = new ServerMessageHandler(p_world, m_logger, dynamic_cast<RootEngine::Network::LocalServer*>(m_server));

					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Started dedicated local server on port: %u", port);
					break;
				default:
					m_logger->LogText(LogTag::NETWORK, LogLevel::FATAL_ERROR, "Invalid server type");
			}
		}

		void MessageHandler::Update()
		{
			if(m_clientMessageHandler != nullptr)
				m_clientMessageHandler->Update();
			 
			m_server->Update();
			std::shared_ptr<RootEngine::Network::Message> message = nullptr;
			while ((message = std::shared_ptr<RootEngine::Network::Message>(m_server->PollMessage())) != nullptr)
			{
				switch (message->MessageID)
				{
					case Network::MessageType::GameStateSnapshot: 
					case Network::MessageType::ChatToClient:
					case Network::MessageType::UserConnected:
					case Network::MessageType::UserDisconnected:
					case RootEngine::Network::InnerMessageID::CONNECTION_ACCEPTED:
					case RootEngine::Network::InnerMessageID::CONNECTION_REFUSED:
						if (m_clientMessageHandler != nullptr)
							m_clientMessageHandler->HandleClientMessage(message.get());
						else
							m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "Received client message as a dedicated server");
						break;

					case Network::MessageType::ChatToServer:
					case Network::MessageType::UserInfo:
					case Network::MessageType::UserCommandMoveForward:
					case Network::MessageType::UserCommandMoveBackward:
					case Network::MessageType::UserCommandStrafeRight:
					case Network::MessageType::UserCommandStrafeLeft:
					case Network::MessageType::UserCommandOrient:
					case Network::MessageType::UserCommandSelectAbility:
					case Network::MessageType::UserCommandActivateAbility:
					case Network::MessageType::UserCommandDeactivateAbility:
					case Network::MessageType::UserCommandPickUpAbility:
					case Network::MessageType::UserCommandJump:
					case Network::MessageType::UserCommandStopJumping:
					case RootEngine::Network::InnerMessageID::CONNECT:
						if (m_serverMessageHandler != nullptr)
							m_serverMessageHandler->HandleServerMessage(message.get());
						else
							m_logger->LogText(LogTag::NETWORK, LogLevel::FATAL_ERROR, "Received server message without local or remote server existing");
						break;

					case RootEngine::Network::InnerMessageID::DISCONNECT:
						// This can either be a client or a server timing out or disconnecting
						if (m_serverMessageHandler != nullptr)
							m_serverMessageHandler->HandleServerMessage(message.get());
						if (m_clientMessageHandler != nullptr)
							m_clientMessageHandler->HandleClientMessage(message.get());
						break;

					case RootEngine::Network::InnerMessageID::NETWORK_DISCOVERY:
						// TODO: Read the server info and attach it to the list of servers available
						{
							RootSystems::ServerInfoInternal* info = (RootSystems::ServerInfoInternal*)message->Data;
							m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Server found through LAN-discovery: %s (%s:%u) %u/%u", info->Name, info->IP, info->Port, info->NumPlayers, info->MaxPlayers);
						}
						break;

					default:
						m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "Unrecognized message ID: %d", message->MessageID);
				}
			}
		}

		void MessageHandler::SetChatSystem( ChatSystemInterface* p_chatSystem )
		{
			if(m_clientMessageHandler != nullptr)
				m_clientMessageHandler->SetChatSystem(p_chatSystem);
		}

	}
}


/*
TEST(NetworkMessage, SerializeOverlay)
{
	RootForce::Network::MessageChat m;
	m.Type = RootForce::Network::MessageChat::TYPE_SERVER_MESSAGE;
	m.SenderID = 10;
	m.Message = "Hello world";

	unsigned char* serializedData = (unsigned char*) &m;
	RootForce::Network::MessageChat m2 = *((RootForce::Network::MessageChat*) serializedData);

	EXPECT_TRUE(m.Type == m2.Type);
	EXPECT_TRUE(m.SenderID == m2.SenderID);
	EXPECT_TRUE(strcmp(m.Message, m2.Message) == 0);
}


TEST(NetworkMessage, SerializeCopy)
{
	RootForce::Network::MessageChat m;
	m.Type = RootForce::Network::MessageChat::TYPE_SERVER_MESSAGE;
	m.SenderID = 10;
	m.Message = "Hello world";
	
	size_t size = 0;
	size += sizeof(m.Type);
	size += sizeof(m.SenderID);
	size += strlen(m.Message);

	unsigned char* serializedData = new unsigned char[size];
	memcpy(serializedData, &m, size);

	RootForce::Network::MessageChat m2;
	memcpy(&m2, serializedData, size);

	EXPECT_TRUE(m.Type == m2.Type);
	EXPECT_TRUE(m.SenderID == m2.SenderID);
	EXPECT_TRUE(strcmp(m.Message, m2.Message) == 0);
}
*/
