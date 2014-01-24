#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <RakNet/RakPeerInterface.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootSystems/Include/Network/LanList.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/ChatSystem.h>

namespace RootForce
{
	namespace Network
	{
		struct RemoteClient
		{
			int8_t Slot;
			RakNet::SystemAddress Address;
		};

		class Client
		{
		public:
			Client(Logging* p_logger, ECS::World* p_world);
			~Client();

			bool Connect(const std::string& p_address, const std::string& p_password, unsigned short p_port, bool p_isRemote);
			void PingNetwork(RootSystems::LanList* p_list, unsigned short p_port);

			void SetMessageHandler(MessageHandler* p_messageHandler);
			void SetChatSystem(RootForce::ChatSystem* p_chatSystem);
			void Update();

			RakNet::RakPeerInterface* GetPeerInterface();
		private:
			ECS::World* m_world;
			Logging* m_logger;
			RakNet::RakPeerInterface* m_peer;
			MessageHandler* m_messageHandler;
			RootForce::ChatSystem* m_chatSystem;
		};
	}
}

#endif