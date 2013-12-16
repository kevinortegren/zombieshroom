#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/Network/ClientMessageSystem.h>
#include <RootSystems/Include/Network/ServerMessageSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>

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

			MessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::NetworkInterface* p_networkInterface);

			void Host(int16_t p_port, ServerType p_type);
			void Connect( int16_t p_port, const char* p_address = "");

			void Update();
		private:
			ECS::World* m_world;
			Logging* m_logger;

			RootEngine::Network::Server* m_server;
			ClientMessageHandler* m_clientMessageHandler;
			ServerMessageHandler* m_serverMessageHandler;

			// Might change to all dynamic objects
			std::map<uint8_t, ECS::Entity*> m_playerEntities;
			RootEngine::Network::NetworkInterface* m_networkInterface;
		};
	}
}