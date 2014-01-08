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

		void ClientMessageHandler::SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap)
		{
			m_networkEntityMap = p_networkEntityMap;
		}

		void ClientMessageHandler::SetPlayerSystem(PlayerSystem* p_playerSystem)
		{
			m_playerSystem = p_playerSystem;
		}

		void ClientMessageHandler::SetWorldSystem(WorldSystem* p_worldSystem)
		{
			m_worldSystem = p_worldSystem;
		}

		const MessagePlayData& ClientMessageHandler::GetServerInfo() const
		{
			return m_serverInfo;
		}

		ClientState::ClientState ClientMessageHandler::GetClientState() const
		{
			return m_state;
		}

		bool ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "CLIENT: Connection accepted");
				} return true;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection refused: server full");

					m_state = ClientState::CONNECTION_FAILED_TOO_MANY_PLAYERS;
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection terminated: booted");

					m_state = ClientState::CONNECTION_LOST;
				} return true;

				case ID_CONNECTION_LOST:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection terminated: connection to server lost");

					m_state = ClientState::CONNECTION_LOST;
				} return true;

				case ID_CONNECTION_ATTEMPT_FAILED:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::WARNING, "CLIENT: Connection attempt failed");

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

				case MessageType::ChatToClient:
				{
					MessageChat m;
					m.Serialize(false, p_bs);

					m_chatSystem->JSAddMessage(m.Message.C_String());
				} return true;

				case MessageType::UserConnected:
				{
					MessageUserConnected m;
					m.Serialize(false, p_bs);

					// Check whether we have a temporary entity already for the connected player (would imply that the connected player is us).
					if (m.UserInfo.PlayerEntity.TemporaryID == TEMPORARY_ID_NONE)
					{
						// This is not a response to a join request. We do not have a temporary entity for this player. Create an entity.
						ECS::Entity* entity = m_networkEntityMap->GetSynchronizedEntity(m.UserInfo.PlayerEntity.SynchronizedID);
						if (entity == nullptr)
						{
							// This is a remote client, we need to create the entity ourselves
							entity = m_world->GetEntityManager()->CreateEntity();
							TemporaryId_t tId = m_networkEntityMap->AddEntity(entity);
							SynchronizedId_t sId = m.UserInfo.PlayerEntity.SynchronizedID;
							m_networkEntityMap->SetSynchronizedId(tId, sId);
							m_networkEntityMap->AssociatePlayerEntityWithUserID(m.UserID, entity);
						}

						// Add client specific components
						NetworkClientComponent* clientComponent = m_world->GetEntityManager()->CreateComponent<NetworkClientComponent>(entity);
						clientComponent->Name = m.UserInfo.PlayerName.C_String();
						clientComponent->State = NetworkClientComponent::CONNECTED;
						clientComponent->UserID = m.UserID;

						NetworkComponent* networkComponent = m_world->GetEntityManager()->CreateComponent<NetworkComponent>(entity);

						Transform* transform = m_world->GetEntityManager()->CreateComponent<Transform>(entity);
						
						Renderable* renderable = m_world->GetEntityManager()->CreateComponent<Renderable>(entity);
						renderable->m_model = m_engineContext->m_resourceManager->LoadCollada("testchar");
						renderable->m_material = m_engineContext->m_resourceManager->GetMaterial("testchar");
						renderable->m_material->m_diffuseMap = m_engineContext->m_resourceManager->LoadTexture("WStexture", Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_normalMap = m_engineContext->m_resourceManager->LoadTexture("WSSpecular", Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_specularMap = m_engineContext->m_resourceManager->LoadTexture("WSNormal", Render::TextureType::TEXTURE_2D);
						renderable->m_material->m_effect = m_engineContext->m_resourceManager->LoadEffect("Mesh_NormalMap");

						// Log the connection
						m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Remote player connected (ID: %d, Name: %s)", m.UserID, m.UserInfo.PlayerName.C_String());
					}
					else
					{
						ECS::Entity* entity = m_networkEntityMap->GetSynchronizedEntity(m.UserInfo.PlayerEntity.SynchronizedID);
						if (entity == nullptr)
						{
							// This is a response to our join request, we should have a temporary player entity.
							entity = m_networkEntityMap->GetTemporaryEntity(m.UserInfo.PlayerEntity.TemporaryID);
							assert(entity != nullptr);

							// Synchronize our temporary entity with the ID the server has.
							m_networkEntityMap->SetSynchronizedId(m.UserInfo.PlayerEntity.TemporaryID, m.UserInfo.PlayerEntity.SynchronizedID);
							m_networkEntityMap->AssociatePlayerEntityWithUserID(m.UserID, entity);
						}

						// Update the user info
						NetworkClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<NetworkClientComponent>(entity);
						clientComponent->State = NetworkClientComponent::CONNECTED;
						clientComponent->UserID = m.UserID;

						// Log the connection
						m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "We have connected (ID: %d, Name: %s)", m.UserID, m.UserInfo.PlayerName.C_String());

						m_state = ClientState::CONNECTED;
					}
				} return true;

				case MessageType::PlayData:
				{
					m_serverInfo.Serialize(false, p_bs);
					
					// Create the world
					// TODO: Consider that a server will be creating an instance of the world itself. Hence, a local client will have duplicates.
					std::string levelName;
					levelName = m_serverInfo.MapName;
					levelName = levelName.substr(0, levelName.size() - std::string(".world").size());
					m_worldSystem->CreateWorld(levelName);


					// Create temporary entity for player
					const std::string PLAYER_NAME = "John Doe"; // TODO: Get this from config/options.

					m_playerSystem->CreatePlayer(0);
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
				} return true;

				case MessageType::UserDisconnected:
				{
					// Another client has disconnected
					MessageUserDisconnected m;
					m.Serialize(false, p_bs);

					ECS::Entity* entity = m_networkEntityMap->GetPlayerEntityFromUserID(m.UserID);
					NetworkClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<NetworkClientComponent>(entity);
					
					m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Client (ID: %d, Name: %s) disconnected", m.UserID, clientComponent->Name.c_str());
					
					m_world->GetEntityManager()->RemoveAllComponents(entity);
					m_world->GetEntityManager()->RemoveEntity(entity);
				} return true;

				case MessageType::HACK_TransformUpdate:
				{
					HACK_MessageTransformUpdate m;
					m.Serialize(false, p_bs);

					ECS::Entity* player = m_networkEntityMap->GetPlayerEntityFromUserID(m_peer->GetIndexFromSystemAddress(p_packet->systemAddress));
					if (player != nullptr)
					{
						Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(player);
						transform->m_position = m.Position;
						transform->m_orientation = m.Orientation;
					}
				} return true;
			}

			return false;
		}



		ServerMessageHandler::ServerMessageHandler(RakNet::RakPeerInterface* p_peer, Logging* p_logger, ECS::World* p_world)
			: MessageHandler(p_peer, p_logger)
			, m_world(p_world)
			, m_networkEntityMap(nullptr) {}

		void ServerMessageHandler::SetNetworkEntityMap(NetworkEntityMap* p_networkEntityMap)
		{
			m_networkEntityMap = p_networkEntityMap;
		}

		void ServerMessageHandler::SetPlayDataResponse(const MessagePlayData& p_response)
		{
			m_response = p_response;
		}

		bool ServerMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet)
		{
			switch (p_id)
			{
				case ID_NEW_INCOMING_CONNECTION:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::SUCCESS, "SERVER: Client connected (%s)", p_packet->systemAddress.ToString());

					// Send server info
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::PlayData);
					m_response.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Client disconnected (%s)", p_packet->systemAddress.ToString());

					MessageUserDisconnected m;
					m.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case ID_CONNECTION_LOST:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Client connection lost (%s)", p_packet->systemAddress.ToString());

					MessageUserDisconnected m;
					m.UserID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::UserDisconnected);
					m.Serialize(true, &bs);

					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case ID_UNCONNECTED_PING:
				{
					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: A LAN-discovery request has been received from %s.", p_packet->systemAddress.ToString());
				} return true;

				case MessageType::ChatToServer:
				{
					MessageChat m;
					m.Serialize(false, p_bs);

					m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "SERVER: Chat from client (%s): %s", p_packet->systemAddress.ToString(), m.Message.C_String());

					// Forward the message to broadcast
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) MessageType::ChatToClient);

					m.SenderID = (int8_t) m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					m.Serialize(true, &bs);
					m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				} return true;

				case MessageType::UserInfo:
				{
					MessageUserInfo m;
					m.Serialize(false, p_bs);

					// Create a synchronized entity for the connected player.
					// If loopback, we need to check if a temporary entity exists also
					ECS::Entity* entity = nullptr;
					TemporaryId_t tId = TEMPORARY_ID_NONE;
					if (p_packet->systemAddress.IsLoopback())
					{
						// Local client, temporary entity has been created
						entity = m_networkEntityMap->GetTemporaryEntity(m.PlayerEntity.TemporaryID);
						assert(entity != nullptr);

						tId = m.PlayerEntity.TemporaryID;
					}
					else
					{
						// Remote client, no entity exists in this application instance
						entity = m_world->GetEntityManager()->CreateEntity();
						tId = m_networkEntityMap->AddEntity(entity);
					}

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
								m_logger->LogText(LogTag::NETWORK, LogLevel::NON_FATAL_ERROR, "SERVER: Unidentified client (%s) encountered while sending connection list to connected client. User info has not been sent yet?", connectedAddresses[i].ToString(false));
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
				} return true;

				case MessageType::HACK_TransformUpdate:
				{
					HACK_MessageTransformUpdate m;
					m.Serialize(false, p_bs);

					ECS::Entity* player = m_networkEntityMap->GetPlayerEntityFromUserID(m_peer->GetIndexFromSystemAddress(p_packet->systemAddress));
					if (player != nullptr)
					{
						Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(player);
						transform->m_position = m.Position;
						transform->m_orientation = m.Orientation;

						// Get a list of the connected players
						DataStructures::List<RakNet::SystemAddress> connectedAddresses;
						DataStructures::List<RakNet::RakNetGUID> connectedGUIDs;
						m_peer->GetSystemList(connectedAddresses, connectedGUIDs);

						for (unsigned int i = 0; i < connectedAddresses.Size(); ++i)
						{
							if (connectedAddresses[i] != p_packet->systemAddress)
							{
								m_peer->Send(p_bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, connectedAddresses[i], false);
							}
						}
					}
				} return true;
			}

			return false;
		}
	}
}