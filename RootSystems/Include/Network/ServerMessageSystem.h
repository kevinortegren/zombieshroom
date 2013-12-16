#pragma once

#include <vector>
#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>
#include <RootSystems/Include/Physics.h>


namespace RootForce
{
	namespace Network
	{
		/**
			Will update the client components (which are attached to player entities). Handles the following messages:

			- Inner message: user connect
			- Inner message: user disconnect
			- User info
		*/
		class ServerClientSystem : public ECS::EntitySystem
		{
		public:
			ServerClientSystem(ECS::World* p_world, Logging* p_logger, NetworkEntityMap* p_networkEntityMap)
				: ECS::EntitySystem(p_world)
				, m_logger(p_logger)
				, m_networkEntityMap(p_networkEntityMap)
			{
				SetUsage<NetworkClientComponent>();
			}

			void Init();
			void Begin();
			void ProcessEntity(ECS::Entity* p_entity);
			void End();

			void HandleUserConnectMessage(RootEngine::Network::Message* p_message);
			void HandleUserDisconnectMessage(RootEngine::Network::Message* p_message);
			void HandleUserInfo(RootEngine::Network::Message* p_message);
		private:
			Logging* m_logger;
			NetworkEntityMap* m_networkEntityMap;
			std::vector<RootEngine::Network::Message> m_messages;

			ECS::ComponentMapper<NetworkClientComponent> m_networkClientComponentMapper;
		};


		/** 
			Will update the physics objects that can be modified across the network.

			- User command: move forward
			- User command: move backward
			- User command: strafe right
			- User command: strafe left
			- User command: orient
			- User command: jump
			- User command: stop jumping
		*/
		/*
		class ServerPhysicsSystem : public ECS::EntitySystem
		{
		public:
			ServerPhysicsSystem(ECS::World* p_world)
				: ECS::EntitySystem(p_world)
			{
				SetUsage<PhysicsAccessor>();
			}

			void Init();
			void Begin();
			void ProcessEntity(ECS::Entity* p_entity);
			void End();
		};
		*/



		/** Handles messages to a local/dedicated server. */
		class ServerMessageHandler
		{
		public:
			ServerMessageHandler(ECS::World* p_world, Logging* p_logger, RootEngine::Network::LocalServer* p_server);

			void HandleServerMessage(RootEngine::Network::Message* p_message);
		private:
			ECS::World* m_world;
			Logging* m_logger;
			
			NetworkEntityMap m_networkEntityMap;
			RootEngine::Network::LocalServer* m_server;
			//ServerClientSystem* m_serverClientSystem;

			// Map the player slot to a player entity.
			std::map<uint8_t, SynchronizedId_t> m_playerEntities;


			void HandleUserConnectedMessage(RootEngine::Network::Message* p_message);
			void HandleUserInfoMessage(RootEngine::Network::Message* p_message);
			void HandleUserDisconnectedMessage(RootEngine::Network::Message* p_message);
			void HandleChatToServerMessage(RootEngine::Network::Message* p_message);
			void HandleUserCommandMessage(RootEngine::Network::Message* p_message, RootForce::PlayerAction::PlayerAction p_action);
		};
	}
}