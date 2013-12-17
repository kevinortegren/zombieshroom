#ifndef COMPILE_LEVEL_EDITOR

#include <Network/ServerMessageSystem.h>
#include <Network/Messages.h>
#include <Network/ServerInfo.h>

namespace RootForce
{
	namespace Network
	{
		/*
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
							TemporaryId_t tId;
							m_networkEntityMap->AddEntity();
							m_networkEntityMap->SetSynchronizedId(tId, m_networkEntityMap->NextSynchronizedId());

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

		void ServerClientSystem::HandleUserConnectMessage(RootEngine::Network::Message* p_message)
		{
			// Create a synchronized entity for the connected player.
			TemporaryId_t tId;
			ECS::Entity* entity = m_networkEntityMap->AddEntity(tId);
			m_networkEntityMap->SetSynchronizedId(tId, m_networkEntityMap->NextSynchronizedId());

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
		*/




		ServerMessageHandler::ServerMessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::LocalServer* p_server)
			: m_world(p_world) 
			, m_logger(p_logger)
			, m_server(p_server)
		{
			//m_serverClientSystem = new ServerClientSystem(m_world, m_logger, m_networkEntityMap);
			//m_world->GetSystemManager()->AddSystem<ServerClientSystem>(m_serverClientSystem, "ServerClientSystem");
			
			UpdateServerInfo();
		}


		void ServerMessageHandler::HandleServerMessage(RootEngine::Network::Message* p_message)
		{
			switch (p_message->MessageID)
			{
				case MessageType::ChatToServer:
					HandleChatToServerMessage(p_message);
					break;
				case MessageType::UserInfo:
					HandleUserInfoMessage(p_message);
					//m_serverClientSystem->HandleUserInfo(p_message);
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
					HandleUserConnectedMessage(p_message);
					//m_serverClientSystem->HandleUserConnectMessage(p_message);
					break;
				case RootEngine::Network::InnerMessageID::DISCONNECT:
					HandleUserDisconnectedMessage(p_message);
					//m_serverClientSystem->HandleUserDisconnectMessage(p_message);
					break;
			}
		}

		void ServerMessageHandler::HandleUserConnectedMessage(RootEngine::Network::Message* p_message)
		{
			// Create a player entity for the newly connected player. Await user info before confirming and sending the synchronized entity to all clients.
			ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
			TemporaryId_t tId = m_networkEntityMap.AddEntity(entity);
			SynchronizedId_t sId = m_networkEntityMap.NextSynchronizedId();
			m_networkEntityMap.SetSynchronizedId(tId, sId);

			m_playerEntities[(uint8_t)p_message->Data[0]] = sId;
			UpdateServerInfo();
		}

		void ServerMessageHandler::HandleUserInfoMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserInfo userInfoMessage;
			userInfoMessage.Deserialize(p_message->Data);

			// Create a new entity for the new player
			SynchronizedId_t sId = m_playerEntities[p_message->SenderID];
			ECS::Entity* entity = m_networkEntityMap.GetSynchronizedEntity(sId);

			NetworkClientComponent* clientComponent = m_world->GetEntityManager()->CreateComponent<NetworkClientComponent>(entity);
			clientComponent->UserID = p_message->SenderID;
			clientComponent->Name = userInfoMessage.PlayerName;
			clientComponent->State = NetworkClientComponent::CONNECTED;

