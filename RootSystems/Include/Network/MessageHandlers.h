#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <RakNet/GetTime.h>
#include <Utility/ECS/Include/World.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/ChatSystem.h>
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/Network/LanList.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/AbilitySpawnSystem.h>
#include <RootSystems/Include/DeserializationSystem.h>

namespace RootForce
{
	namespace Network
	{
		class MessageHandler
		{
		public:
			MessageHandler(RakNet::RakPeerInterface* p_peer);
			virtual ~MessageHandler();

			virtual bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet, RakNet::Time p_timestamp) = 0;
		protected:
			RakNet::RakPeerInterface* m_peer;
		};


		/*
			Handle messages that arrive to a client from the server.
		*/
		class ClientMessageHandler : public MessageHandler
		{
		public:
			ClientMessageHandler(RakNet::RakPeerInterface* p_peer, ECS::World* p_world);

			void SetLanList(RootSystems::LanList* p_list);
			void SetChatSystem(RootForce::ChatSystem* p_chatSystem);
			void SetWorldSystem(WorldSystem* p_worldSystem);
			void SetAbilitySpawnSystem(AbilitySpawnSystem* p_abilityspawnSystem) { m_abilitySpawnSystem = p_abilityspawnSystem; }
			void SetDeserializationSystem(DeserializationSystem* p_deserializationSystem) { p_deserializationSystem = p_deserializationSystem; }

			bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet, RakNet::Time p_timestamp);
		private:
			ECS::World* m_world;
			RootSystems::LanList* m_list;
			RootForce::ChatSystem* m_chatSystem;
			WorldSystem* m_worldSystem;
			AbilitySpawnSystem* m_abilitySpawnSystem;
			DeserializationSystem* m_deserializationSystem;
		};


		/*
			Handle messages that arrive to the server from clients.
		*/
		class ServerMessageHandler : public MessageHandler
		{
		public:
			ServerMessageHandler(RakNet::RakPeerInterface* p_peer, ECS::World* p_world);

			bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet, RakNet::Time p_timestamp);
		private:
			ECS::World* m_world;
		};
	}
}

#endif