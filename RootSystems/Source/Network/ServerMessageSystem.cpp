#include <Network/ServerMessageSystem.h>
#include <Network/Messages.h>

namespace RootForce
{
	namespace Network
	{
		ServerMessageSystem::ServerMessageSystem(ECS::World* p_world, Logging* p_logger)
			: ECS::EntitySystem(p_world)
			, m_logger(p_logger)
		{

		}


		void ServerMessageSystem::Init()
		{

		}

		void ServerMessageSystem::Begin()
		{

		}

		void ServerMessageSystem::ProcessEntity(ECS::Entity* p_entity)
		{

		}

		void ServerMessageSystem::End()
		{

		}


		void ServerMessageSystem::HandleServerMessage(RootEngine::Network::Message* p_message)
		{
			switch (p_message->MessageID)
			{
				case MessageType::ChatToServer:
					HandleChatToServerMessage(p_message);
					break;
				case MessageType::UserInfo:
					HandleUserInfoMessage(p_message);
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
					// Create an entity for the remote player
					// Notify all other connected clients through a UserConnected message
					break;
				case RootEngine::Network::InnerMessageID::DISCONNECT:
					// Remove the entity for the remote player
					// Notify all other connected clients through a UserDisconnected message
					break;
			}
		}

		void ServerMessageSystem::HandleChatToServerMessage(RootEngine::Network::Message* p_message)
		{
			MessageChat* header = (MessageChat*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Chat message sent to server from %d to %d: %s", header->SenderID, p_message->RecipientID, header->Message); 
		}

		void ServerMessageSystem::HandleUserInfoMessage(RootEngine::Network::Message* p_message)
		{
			MessageUserInfo* header = (MessageUserInfo*) p_message->Data;
			m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: User info sent to server. Name: %s.", header->PlayerName);
		}

		void ServerMessageSystem::HandleUserCommandMessage(RootEngine::Network::Message* p_message, RootForce::PlayerAction::PlayerAction p_action)
		{
			
		}


	}
}