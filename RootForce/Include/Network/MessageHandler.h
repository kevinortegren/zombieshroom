#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <Network/ClientMessageSystem.h>
#include <Network/ServerMessageSystem.h>

namespace RootForce
{
	namespace Network
	{
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
			Logging* m_logger;
			RootEngine::Network::Server* m_server;
			std::shared_ptr<ClientMessageSystem> m_clientMessageSystem;
			std::shared_ptr<ServerMessageSystem> m_serverMessageSystem;
		};
	}
}