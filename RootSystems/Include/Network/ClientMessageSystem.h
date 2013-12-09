#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

namespace RootForce
{
	namespace Network
	{
		/** Handles messages to a client (on either a remote or a local system). */
		class ClientMessageHandler
		{
		public:
			ClientMessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::Server* p_server);

			void HandleClientMessage(RootEngine::Network::Message* p_message);
		private:
			ECS::World* m_world;
			Logging* m_logger;
			RootEngine::Network::Server* m_server;

			void HandleGameStateSnapshotMessage(RootEngine::Network::Message* p_message);
			void HandleChatToClientMessage(RootEngine::Network::Message* p_message);
			void HandleUserConnectedMessage(RootEngine::Network::Message* p_message);
			void HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message);
		};
	}
}