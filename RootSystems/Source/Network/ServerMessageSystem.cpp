#include <Network/ServerMessageSystem.h>
#include <Network/Messages.h>

namespace RootForce
{
	namespace Network
	{
		void ServerClientSystem::Init()
		{

		}

		void ServerClientSystem::Begin()
		{

		}

		void ServerClientSystem::ProcessEntity(ECS::Entity* p_entity)
		{
			NetworkClientComponent* networkClientComponent = m_networkClientComponentMapper.Get(p_entity);

			for (RootEngine::Network::Message& message : m_messages)
			{
				switch (message.MessageID)
				{
					case RootEngine::Network::InnerMessageID::DISCONNECT:
					{
						uint8_t userID = static_cast<uint8_t>(message.Data[0]);
						if (networkClientComponent->UserID == userID)
						{
							m_world->GetEntityManager()->RemoveEntity(p_entity);
						}
					} break;
					
					case MessageType::UserInfo:
					{
						if (message.SenderID == networkClientComponent->UserID)
						{
							networkClientComponent->Name = std::string(reinterpret_cast<char*>(message.Data));
							networkClientComponent->State = NetworkClientComponent::CONNECTED;

							m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: User info for slot %u. Name: %s.", networkClientComponent->UserID, networkClientComponent->Name);
						}
					} break;
					// TODO: Add a default
				}
			}
		}

		void ServerClientSystem::End()
		{
			m_messages.clear();
		}

		void ServerClientSystem::HandleUserConnectMessage(RootEngine::Network::Message* p_message, bool p_local)
		{
			ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
			NetworkClientComponent* newPlayerNetworkClientComponent = m_world->GetEntityManager()->CreateComponent<NetworkClientComponent>(entity);

			// TODO: Add physics component
			//m_world->GetEntityManager()->CreateEntity(

			newPlayerNetworkClientComponent->UserID = p_message->Data[0];
			newPlayerNetworkClientComponent->Name = "Unknown Player";
			newPlayerNetworkClientComponent->State = NetworkClientComponent::EXPECTING_USER_INFO;

			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: User with ID %u connected", newPlayerNetworkClientComponent->UserID);
		}

		void ServerClientSystem::HandleUserDisconnectMessage(RootEngine::Network::Message* p_message)
		{
			m_messages.push_back(*p_message);
		}

		void ServerClientSystem::HandleUserInfo(RootEngine::Network::Message* p_message)
		{
			m_messages.push_back(*p_message);
		}




		ServerMessageHandler::ServerMessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::LocalServer* p_server)
			: m_world(p_world) 
			, m_logger(p_logger)
			, m_server(p_server)
		{
			m_serverClientSystem = new ServerClientSystem(m_world, m_logger);
			m_world->GetSystemManager()->AddSystem<ServerClientSystem>(m_serverClientSystem, "ServerClientSystem");
		}


		void ServerMessageHandler::HandleServerMessage(RootEngine::Network::Message* p_message)
		{
			switch (p_message->MessageID)
			{
				case MessageType::ChatToServer:
					HandleChatToServerMessage(p_message);
					break;
				case MessageType::UserInfo:
					m_serverClientSystem->HandleUserInfo(p_message);
					break;
				case MessageType::UserCommandMoveForward:
					// Validate if the player can move in the current state
					// Use physics to cause player entity to move
					HandleUserCommandMessage(p_message, PlayerAction::MOVE_FORWARDS);
					break;
				case MessageType::UserCommandMoveBackward:
					// Validate if the player can move in the current state
					// Use physics to cause player entity to move
					HandleUserCommandMessage(p_message, PlayerAction::MOVE_BACKWARDS);
					break;
				case MessageType::UserCommandStrafeRight:
					// Validate if the player can move in the current state
					// Use physics to cause player entity to move
					HandleUserCommandMessage(p_message, PlayerAction::STRAFE_RIGHT);
					break;
				case MessageType::UserCommandStrafeLeft:
					// Validate if the player can move in the current state
					// Use physics to cause player entity to move
					HandleUserCommandMessage(p_message, PlayerAction::STRAFE_LEFT);
					break;
				case MessageType::UserCommandOrient:
					HandleUserCommandMessage(p_message, PlayerAction::ORIENTATE);
					break;
				case MessageType::UserCommandSelectAbility:
					HandleUserCommandMessage(p_message, PlayerAction::SELECT_ABILITY);
					break;
				case MessageType::UserCommandActivateAbility:
					// Validate if the player can use an ability in the current state
					// Validate if the player has an ability selected
					// Update the game logic
					HandleUserCommandMessage(p_message, PlayerAction::ACTIVATE_ABILITY);
					break;
				case MessageType::UserCommandDeactivateAbility:
					// Validate if the player has an ability active
					// Update the game logic
					break;
				case MessageType::UserCommandPickUpAbility:
					// Validate if the player can pick up an ability in the current state
					// Validate if the player is near an ability
					// Update the player abilities
					break;
				case MessageType::UserCommandJump:
					// Validate if the player can jump in the current state
					// Use physics to cause player entity to jump
					break;
				case MessageType::UserCommandStopJumping:
					// TODO: Decide if we will use this
					break;
				case RootEngine::Network::InnerMessageID::CONNECT:
					m_serverClientSystem->HandleUserConnectMessage(p_message, m_server->IsClientLocal(p_message->SenderID));
					break;
				case RootEngine::Network::InnerMessageID::DISCONNECT:
					m_serverClientSystem->HandleUserDisconnectMessage(p_message);
					break;
			}
		}

		void ServerMessageHandler::HandleChatToServerMessage(RootEngine::Network::Message* p_message)
		{
			MessageChat* header = (MessageChat*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Chat message sent to server from %d to %d: %s", header->SenderID, p_message->RecipientID, header->Message);

			// TODO: Might add team chat
			if (p_message->RecipientID == RootEngine::Network::RECIPIENT_BROADCAST)
			{
				m_server->Send(*p_message);
			}
			else
			{
				m_server->Send(*p_message);
			}
		}

		void ServerMessageHandler::HandleUserCommandMessage(RootEngine::Network::Message* p_message, RootForce::PlayerAction::PlayerAction p_action)
		{
			
		}


	}
}