#include <Network/ClientMessageSystem.h>
#include <Network/Messages.h>

namespace RootForce
{
	namespace Network
	{
		ClientMessageSystem::ClientMessageSystem(ECS::World* p_world, Logging* p_logger)
			: ECS::EntitySystem(p_world)
			, m_logger(p_logger)
		{

		}


		void ClientMessageSystem::Init()
		{

		}

		void ClientMessageSystem::Begin()
		{

		}

		void ClientMessageSystem::ProcessEntity(ECS::Entity* p_entity)
		{

		}

		void ClientMessageSystem::End()
		{

		}


		void ClientMessageSystem::HandleClientMessage(RootEngine::Network::Message* p_message)
		{
			switch (p_message->MessageID)
			{
				case MessageType::GameStateSnapshot:
					break;
				case MessageType::ChatToClient:
					break;
				case MessageType::UserConnected:
					break;
				case MessageType::UserDisconnected:
					break;
				case RootEngine::Network::InnerMessageID::CONNECTION_ACCEPTED:
					m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "Connection to server established");

					// TODO: Send a user info message and await a user connected message (add a connection refused message to the protocol?)
					break;
				case RootEngine::Network::InnerMessageID::CONNECTION_REFUSED:
					m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Connection to server refused");
					// TODO: Bail. Run for it. They're on to you.
					break;
				case RootEngine::Network::InnerMessageID::DISCONNECT:
					break;
			}
		}

		void ClientMessageSystem::HandleGameStateSnapshotMessage(RootEngine::Network::Message* p_message)
		{

		}

		void ClientMessageSystem::HandleChatToClientMessage(RootEngine::Network::Message* p_message)
		{
			MessageChat* header = (MessageChat*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Chat from client %d: %s", header->SenderID, header->Message);
		}

		void ClientMessageSystem::HandleUserConnectedMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserConnected* header = (MessageUserConnected*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "User %d (%s) connected", header->UserID, header->UserInfo.PlayerName);
		}

		void ClientMessageSystem::HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserDisconnected* header = (MessageUserDisconnected*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "User %d connected", header->UserID);
		}

	}
}