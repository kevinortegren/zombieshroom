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
		typedef int16_t TemporaryId_t;
		typedef int16_t SynchronizedId_t;

		const TemporaryId_t TEMPORARY_ID_NONE = -1;
		const SynchronizedId_t SYNCHRONIZED_ID_NONE = -1;

		class NetworkEntityMap
		{
		public:
			NetworkEntityMap(ECS::World* p_world);

			ECS::Entity* AddEntity(TemporaryId_t& p_temporaryId);
			void SetSynchronizedId(TemporaryId_t p_temporaryId, SynchronizedId_t p_synchronizedId);

			SynchronizedId_t GetSynchronizedId(ECS::Entity* p_entity) const;
			TemporaryId_t GetTemporaryId(ECS::Entity* p_entity) const;
			ECS::Entity* GetSynchronizedEntity(SynchronizedId_t p_synchronizedId) const;

			/** Only used by server */
			SynchronizedId_t NextSynchronizedId();
		private:
			ECS::World* m_world;

			std::map<TemporaryId_t, ECS::Entity*> m_temporaryEntityMap;
			std::map<SynchronizedId_t, ECS::Entity*> m_synchronizedEntityMap;

			TemporaryId_t m_nextTemporaryId;
			SynchronizedId_t m_nextSynchronizedId;
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