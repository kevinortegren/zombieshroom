#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <Utility/ECS/Include/World.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootForce/Include/ChatSystem.h>
#include <RootSystems/Include/PlayerSystem.h>

namespace RootForce
{
	namespace Network
	{
		/** Handles messages to a client (on either a remote or a local system). */
		class ClientMessageHandler
		{
		public:
			ClientMessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::Server* p_server);

			void Update();
			void HandleClientMessage(RootEngine::Network::Message* p_message);
			void SetChatSystem(ChatSystemInterface* p_chatSystem) { m_chatSystem = p_chatSystem; }
			void SetPlayerSystem(PlayerSystem* p_playerSystem) { m_playerSystem = p_playerSystem; }
		private:
			ECS::World* m_world;
			Logging* m_logger;

			NetworkEntityMap m_networkEntityMap;
			RootEngine::Network::Server* m_server;
			ChatSystemInterface* m_chatSystem;
			PlayerSystem* m_playerSystem;

			void HandleGameStateSnapshotMessage(RootEngine::Network::Message* p_message);
			void HandleChatToClientMessage(RootEngine::Network::Message* p_message);
			void HandleUserConnectedMessage(RootEngine::Network::Message* p_message);
			void HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message);
			void HandleConnectionAcceptedMessage(RootEngine::Network::Message* p_message);
			void HandleConnectionRefusedMessage(RootEngine::Network::Message* p_message);
			void HandleConnectionLostMessage(RootEngine::Network::Message* p_message);
		};
	}
}