#pragma once

#ifndef COMPILE_LEVEL_EDITOR

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

namespace RootForce
{
	namespace Network
	{
		class MessageHandler
		{
		public:
			MessageHandler(RakNet::RakPeerInterface* p_peer);
			virtual ~MessageHandler();

			virtual bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet) = 0;
		protected:
			RakNet::RakPeerInterface* m_peer;
			float m_lastHalfPing;
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

			bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		private:
			ECS::World* m_world;
			RootSystems::LanList* m_list;
			RootForce::ChatSystem* m_chatSystem;
			WorldSystem* m_worldSystem;
		};


		/*
			Handle messages that arrive to the server from clients.
		*/
		class ServerMessageHandler : public MessageHandler
		{
		public:
			ServerMessageHandler(RakNet::RakPeerInterface* p_peer, ECS::World* p_world);

			bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		private:
			ECS::World* m_world;
		};
	}
}

#endif