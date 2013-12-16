#pragma once

#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/Network/ClientMessageSystem.h>
#include <RootSystems/Include/Network/ServerMessageSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>

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
			void PingNetwork(USHORT p_port);

			void Host(int16_t p_port, ServerType p_type);
			void Connect( int16_t p_port, const char* p_address = "");

			void Update();
			void SetChatSystem(ChatSystemInterface* p_chatSystem);

			ClientMessageHandler* GetClientMessageHandler() { return m_clientMessageHandler; }
			ServerMessageHandler* GetServerMessageHandler() { return m_serverMessageHandler; }
		private:
			ECS::World* m_world;
			Logging* m_logger;

			RootEngine::Network::Server* m_server;
			ClientMessageHandler* m_clientMessageHandler;
			ServerMessageHandler* m_serverMessageHandler;
			RootEngine::Network::NetworkInterface* m_networkInterface;
		};
	}
}