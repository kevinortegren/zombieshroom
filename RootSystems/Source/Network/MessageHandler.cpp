#include <Network/MessageHandler.h>
#include <Network/Messages.h>
#include <RootEngine/Network/Include/LocalServer.h>
#include <RootEngine/Network/Include/RemoteServer.h>
#include <gtest/gtest.h>

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
					dynamic_cast<RootEngine::Network::LocalServer*>(m_server)->Host(port, false);
					
					m_clientMessageSystem = new ClientMessageSystem(p_world, m_logger);
					m_serverMessageSystem = new ServerMessageSystem(p_world, m_logger);
					break;
				case MessageHandler::REMOTE:
					p_networkInterface->Initialize(RootEngine::Network::PeerType::REMOTESERVER);
					m_server = p_networkInterface->GetNetworkSystem();
					dynamic_cast<RootEngine::Network::RemoteServer*>(m_server)->ConnectTo(address, port);

					m_clientMessageSystem = new ClientMessageSystem(p_world, m_logger);
					break;
				case MessageHandler::DEDICATED:
					p_networkInterface->Initialize(RootEngine::Network::PeerType::LOCALSERVER);
					m_server = p_networkInterface->GetNetworkSystem();
					dynamic_cast<RootEngine::Network::LocalServer*>(m_server)->Host(port, true);

					m_serverMessageSystem = new ServerMessageSystem(p_world, m_logger);
					break;
				default:
					m_logger->LogText(LogTag::NETWORK, LogLevel::FATAL_ERROR, "Invalid server type");
			}

			if (m_clientMessageSystem != nullptr) p_world->GetSystemManager()->AddSystem<ClientMessageSystem>(m_clientMessageSystem, "ClientMessageSystem");
			if (m_serverMessageSystem != nullptr) p_world->GetSystemManager()->AddSystem<ServerMessageSystem>(m_serverMessageSystem, "ServerMessageSystem");
		}

		void MessageHandler::Update()
		{
			m_server->Update();

			RootEngine::Network::Message* message = nullptr;
			while ((message = m_server->PollMessage()) != nullptr)
			{
				switch (message->MessageID)
				{
					case Network::MessageType::GameStateSnapshot: 
					case Network::MessageType::ChatToClient:
					case Network::MessageType::UserConnected:
					case Network::MessageType::UserDisconnected:
						if (m_clientMessageSystem != nullptr)
							m_clientMessageSystem->HandleClientMessage(message);
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
						if (m_serverMessageSystem != nullptr)
							m_serverMessageSystem->HandleServerMessage(message);
						else
							m_logger->LogText(LogTag::NETWORK, LogLevel::FATAL_ERROR, "Received server message without local or remote server existing");
						break;

					case RootEngine::Network::InnerMessageID::CONNECT:
					{
						/*
						uint8_t slot = *(uint8_t*)message->Data;

						ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
						NetworkPlayerComponent* comp = m_world->GetEntityManager()->CreateComponent<NetworkPlayerComponent>(entity);
						comp->PlayerSlot = slot;

						m_playerEntities[slot] = entity;

						m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Player at slot %u connected", slot);
						*/
					} break;

					case RootEngine::Network::InnerMessageID::DISCONNECT:
					{
						/*
						uint8_t slot = *(uint8_t*)message->Data;
						
						m_world->GetEntityManager()->RemoveEntity(m_playerEntities[slot]);

						m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Player at slot %u disconnected", slot);
						*/
					} break;

					default:
						m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "Unrecognized message ID: %d", message->MessageID);
				}
			}
		}
	}
}



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