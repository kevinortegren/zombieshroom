#include <Network/ClientMessageSystem.h>
#include <Network/Messages.h>

namespace RootForce
{
	namespace Network
	{
		ClientMessageHandler::ClientMessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::Server* p_server)
			: m_world(p_world)
			, m_logger(p_logger)
			, m_server(p_server)
		{

		}


		void ClientMessageHandler::HandleClientMessage(RootEngine::Network::Message* p_message)
		{
			switch (p_message->MessageID)
			{
				case MessageType::GameStateSnapshot:
					break;
				case MessageType::ChatToClient:
					HandleChatToClientMessage(p_message);
					break;
				case MessageType::UserConnected:
					HandleUserConnectedMessage(p_message);
					break;
				case MessageType::UserDisconnected:
					HandleUserDisconnectedMessage(p_message);
					break;
				case RootEngine::Network::InnerMessageID::CONNECTION_ACCEPTED:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "CLIENT: Connection to server established");

					// TODO: Create a user info message and send it
					RootForce::Network::MessageUserInfo userInfoContents;
					userInfoContents.PlayerName = "john doe";

					RootEngine::Network::Message userInfoMessage;
					userInfoMessage.MessageID = MessageType::UserInfo;
					userInfoMessage.RecipientID = 0;
					userInfoMessage.Reliability = RELIABLE;
					userInfoMessage.DataSize = userInfoContents.GetSerializedSize();
					userInfoMessage.Data = new uint8_t[userInfoMessage.DataSize];
					userInfoContents.Serialize(userInfoMessage.Data);

					m_server->Send(userInfoMessage);
				} break;
				case RootEngine::Network::InnerMessageID::CONNECTION_REFUSED:
					m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Connection to server refused");
					// TODO: Bail. Run for it. They're on to you.
					break;
				case RootEngine::Network::InnerMessageID::DISCONNECT:
					// TODO: Remove entity associated with the given player
					break;
			}
		}

		void ClientMessageHandler::HandleGameStateSnapshotMessage(RootEngine::Network::Message* p_message)
		{

		}

		void ClientMessageHandler::HandleChatToClientMessage(RootEngine::Network::Message* p_message)
		{
			MessageChat* header = (MessageChat*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Chat from client %d: %s", header->SenderID, header->Message);
		}

		void ClientMessageHandler::HandleUserConnectedMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserConnected* header = (MessageUserConnected*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "User %d (%s) connected", header->UserID, header->UserInfo.PlayerName);
		}

		void ClientMessageHandler::HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserDisconnected* header = (MessageUserDisconnected*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "User %d connected", header->UserID);
		}

	}
}