#include <Network/ClientMessageSystem.h>
#include <Network/Messages.h>

namespace RootForce
{
	namespace Network
	{
		ClientMessageSystem::ClientMessageSystem(ECS::World* p_world, Logging* p_logger)
			: ECS::EntitySystem(p_world)
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