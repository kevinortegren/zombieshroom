#pragma once

#include <Utility/ECS/Include/EntitySystem.h>
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
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
			ServerClientSystem(ECS::World* p_world)
				: ECS::EntitySystem(p_world)
			{
				SetUsage<NetworkClientComponent>();
			}

			void Init();
			void Begin();
			void ProcessEntity(ECS::Entity* p_entity);
			void End();

			
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



		// TODO: Change name to ServerMessageHandler
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