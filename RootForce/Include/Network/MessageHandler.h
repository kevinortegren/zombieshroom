#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <Network/ClientMessageSystem.h>
#include <Network/ServerMessageSystem.h>

namespace RootForce
{
	namespace Network
	{
		struct NetworkPlayerComponent : public ECS::Component<NetworkPlayerComponent>
		{
			int PlayerSlot;
		};

		

		class MessageHandler
		{
		public:
			enum ServerType
			{
				LOCAL,
				REMOTE,
				DEDICATED
			};

			MessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::NetworkInterface* p_networkInterface, ServerType p_type, int16_t port, const char* address = "");

			void Update();
		private:
			ECS::World* m_world;
			Logging* m_logger;

			RootEngine::Network::Server* m_server;
			ClientMessageSystem* m_clientMessageSystem;
			ServerMessageSystem* m_serverMessageSystem;

			// Might change to all dynamic objects
			std::map<uint8_t, ECS::Entity*> m_playerEntities;
		};
	}
}