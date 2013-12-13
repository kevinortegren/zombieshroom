#include <RootSystems/Include/Network/ClientMessageSystem.h>
#include <RootSystems/Include/Network/Messages.h>

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
					HandleGameStateSnapshotMessage(p_message);
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
					HandleConnectionAcceptedMessage(p_message);
					break;
				case RootEngine::Network::InnerMessageID::CONNECTION_REFUSED:
					HandleConnectionRefusedMessage(p_message);
					break;
				case RootEngine::Network::InnerMessageID::DISCONNECT:
					HandleConnectionLostMessage(p_message);
					break;
			}
		}

		void ClientMessageHandler::HandleGameStateSnapshotMessage(RootEngine::Network::Message* p_message)
		{

		}

		void ClientMessageHandler::HandleChatToClientMessage(RootEngine::Network::Message* p_message)
		{
			MessageChat header;
			header.Deserialize(p_message->Data);

			m_chatSystem->JSAddMessage( "#" + std::to_string(header.SenderID) + ": " + header.Message );
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "CLIENT: Chat from client %d: %s", header.SenderID, header.Message);
		}

		void ClientMessageHandler::HandleUserConnectedMessage(RootEngine::Network::Message* p_message)
		{
			//MessageUserConnected* header = (MessageUserConnected*) p_message->Data;
			MessageUserConnected contents;
			contents.Deserialize(p_message->Data);

			m_chatSystem->JSAddMessage(std::string("User Connected: ") + contents.UserInfo.PlayerName + " #" + std::to_string(contents.UserID));
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "CLIENT: User %d (%s) connected", contents.UserID, contents.UserInfo.PlayerName);
		}

		void ClientMessageHandler::HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserDisconnected contents;
			contents.Deserialize(p_message->Data);

			m_chatSystem->JSAddMessage(std::string("User disconnected: #") + std::to_string(contents.UserID) );
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "CLIENT: User %d disconnected", contents.UserID);
		}

		void ClientMessageHandler::HandleConnectionAcceptedMessage(RootEngine::Network::Message* p_message)
		{
			m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "CLIENT: Connection to server established.");

			// Create a player entity with a temporary ID until the server responds.
			ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
			TemporaryId_t tId = m_networkEntityMap.AddEntity(entity);


			// Notify the server of our info
			RootForce::Network::MessageUserInfo userInfoContents;
			userInfoContents.PlayerName = "John Doe";
			userInfoContents.PlayerEntity.EntityType = EntityCreated::TYPE_PLAYER;
			userInfoContents.PlayerEntity.SynchronizedID = SYNCHRONIZED_ID_NONE;
			userInfoContents.PlayerEntity.TemporaryID = tId;

			RootEngine::Network::Message m;
			m.MessageID = MessageType::UserInfo;
			m.RecipientID = 0;
			m.Reliability = RELIABLE_ORDERED;
			m.DataSize = userInfoContents.GetSerializedSize();
			m.Data = new uint8_t[m.DataSize];
			userInfoContents.Serialize(m.Data);

			m_server->Send(m);
		}

		void ClientMessageHandler::HandleConnectionRefusedMessage(RootEngine::Network::Message* p_message)
		{
			m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "CLIENT: Connection to server refused.");
		}

		void ClientMessageHandler::HandleConnectionLostMessage(RootEngine::Network::Message* p_message)
		{
			m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "CLIENT: Connection to server lost.");
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
				netmsgchat.Message = chatmsg.c_str();

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