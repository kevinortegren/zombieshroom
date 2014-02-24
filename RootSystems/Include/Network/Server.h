#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <RakNet/RakPeerInterface.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerConfig.h>
#include <RootSystems/Include/Network/PacketLogger.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <string>

namespace RootForce
{
	namespace Network
	{
		class Server
		{
		public:
			Server(Logging* p_logger, ECS::World* p_world, const RootSystems::ServerConfig& p_config);
			~Server();

			// For initializing server components on an existing server socket
			void Initialize( WorldSystem* p_worldSystem, AbilitySpawnSystem* p_abilitySpawnSystem, const RootSystems::ServerConfig& p_config, bool p_isDedicated );

			const NetworkMessage::ServerInformation& GetServerInformation() const;
			void SetServerInformation(const NetworkMessage::ServerInformation& p_information);

			void SetMessageHandler(MessageHandler* p_messageHandler);
			void Update();
			//void Send(RakNet::BitStream* p_bs, PacketPriority p_priority, PacketReliability p_reliability, char p_orderingChannel, RakNet::AddressOrGUID p_systemIdentifier, bool p_broadcast, uint32_t p_forceReceiptNumber = 0U);

			RakNet::RakPeerInterface* GetPeerInterface();
		private:
			Logging* m_logger;
			ECS::World* m_world;

			NetworkMessage::ServerInformation m_information;
			RakNet::RakPeerInterface* m_peer;
			MessageHandler* m_messageHandler;
			float m_worldDeltaTimer;
			PacketLogger m_packetLogger;

			void UpdatePingResponse();
		};
	}
}

#endif