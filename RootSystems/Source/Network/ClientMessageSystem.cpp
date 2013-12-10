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
			//MessageChat* header = (MessageChat*) p_message->Data;
			MessageChat header;
			header.Deserialize(p_message->Data);
			m_chatSystem->JSAddMessage( "#" + std::to_string(header.SenderID) + ": " + header.Message );
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Chat from client %d: %s", header.SenderID, header.Message);
		}

		void ClientMessageHandler::HandleUserConnectedMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserConnected* header = (MessageUserConnected*) p_message->Data;
			m_chatSystem->JSAddMessage(std::string("User Connected: ") + header->UserInfo.PlayerName + " #" + std::to_string(header->UserID));
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "User %d (%s) connected", header->UserID, header->UserInfo.PlayerName);
		}

		void ClientMessageHandler::HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserDisconnected* header = (MessageUserDisconnected*) p_message->Data;
			m_chatSystem->JSAddMessage(std::string("User disconnected: #") + std::to_string(header->UserID) );
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "User %d disconnected", header->UserID);
		}

		void ClientMessageHandler::Update()
		{
			if(m_chatSystem == nullptr)
				return;

			std::string chatmsg;
			while((chatmsg = m_chatSystem->PollMessage()).compare("") != 0)
			{
				RootEngine::Network::Message netmsg;
				RootForce::Network::MessageChat netmsgchat;
				netmsgchat.SenderID = -1; // TODO: Fix own sender ID to first player connected ID received
				netmsgchat.Type = RootForce::Network::MessageChat::TYPE_DEBUG;
				netmsgchat.Message = new char[chatmsg.size()+1];
				std::strcpy(netmsgchat.Message, chatmsg.c_str());

				netmsg.MessageID = RootForce::Network::MessageType::ChatToServer;
				netmsg.RecipientID = RootEngine::Network::RECIPIENT_BROADCAST;
				netmsg.Reliability = PacketReliability::RELIABLE_ORDERED;
				netmsg.SenderID = -1; // TODO: Fix own sender ID to first player connected ID received
				netmsg.DataSize = netmsgchat.GetSerializedSize();
				netmsg.Data = new uint8_t[netmsg.DataSize];
				netmsgchat.Serialize(netmsg.Data);
				m_server->Send(netmsg);
			}
		}

	}
}