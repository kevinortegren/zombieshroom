#ifndef COMPILE_LEVEL_EDITOR

#include <Network/MessageHandler.h>
#include <Network/Messages.h>
#include <RootEngine/Network/Include/LocalServer.h>
#include <RootEngine/Network/Include/RemoteServer.h>
#include <algorithm>

namespace RootForce
{
	namespace Network
	{
		MessageHandler::MessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::NetworkInterface* p_networkInterface)
			: m_world(p_world)
			, m_logger(p_logger)
		{
			m_clientMessageHandler = nullptr;
			m_serverMessageHandler = nullptr;
			m_networkInterface = p_networkInterface;

			p_networkInterface->Initialize(RootEngine::Network::PeerType::REMOTESERVER);
			m_server = p_networkInterface->GetNetworkSystem();
			dynamic_cast<RootEngine::Network::RemoteServer*>(m_server)->Initialize();
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Started remote server");
			//if (m_clientMessageSystem != nullptr) p_world->GetSystemManager()->AddSystem<ClientMessageSystem>(m_clientMessageSystem, "ClientMessageSystem");
			//if (m_serverMessageSystem != nullptr) p_world->GetSystemManager()->AddSystem<ServerMessageSystem>(m_serverMessageSystem, "ServerMessageSystem");
		}

		void MessageHandler::PingNetwork(USHORT p_port)
		{
			m_server->SendNetworkDiscoveryMessage(p_port);
			m_lanList.Start();
		}

		void MessageHandler::Host(int16_t p_port, ServerType p_type)
		{
			m_networkInterface->Initialize(RootEngine::Network::PeerType::LOCALSERVER);
			m_server = m_networkInterface->GetNetworkSystem();
			switch (p_type)
			{
			case RootForce::Network::MessageHandler::LOCAL:
				dynamic_cast<RootEngine::Network::LocalServer*>(m_server)->Host(p_port, false);
				m_clientMessageHandler = new ClientMessageHandler(m_world, m_logger, m_server);
				m_serverMessageHandler = new ServerMessageHandler(m_world, m_logger, dynamic_cast<RootEngine::Network::LocalServer*>(m_server));
				break;
			case RootForce::Network::MessageHandler::DEDICATED:
				dynamic_cast<RootEngine::Network::LocalServer*>(m_server)->Host(p_port);
				m_serverMessageHandler = new ServerMessageHandler(m_world, m_logger, dynamic_cast<RootEngine::Network::LocalServer*>(m_server));
				break;
			default:
				break;
			}			
		}

		void MessageHandler::Connect( int16_t p_port, const char* p_address /*= ""*/ )
		{
			m_networkInterface->Initialize(RootEngine::Network::PeerType::REMOTESERVER);
			m_server = m_networkInterface->GetNetworkSystem();
			dynamic_cast<RootEngine::Network::RemoteServer*>(m_server)->Initialize();
			dynamic_cast<RootEngine::Network::RemoteServer*>(m_server)->ConnectTo(p_address, p_port);

			m_clientMessageHandler = new ClientMessageHandler(m_world, m_logger, m_server);

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
							m_lanList.AddServer(info);
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

#endif
