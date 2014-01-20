#pragma once

#include <RakNet/RakPeerInterface.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerConfig.h>
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

			const NetworkMessage::ServerInformation& GetServerInformation() const;
			void SetServerInformation(const NetworkMessage::ServerInformation& p_information);

			void SetMessageHandler(MessageHandler* p_messageHandler);
			void Update();

			RakNet::RakPeerInterface* GetPeerInterface();
		private:
			Logging* m_logger;
			ECS::World* m_world;

			NetworkMessage::ServerInformation m_information;
			RakNet::RakPeerInterface* m_peer;
			MessageHandler* m_messageHandler;

			void UpdatePingResponse();
		};
	}
}