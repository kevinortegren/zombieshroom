#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootSystems/Include/Network/Messages.h>

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

		void ClientMessageHandler::SetChatSystem(RootForce::ChatSystem* p_chatSystem)
		{
			m_chatSystem = p_chatSystem;
		}

		void ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "CLIENT: Connection accepted");

					// TODO: Create temporary entity for player
				} break;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection refused: server full");
				} break;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection terminated: booted");
				} break;

				case ID_CONNECTION_LOST:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection terminated: connection to server lost");
				} break;

				case ID_CONNECTION_ATTEMPT_FAILED:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection attempt failed");
				} break;

				case ID_UNCONNECTED_PONG:
				{
					p_bs->IgnoreBytes(4); // Timestamp

					RootSystems::ServerInfo info;
					info.Serialize(false, p_bs);

					RootSystems::ServerInfoInternal internalInfo;
					internalInfo.SetInfo(info, p_packet);

					RootSystems::ServerInfoInternal copy = internalInfo;

					m_list->AddServer(internalInfo);
				} break;

				case MessageType::ChatToClient:
				{
					MessageChat c;
					c.Serialize(false, p_bs);

					m_chatSystem->JSAddMessage(c.Message.C_String());
				} break;
			}
		}



		ServerMessageHandler::ServerMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, ECS::World* p_world)
			: MessageHandler(p_peer, p_logger)
			, m_world(p_world)
			, m_networkEntityMap(nullptr) {}

		void ServerMessageHandler::SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap)
		{
			m_networkEntityMap = p_networkEntityMap;
		}

		void ServerMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_NEW_INCOMING_CONNECTION:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "SERVER: Client connected (%s)", p_packet->systemAddress.ToString());
				} break;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Client disconnected (%s)", p_packet->systemAddress.ToString());
				} break;

				case ID_CONNECTION_LOST:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Client connection lost (%s)", p_packet->systemAddress.ToString());
				} break;

				case ID_UNCONNECTED_PING:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: A LAN-discovery request has been received from %s.", p_packet->systemAddress.ToString());
				} break;

				case MessageType::ChatToServer:
				{
					MessageChat m;
					m.Serialize(false, p_bs);

					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Chat from client (%s): %s", p_packet->systemAddress.ToString(), m.Message.C_String());

					// Forward the message to broadcast
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::ChatToClient);

					m.SenderID = (int8_t) m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					m.Serialize(true, &bs);
					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} break;

				case MessageType::UserInfo:
				{
					MessageUserInfo m;
					m.Serialize(false, p_bs);

					// Create a synchronized entity for the connected player.
					ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
					TemporaryId_t tId = m_networkEntityMap->AddEntity(entity);
					SynchronizedId_t sId = m_networkEntityMap->NextSynchronizedId();
					m_networkEntityMap->SetSynchronizedId(tId, sId);

					// Notify the connected player of the synchronized entity.
					{
						MessageUserConnected n;
						n.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
						n.UserInfo.PlayerEntity.EntityType = EntityCreated::TYPE_PLAYER;
						n.UserInfo.PlayerEntity.SynchronizedID = sId;
						n.UserInfo.PlayerEntity.TemporaryID = m.PlayerEntity.TemporaryID;
						n.UserInfo.PlayerName = m.PlayerName;

						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) MessageType::UserConnected);
						n.Serialize(true, &bs);

						m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);
					}

					// Notify the rest of the connected players of the synchronized entity.
					{
						DataStructures::List<RakNet::SystemAddress> connectedAddresses;
						DataStructures::List<RakNet::RakNetGUID> connectedGUIDs;
						m_peer->GetSystemList(connectedAddresses, connectedGUIDs);

						MessageUserConnected n;
						n.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
						n.UserInfo.PlayerEntity.EntityType = EntityCreated::TYPE_PLAYER;
						n.UserInfo.PlayerEntity.SynchronizedID = sId;
						n.UserInfo.PlayerEntity.TemporaryID = -1;
						n.UserInfo.PlayerName = m.PlayerName;

						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) MessageType::UserConnected);
						n.Serialize(true, &bs);

						for (unsigned int i = 0; i < connectedAddresses.Size(); ++i)
						{
							if (connectedAddresses[i] != p_packet->systemAddress)
							{
								m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, connectedAddresses[i], false);
							}
						}
					}

					// Notify the connected player of the rest of the connected players and their synchronized entities.
					{
						// TODO: Need to add functionality to the NetworkEntityMap to be able to yield Entity from SystemAddress.
					}
				} break;
			}
		}
	}
}