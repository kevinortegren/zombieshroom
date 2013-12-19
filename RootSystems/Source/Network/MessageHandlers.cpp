#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <cassert>

namespace RootForce
{
	namespace Network
	{
		MessageHandler::MessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger)
			: m_peer(p_peer)
			, m_logger(p_logger) {}
		MessageHandler::~MessageHandler() {}


		ClientMessageHandler::ClientMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, ECS::World* p_world)
			: MessageHandler(p_peer, p_logger)
			, m_world(p_world) {}

		void ClientMessageHandler::SetLanList(RootSystems::LanList* p_list)
		{
			m_list = p_list;
		}

		void ClientMessageHandler::SetChatSystem(RootForce::ChatSystem* p_chatSystem)
		{
			m_chatSystem = p_chatSystem;
		}

		void ClientMessageHandler::SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap)
		{
			m_networkEntityMap = p_networkEntityMap;
		}

		void ClientMessageHandler::SetPlayerSystem(PlayerSystem* p_playerSystem)
		{
			m_playerSystem = p_playerSystem;
		}

		void ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "CLIENT: Connection accepted");

					// Create temporary entity for player
					const std::string PLAYER_NAME = "John Doe"; // TODO: Get this from config/options.

					m_playerSystem->CreatePlayer();
					ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Player");
					TemporaryId_t tId = m_networkEntityMap->AddEntity(entity);

					NetworkClientComponent* clientComponent = m_world->GetEntityManager()->CreateComponent<NetworkClientComponent>(entity);
					clientComponent->Name = PLAYER_NAME;
					clientComponent->State = NetworkClientComponent::EXPECTING_USER_INFO;
					clientComponent->UserID = -1;

					NetworkComponent* networkComponent = m_world->GetEntityManager()->CreateComponent<NetworkComponent>(entity);

					// Send a user info message to the server
					MessageUserInfo m;
					m.PlayerEntity.EntityType = EntityCreated::TYPE_PLAYER;
					m.PlayerEntity.SynchronizedID = SYNCHRONIZED_ID_NONE;
					m.PlayerEntity.TemporaryID = tId;
					m.PlayerName = PLAYER_NAME.c_str();

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::UserInfo);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);
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
					MessageChat m;
					m.Serialize(false, p_bs);

					m_chatSystem->JSAddMessage(m.Message.C_String());
				} break;

				case MessageType::UserConnected:
				{
					MessageUserConnected m;
					m.Serialize(false, p_bs);

					// Check whether we have a temporary entity already for the connected player (would imply that the connected player is us).
					if (m.UserInfo.PlayerEntity.TemporaryID == TEMPORARY_ID_NONE)
					{
						// This is not a response to a join request. We do not have a temporary entity for this player. Create an entity.
						ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
						TemporaryId_t tId = m_networkEntityMap->AddEntity(entity);
						SynchronizedId_t sId = m.UserInfo.PlayerEntity.SynchronizedID;
						m_networkEntityMap->SetSynchronizedId(tId, sId);
						m_networkEntityMap->AssociatePlayerEntityWithUserID(m.UserID, entity);
						
						NetworkClientComponent* clientComponent = m_world->GetEntityManager()->CreateComponent<NetworkClientComponent>(entity);
						clientComponent->Name = m.UserInfo.PlayerName.C_String();
						clientComponent->State = NetworkClientComponent::CONNECTED;
						clientComponent->UserID = m.UserID;

						NetworkComponent* networkComponent = m_world->GetEntityManager()->CreateComponent<NetworkComponent>(entity);

						// Log the connection
						m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Remote player connected (ID: %d, Name: %s)", m.UserID, m.UserInfo.PlayerName.C_String());
					}
					else
					{
						// This is a response to our join request, we should have a temporary player entity.
						ECS::Entity* entity = m_networkEntityMap->GetTemporaryEntity(m.UserInfo.PlayerEntity.TemporaryID);
						assert(entity != nullptr);

						// Synchronize our temporary entity with the ID the server has.
						m_networkEntityMap->SetSynchronizedId(m.UserInfo.PlayerEntity.TemporaryID, m.UserInfo.PlayerEntity.SynchronizedID);
						m_networkEntityMap->AssociatePlayerEntityWithUserID(m.UserID, entity);

						// Update the user info
						NetworkClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<NetworkClientComponent>(entity);
						clientComponent->State = NetworkClientComponent::CONNECTED;
						clientComponent->UserID = m.UserID;

						// Log the connection
						m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "We have connected (ID: %d, Name: %s)", m.UserID, m.UserInfo.PlayerName.C_String());
					}
				} break;

				case MessageType::UserDisconnected:
				{
					// Another client has disconnected
					MessageUserDisconnected m;
					m.Serialize(false, p_bs);
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

					MessageUserDisconnected m;
					m.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} break;

				case ID_CONNECTION_LOST:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Client connection lost (%s)", p_packet->systemAddress.ToString());

					MessageUserDisconnected m;
					m.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
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
					m_networkEntityMap->AssociatePlayerEntityWithUserID(m_peer->GetIndexFromSystemAddress(p_packet->systemAddress), entity);

					NetworkClientComponent* clientComponent = m_world->GetEntityManager()->CreateComponent<NetworkClientComponent>(entity);
					clientComponent->Name = std::string(m.PlayerName.C_String());
					clientComponent->State = NetworkClientComponent::CONNECTED;
					clientComponent->UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);

					NetworkComponent* networkComponent = m_world->GetEntityManager()->CreateComponent<NetworkComponent>(entity);

					// Get a list of the connected players
					DataStructures::List<RakNet::SystemAddress> connectedAddresses;
					DataStructures::List<RakNet::RakNetGUID> connectedGUIDs;
					m_peer->GetSystemList(connectedAddresses, connectedGUIDs);

					// Notify the rest of the connected players of the synchronized entity.
					{
						MessageUserConnected n;
						n.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
						n.UserInfo.PlayerEntity.EntityType = EntityCreated::TYPE_PLAYER;
						n.UserInfo.PlayerEntity.SynchronizedID = sId;
						n.UserInfo.PlayerEntity.TemporaryID = TEMPORARY_ID_NONE;
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

					// Notify the connected player of all of the connected players and their synchronized entities (including self).
					{
						for (unsigned int i = 0; i < connectedAddresses.Size(); ++i)
						{
							ECS::Entity* otherPlayerEntity = m_networkEntityMap->GetPlayerEntityFromUserID(i);
							if (otherPlayerEntity == nullptr)
							{
								m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "Unidentified client (%s) encountered while sending connection list to connected client. User info has not been sent yet?", connectedAddresses[i].ToString(false));
								continue;
							}

							NetworkClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<NetworkClientComponent>(otherPlayerEntity);
							assert(otherClientComponent != nullptr);

							MessageUserConnected n;
							n.UserID = i;
							n.UserInfo.PlayerEntity.EntityType = EntityCreated::TYPE_PLAYER;
							n.UserInfo.PlayerEntity.SynchronizedID = m_networkEntityMap->GetSynchronizedId(otherPlayerEntity);
							n.UserInfo.PlayerEntity.TemporaryID = TEMPORARY_ID_NONE;
							n.UserInfo.PlayerName = otherClientComponent->Name.c_str();
							
							// If self, also add the temporary ID for the entity.
							if (connectedAddresses[i] == p_packet->systemAddress)
								n.UserInfo.PlayerEntity.TemporaryID = m.PlayerEntity.TemporaryID;

							RakNet::BitStream bs;
							bs.Write((RakNet::MessageID) MessageType::UserConnected);
							n.Serialize(true, &bs);

							m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);
						}
					}
				} break;
			}
		}
	}
}