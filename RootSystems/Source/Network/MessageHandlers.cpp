#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Components.h>
#include <cassert>

namespace RootForce
{
	namespace Network
	{
		MessageHandler::MessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger)
			: m_peer(p_peer)
			, m_logger(p_logger) {}
		MessageHandler::~MessageHandler() {}


		ClientMessageHandler::ClientMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, RootEngine::GameSharedContext* p_engineContext, ECS::World* p_world)
			: MessageHandler(p_peer, p_logger)
			, m_engineContext(p_engineContext)
			, m_world(p_world)
			, m_state(ClientState::UNCONNECTED) {}

		void ClientMessageHandler::SetLanList(RootSystems::LanList* p_list)
		{
			m_list = p_list;
		}

		void ClientMessageHandler::SetChatSystem(RootForce::ChatSystem* p_chatSystem)
		{
			m_chatSystem = p_chatSystem;
		}

		void ClientMessageHandler::SetPlayerSystem(PlayerSystem* p_playerSystem)
		{
			m_playerSystem = p_playerSystem;
		}

		void ClientMessageHandler::SetWorldSystem(WorldSystem* p_worldSystem)
		{
			m_worldSystem = p_worldSystem;
		}

		ClientState::ClientState ClientMessageHandler::GetClientState() const
		{
			return m_state;
		}

		const NetworkMessage::ServerInformation& ClientMessageHandler::GetServerInformation() const
		{
			return m_serverInformation;
		}

		bool ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					m_logger->LogText(LogTag::CLIENT, LogLevel::SUCCESS, "Connection accepted");
				} return true;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection refused: server full");

					m_state = ClientState::CONNECTION_FAILED_TOO_MANY_PLAYERS;
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection terminated: booted");

					m_state = ClientState::CONNECTION_LOST;
				} return true;

				case ID_CONNECTION_LOST:
				{
					m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection terminated: connection to server lost");

					m_state = ClientState::CONNECTION_LOST;
				} return true;

				case ID_CONNECTION_ATTEMPT_FAILED:
				{
					m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection attempt failed");

					m_state = ClientState::CONNECTION_FAILED;
				} return true;

				case ID_UNCONNECTED_PONG:
				{
					p_bs->IgnoreBytes(4); // Timestamp

					RootSystems::ServerInfo info;
					info.Serialize(false, p_bs);

					RootSystems::ServerInfoInternal internalInfo;
					internalInfo.SetInfo(info, p_packet);

					RootSystems::ServerInfoInternal copy = internalInfo;

					m_list->AddServer(internalInfo);
				} return true;

				case NetworkMessage::MessageType::GameStateDelta:
				{

				} return true;

				case NetworkMessage::MessageType::Chat:
				{
					NetworkMessage::Chat m;
					m.Serialize(false, p_bs);

					m_chatSystem->JSAddMessage(m.Message.C_String());
				} return true;

				case NetworkMessage::MessageType::UserConnected:
				{
					
				} return true;

				case NetworkMessage::MessageType::UserDisconnected:
				{
					
				} return true;

				case NetworkMessage::MessageType::PlayerCommand:
				{
					
				} return true;

				case NetworkMessage::MessageType::DestroyEntities:
				{
					
				} return true;

				case NetworkMessage::MessageType::SpawnUser:
				{

				} return true;

				case NetworkMessage::MessageType::LoadMap:
				{

				} return true;

				case NetworkMessage::MessageType::SetMaxPlayers:
				{

				} return true;

				case NetworkMessage::MessageType::SetGameMode:
				{

				} return true;

				case NetworkMessage::MessageType::SetMatchTime:
				{

				} return true;

				case NetworkMessage::MessageType::SetKillCount:
				{

				} return true;

				case NetworkMessage::MessageType::ServerInformation:
				{
					m_serverInformation.Serialize(false, p_bs);

					// Load the map
					m_worldSystem->CreateWorld(m_serverInformation.MapName.C_String());

					// Send load map status
					NetworkMessage::LoadMapStatus m;
					m.Status = NetworkMessage::LoadMapStatus::STATUS_COMPLETED;
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::LoadMapStatus);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, false);
				} return true;
			}

			return false;
		}



		ServerMessageHandler::ServerMessageHandler(RootEngine::GameSharedContext* p_engineContext, RakNet::RakPeerInterface* p_peer, Logging* p_logger, ECS::World* p_world)
			: MessageHandler(p_peer, p_logger)
			, m_engineContext(p_engineContext)
			, m_world(p_world) {}

		void ServerMessageHandler::SetServerInformation(const NetworkMessage::ServerInformation& p_information)
		{
			m_information = p_information;
		}

		bool ServerMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_NEW_INCOMING_CONNECTION:
				{
					m_logger->LogText(LogTag::SERVER, LogLevel::SUCCESS, "Client connected (%s)", p_packet->systemAddress.ToString());

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::ServerInformation);
					m_information.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);

					// TODO: Set state for user, waiting for map load status.
					
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client disconnected (%s)", p_packet->systemAddress.ToString());
					
					NetworkMessage::UserDisconnected m;
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					// TODO: Send only to clients who have received user connected for all other clients?
					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case ID_CONNECTION_LOST:
				{
					m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client disconnected (%s)", p_packet->systemAddress.ToString());
					
					NetworkMessage::UserDisconnected m;
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					// TODO: Send only to clients who have received user connected for all other clients?
					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case ID_UNCONNECTED_PING:
				{
					m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "A LAN-discovery request has been received from %s.", p_packet->systemAddress.ToString());
				} return true;

				case NetworkMessage::MessageType::Chat:
				{
					NetworkMessage::Chat m;
					m.Serialize(false, p_bs);

					m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Chat from client (%s): %s", p_packet->systemAddress.ToString(), m.Message.C_String());

					// Forward the message to broadcast
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::Chat);
					
					m.Sender = (int8_t) m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case NetworkMessage::MessageType::PlayerCommand:
				{
					// TODO: Fix this after connections work.
				} return true;

				case NetworkMessage::MessageType::LoadMapStatus:
				{
					NetworkMessage::LoadMapStatus m;
					m.Serialize(false, p_bs);

					switch (m.Status)
					{
						case NetworkMessage::LoadMapStatus::STATUS_LOADING:
						{
							// TODO: Ignore this for now.
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_COMPLETED:
						{
							m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client (%s) successfully loaded map", p_packet->systemAddress.ToString());
							
							m_engineContext->m_script->SetFunction("Player", "OnCreate");
							m_engineContext->m_script->AddParameterNumber(m_peer->GetIndexFromSystemAddress(p_packet->systemAddress));
							m_engineContext->m_script->AddParameterNumber(Network::ReservedActionID::CONNECT);
							m_engineContext->m_script->ExecuteScript();
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_FAILED_MAP_NOT_FOUND:
						{
							m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client (%s) failed to load map: Map not found", p_packet->systemAddress.ToString());
							m_peer->CloseConnection(p_packet->systemAddress, true);
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_FAILED_INVALID_HASH:
						{
							m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client (%s) failed to load map: Invalid map hash", p_packet->systemAddress.ToString());
							m_peer->CloseConnection(p_packet->systemAddress, true);
						} break;
					}
				} return true;
			}
			
			return false;
		}
	}
}