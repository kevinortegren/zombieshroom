#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

namespace RootForce
{
	namespace Network
	{
		// TODO: Change name to ServerMessageHandler
		class ClientMessageSystem : public ECS::EntitySystem
		{
		public:
			ClientMessageSystem(ECS::World* p_world, Logging* p_logger);

			void Init();
			void Begin();
			void ProcessEntity(ECS::Entity* p_entity);
			void End();

			void HandleClientMessage(RootEngine::Network::Message* p_message);
		private:
			Logging* m_logger;

			void HandleGameStateSnapshotMessage(RootEngine::Network::Message* p_message);
			void HandleChatToClientMessage(RootEngine::Network::Message* p_message);
			void HandleUserConnectedMessage(RootEngine::Network::Message* p_message);
			void HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message);
		};
	}
}