			{
				// Notify all other clients of the connected player.
				MessageUserConnected userConnectedMessage;
				userConnectedMessage.UserID = p_message->SenderID;
				userConnectedMessage.UserInfo.PlayerName = userInfoMessage.PlayerName;
				userConnectedMessage.UserInfo.PlayerEntity.SynchronizedID = sId;
				userConnectedMessage.UserInfo.PlayerEntity.TemporaryID = TEMPORARY_ID_NONE;

				RootEngine::Network::Message m;
				m.SenderID = 0;
				m.Reliability = RELIABLE_ORDERED;
				m.MessageID = MessageType::UserConnected;
				m.DataSize = userConnectedMessage.GetSerializedSize();
				m.Data = new uint8_t[m.DataSize];
				userConnectedMessage.Serialize(m.Data);

				std::vector<uint8_t> connectedClients = m_server->GetConnectedClients();
				for (size_t i = 0; i < connectedClients.size(); ++i)
				{
					if (connectedClients[i] != p_message->SenderID)
					{
						m.RecipientID = connectedClients[i];
						m_server->Send(m);
					}
				}

				// Notify the connected player of its synchronized entity ID and user ID.
				userConnectedMessage.UserInfo.PlayerEntity.TemporaryID = userInfoMessage.PlayerEntity.TemporaryID;
				m.RecipientID = p_message->SenderID;
				m_server->Send(m);

				// Notify the connected player of all other players.
				for (size_t i = 0; i < connectedClients.size(); ++i)
				{
					if (connectedClients[i] != p_message->SenderID)
					{
						SynchronizedId_t otherSynchronizedId = m_playerEntities[connectedClients[i]];
						NetworkClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<NetworkClientComponent>(m_networkEntityMap.GetSynchronizedEntity(otherSynchronizedId));

						userConnectedMessage.UserID = connectedClients[i];
						userConnectedMessage.UserInfo.PlayerName = otherClientComponent->Name.c_str();
						userConnectedMessage.UserInfo.PlayerEntity.EntityType = EntityCreated::TYPE_PLAYER;
						userConnectedMessage.UserInfo.PlayerEntity.SynchronizedID = otherSynchronizedId;
						userConnectedMessage.UserInfo.PlayerEntity.TemporaryID = TEMPORARY_ID_NONE;
						
						userConnectedMessage.Serialize(m.Data);
						m_server->Send(m);
					}
				}
			}

			m_playerEntities[p_message->SenderID] = sId;

			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: User %d (%s) connected", p_message->SenderID, userInfoMessage.PlayerName);
		}

		void ServerMessageHandler::HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message)
		{
			// Notify clients of the disconnected player
			MessageUserDisconnected userDisconnected;
			userDisconnected.UserID = (int8_t) p_message->Data[0];

			RootEngine::Network::Message m;
			m.SenderID = 0;
			m.Reliability = RELIABLE_ORDERED;
			m.RecipientID = RootEngine::Network::RECIPIENT_BROADCAST;
			m.MessageID = MessageType::UserDisconnected;
			m.DataSize = userDisconnected.GetSerializedSize();
			m.Data = new uint8_t[m.DataSize];
			userDisconnected.Serialize(m.Data);

			m_server->Send(m);

			// Remove the disconnected player's entity
			ECS::Entity* entity = m_networkEntityMap.GetSynchronizedEntity(m_playerEntities[(int8_t)p_message->SenderID]);
			NetworkClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<NetworkClientComponent>(entity);
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Client %d (%s) disconnected", (int8_t)p_message->Data[0], clientComponent->Name);

			m_networkEntityMap.RemoveSynchronizedEntity(m_playerEntities[(int8_t)p_message->Data[0]]);
			m_playerEntities.erase(m_playerEntities.find((int8_t)p_message->Data[0]));

			m_world->GetEntityManager()->RemoveAllComponents(entity);
			m_world->GetEntityManager()->RemoveEntity(entity);
			UpdateServerInfo();
		}


		void ServerMessageHandler::HandleChatToServerMessage(RootEngine::Network::Message* p_message)
		{
			MessageChat header;
			header.Deserialize(p_message->Data);

			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Chat message sent to server from %d to %d: %s", header.SenderID, p_message->RecipientID, header.Message);

			RootEngine::Network::Message msg = *p_message;
			msg.MessageID = MessageType::ChatToClient; //Set the new message type so that they are forwarded to the clients
			msg.Reliability = PacketReliability::RELIABLE_ORDERED; //Set a new reliability since this is not sent over the network
			
			if (p_message->RecipientID == RootEngine::Network::RECIPIENT_BROADCAST)
			{
				m_server->Send(msg);
			}
			else
			{
				m_server->Send(msg);
			}
			
		}

		void ServerMessageHandler::HandleUserCommandMessage(RootEngine::Network::Message* p_message, RootForce::PlayerAction::PlayerAction p_action)
		{
			
		}
		void ServerMessageHandler::UpdateServerInfo()
		{
			RootSystems::ServerInfo info;
			strcpy(info.Name, "Local server");
			info.MaxPlayers = 12;
			info.NumPlayers = m_server->GetConnectedClients().size();
			info.PasswordProtected = false;
			m_server->SetNetworkDiscoveryResponse((uint8_t*)&info, sizeof(info));
		}

	}
}

#endif
