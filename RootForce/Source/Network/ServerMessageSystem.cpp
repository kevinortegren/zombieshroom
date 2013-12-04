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
					HandleUserCommandMessage(p_message, PlayerAction::MOVE_FORWARDS);
					break;
				case MessageType::UserCommandMoveBackward:
					HandleUserCommandMessage(p_message, PlayerAction::MOVE_BACKWARDS);
					break;
				case MessageType::UserCommandStrafeRight:
					HandleUserCommandMessage(p_message, PlayerAction::STRAFE_RIGHT);
					break;
				case MessageType::UserCommandStrafeLeft:
					HandleUserCommandMessage(p_message, PlayerAction::STRAFE_LEFT);
					break;
				case MessageType::UserCommandOrient:
					HandleUserCommandMessage(p_message, PlayerAction::ORIENTATE);
					break;
				case MessageType::UserCommandSelectAbility:
					HandleUserCommandMessage(p_message, PlayerAction::SELECT_ABILITY);
					break;
				case MessageType::UserCommandActivateAbility:
					HandleUserCommandMessage(p_message, PlayerAction::ACTIVATE_ABILITY);
					break;
				case MessageType::UserCommandDeactivateAbility:
					break;
				case MessageType::UserCommandPickUpAbility:
					break;
				case MessageType::UserCommandJump:
					break;
				case MessageType::UserCommandStopJumping:
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