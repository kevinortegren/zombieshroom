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
		class NetworkEntityMap
		{
		public:
			NetworkEntityMap(ECS::World* p_world);

			ECS::Entity* AddTemporaryEntity(int16_t& p_temporaryId);
			ECS::Entity* AddSynchronizedEntity(int16_t& p_synchronizedId);
			void SetSynchronizedId(int16_t p_temporaryId, int16_t p_synchronizedId);

			int16_t GetSynchronizedId(ECS::Entity* p_entity) const;
			int16_t GetTemporaryId(ECS::Entity* p_entity) const;
		private:
			ECS::World* m_world;

			std::map<int32_t, unsigned int> m_entityMap;
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
			void SetChatSystem(ChatSystemInterface* p_chatSystem);

		private:
			ECS::World* m_world;
			Logging* m_logger;

			RootEngine::Network::Server* m_server;
			ClientMessageHandler* m_clientMessageHandler;
			ServerMessageHandler* m_serverMessageHandler;
		};
	}
}