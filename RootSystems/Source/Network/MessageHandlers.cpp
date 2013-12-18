#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerInfo.h>

namespace RootForce
{
	namespace Network
	{
		MessageHandler::MessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger)
			: m_peer(p_peer)
			, m_logger(p_logger) {}
		MessageHandler::~MessageHandler() {}


		ClientMessageHandler::ClientMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger)
			: MessageHandler(p_peer, p_logger) {}

		void ClientMessageHandler::SetLanList(RootSystems::LanList* p_list)
		{
			m_list = p_list;
		}

		void ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_UNCONNECTED_PONG:
				{
					p_bs->IgnoreBytes(4);

					RootSystems::ServerInfo info;
					info.Serialize(false, p_bs);

					RootSystems::ServerInfoInternal internalInfo;
					internalInfo.SetInfo(info, p_packet);

					RootSystems::ServerInfoInternal copy = internalInfo;

					m_list->AddServer(internalInfo);
				} break;
			}
		}



		ServerMessageHandler::ServerMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger)
			: MessageHandler(p_peer, p_logger) {}

		void ServerMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_UNCONNECTED_PING:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "A LAN-discovery request has been received from %s.", p_packet->systemAddress.ToString());
				} break;
			}
		}
	}
}