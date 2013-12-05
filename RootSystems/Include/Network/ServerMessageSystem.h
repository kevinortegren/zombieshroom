#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/PlayerControlSystem.h>

namespace RootForce
{
	namespace Network
	{
		class ServerMessageSystem : public ECS::EntitySystem
		{
		public:
			ServerMessageSystem(ECS::World* p_world, Logging* p_logger);

			void Init();
			void Begin();
			void ProcessEntity(ECS::Entity* p_entity);
			void End();

			void HandleServerMessage(RootEngine::Network::Message* p_message);
		private:
			Logging* m_logger;

			void HandleChatToServerMessage(RootEngine::Network::Message* p_message);
			void HandleUserInfoMessage(RootEngine::Network::Message* p_message);
			void HandleUserCommandMessage(RootEngine::Network::Message* p_message, RootForce::PlayerAction::PlayerAction p_action);
		};
	}
}