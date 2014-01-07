#pragma once

#include <Utility/ECS/Include/World.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/ChatSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/Network/LanList.h>
#include <RootSystems/Include/Network/NetworkEntityMap.h>

namespace RootForce
{
	namespace Network
	{
		class MessageHandler
		{
		public:
			MessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger);
			virtual ~MessageHandler();

			virtual bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet) = 0;
		protected:
			RakNet::RakPeerInterface* m_peer;
			Logging* m_logger;
		};


		namespace ClientState
		{
			enum ClientState
			{
				UNCONNECTED,
				AWAITING_CONFIRMATION,
				CONNECTED
			};
		}

		class ClientMessageHandler : public MessageHandler
		{
		public:
			ClientMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, RootEngine::GameSharedContext* p_engineContext, ECS::World* p_world);

			void SetLanList(RootSystems::LanList* p_list);
			void SetChatSystem(RootForce::ChatSystem* p_chatSystem);
			void SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap);
			void SetPlayerSystem(PlayerSystem* p_playerSystem);
			ClientState::ClientState GetClientState() const;

			bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		private:
			RootEngine::GameSharedContext* m_engineContext;
			ECS::World* m_world;
			RootSystems::LanList* m_list;
			RootForce::ChatSystem* m_chatSystem;
			NetworkEntityMap* m_networkEntityMap;
			PlayerSystem* m_playerSystem;

			ClientState::ClientState m_state;
		};

		class ServerMessageHandler : public MessageHandler
		{
		public:
			ServerMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, ECS::World* p_world);

			void SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap);

			bool ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet);
		private:
			ECS::World* m_world;
			NetworkEntityMap* m_networkEntityMap;
		};
	}
}