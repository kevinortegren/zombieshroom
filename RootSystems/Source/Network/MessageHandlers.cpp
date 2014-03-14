#ifndef COMPILE_LEVEL_EDITOR

#include <RakNet/GetTime.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/ServerInfo.h>
#include <RootSystems/Include/Network/Messages.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootSystems/Include/Components.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootSystems/Include/AbilitySpawnSystem.h>
#include <RootSystems/Include/StatChangeSystem.h>
#include <cassert>

extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootForce
{
	namespace Network
	{
		MessageHandler::MessageHandler(RakNet::RakPeerInterface* p_peer)
			: m_peer(p_peer) {}
		MessageHandler::~MessageHandler() {}


		ClientMessageHandler::ClientMessageHandler(RakNet::RakPeerInterface* p_peer, ECS::World* p_world)
			: MessageHandler(p_peer)
			, m_world(p_world)
			, m_list(nullptr)
			, m_chatSystem(nullptr)
			, m_worldSystem(nullptr) {}

		void ClientMessageHandler::SetLanList(RootSystems::LanList* p_list)
		{
			m_list = p_list;
		}

		void ClientMessageHandler::SetChatSystem(RootForce::ChatSystem* p_chatSystem)
		{
			m_chatSystem = p_chatSystem;
		}

		void ClientMessageHandler::SetWorldSystem(WorldSystem* p_worldSystem)
		{
			m_worldSystem = p_worldSystem;
		}

		bool ClientMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet, RakNet::Time p_timestamp)
		{
			// Get the client entity and its client component, which contains information about our own client.
			ECS::Entity* clientEntity = m_world->GetTagManager()->GetEntityByTag("Client");
			assert(clientEntity != nullptr);

			Network::ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);
			assert(clientComponent != nullptr);

			// Calculate the time it took for the message to reach us.
			float halfPing = float(RakNet::GetTime() - p_timestamp) * 0.001f;
			
			// Parse the different types of messages.
			switch (p_id)
			{
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					if (clientComponent->State == ClientState::AWAITING_CONNECTION_ACCEPT)
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::SUCCESS, "Connection accepted");
						clientComponent->State = ClientState::AWAITING_SERVER_INFO;

						// Send user information
						NetworkMessage::UserInformation m;
						m.Name = clientComponent->Name;

						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) ID_TIMESTAMP);
						bs.Write(RakNet::GetTime());
						bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserInformation);
						m.Serialize(true, &bs);

						m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "ID_CONNECTION_REQUEST_ACCEPTED received in an invalid state (%d)", clientComponent->State);
					}
				} return true;

				case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection refused: server full");

					clientComponent->State = ClientState::DISCONNECTED_REFUSED_TOO_MANY_PLAYERS;
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection terminated: booted");

					clientComponent->State = ClientState::DISCONNECTED_SERVER_SHUTDOWN;
				} return true;

				case ID_CONNECTION_LOST:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection terminated: connection to server lost");

					clientComponent->State = ClientState::DISCONNECTED_TIMEOUT;
				} return true;

				case ID_CONNECTION_ATTEMPT_FAILED:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Connection attempt failed");

					clientComponent->State = ClientState::DISCONNECTED_REFUSED;
				} return true;

				case ID_INVALID_PASSWORD:
				{
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Invalid password");

					clientComponent->State = ClientState::DISCONNECTED_REFUSED_INVALID_PASSWORD;
				} return true;

				case ID_UNCONNECTED_PONG:
				{
					if (clientComponent->State == ClientState::UNCONNECTED)
					{
						p_bs->IgnoreBytes(4); // Timestamp
					
						RootSystems::ServerInfoInternal info;
						info.Information.Serialize(false, p_bs);
						info.IP = p_packet->systemAddress.ToString(false);
						info.Port = p_packet->systemAddress.GetPort();

						m_list->AddServer(info);
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "ID_UNCONNECTED_PONG received in an invalid state (%d)", clientComponent->State);
					}
				} return true;

				case NetworkMessage::MessageType::GameStateDelta:
				{
					// Local clients share world with server - does not need to do anything.
					if (clientComponent->IsRemote)
					{
						// Only accept game state deltas if we are in a connected state, or awaiting our first entire game state snapshot.
						if (clientComponent->State == ClientState::AWAITING_FIRST_GAMESTATE_DELTA ||
							clientComponent->State == ClientState::CONNECTED)
						{
							//g_engineContext.m_logger->LogText(LogTag::NETWORK, LogLevel::DEBUG_PRINT, "Received DeltaWorld snapshot! Yay!");

							m_deserializationSystem->SetData(p_bs);
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "GameStateDelta received in an invalid state (%d)", clientComponent->State);
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "GameStateDelta received as a local client");
					}
				} return true;

				case NetworkMessage::MessageType::Chat:
				{
					if (ClientState::IsConnected(clientComponent->State))
					{
						NetworkMessage::Chat m;
						m.Serialize(false, p_bs);

						ECS::Entity* sender = FindEntity(g_networkEntityMap, NetworkEntityID(m.Sender, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
						if (sender != nullptr)
						{
							PlayerComponent* senderPlayerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(sender);
							assert(senderPlayerComponent != nullptr);
						
							std::string message = RootEngine::GUISystem::PreventHTMLInjections(senderPlayerComponent->Name) + ": " + m.Message.C_String();
							m_chatSystem->JSAddMessage(message);
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Chat received from unknown sender. User: %u. Message: \"%s\".", m.Sender, m.Message.C_String());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Chat received in an invalid state (%d)", clientComponent->State);
					}
				} return true;

				case NetworkMessage::MessageType::UserConnected:
				{
					NetworkMessage::UserConnected m;
					m.Serialize(false, p_bs);

					if (clientComponent->State == ClientState::AWAITING_USER_CONNECT ||
						clientComponent->State == ClientState::AWAITING_FIRST_GAMESTATE_DELTA ||
						clientComponent->State == ClientState::AWAITING_SPAWN_POINT ||
						ClientState::IsConnected(clientComponent->State))
					{
						// If we are a remote client, we need to create our own entities.
						if (clientComponent->IsRemote)
						{
							// We have received a connection acknowledgement about ourselves, set our user ID.
							if (m.IsYou)
							{
								g_engineContext.m_script->SetGlobalNumber("UserID", m.User);

								// Set the client state
								clientComponent->State = ClientState::AWAITING_FIRST_GAMESTATE_DELTA;
							}

							// Call the OnCreate script
							g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnCreate");
							g_engineContext.m_script->AddParameterNumber(m.User);
							g_engineContext.m_script->AddParameterNumber(ReservedActionID::CONNECT);
							g_engineContext.m_script->ExecuteScript();

							// Get the player entity and store the name
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							assert(playerEntity != nullptr);

							if(m.TeamID != 0)
							{
								// Call the OnCreate script
								g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnTeamSelect");
								g_engineContext.m_script->AddParameterUserData(playerEntity, sizeof(ECS::Entity*), "Entity");
								g_engineContext.m_script->AddParameterNumber(m.TeamID);
								g_engineContext.m_script->ExecuteScript();
							}

							PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
							assert(playerComponent != nullptr);

							playerComponent->Name = m.Name;
						}
						else
						{
							// For a local client, just set the client state. The server has already created the entities for us.
							if (m.IsYou)
							{
								clientComponent->State = ClientState::CONNECTED;
							}
						}

						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "User connected (%d): %s", m.User, m.Name.C_String());
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "UserConnected received in an invalid state (%d). User (%d: %s): %s.", clientComponent->State, m.User, p_packet->systemAddress.ToString(), m.Name.C_String());
					}
				} return true;


				case NetworkMessage::MessageType::UserDisconnected:
				{
					NetworkMessage::UserDisconnected m;
					m.Serialize(false, p_bs);

					// Only process if remote. For a local client, the server has already removed the disconnected client.
					if (clientComponent->IsRemote)
					{
						// Make sure we are in a valid state.
						if (clientComponent->State == ClientState::AWAITING_USER_CONNECT ||
							clientComponent->State == ClientState::AWAITING_FIRST_GAMESTATE_DELTA ||
							clientComponent->State == ClientState::AWAITING_SPAWN_POINT ||
							ClientState::IsConnected(clientComponent->State))
						{
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							
							// Make sure we have received a connect message for this player before.
							if (playerEntity != nullptr)
							{
								// Log the disconnect.
								PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
								assert(playerComponent != nullptr);

								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "User disconnected (%d: %s): %s", m.User, p_packet->systemAddress.ToString(), playerComponent->Name.c_str());

								// Delete the entities associated with this client.
								Network::DeleteEntities(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::ALL, ReservedSequenceID::ALL), m_world->GetEntityManager());
								Network::NetworkComponent::ResetSequenceForUser(m.User);
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "UserDisconnected received without the player entity existing. (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "UserDisconnected received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "UserDisconnected received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::PlayerCommand:
				{
					NetworkMessage::PlayerCommand m;
					m.Serialize(false, p_bs);

					// Only remote clients need to parse. A local server would already have updated the entities.
					if (clientComponent->IsRemote && ClientState::IsConnected(clientComponent->State))
					{
						ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
						assert(playerEntity != nullptr);

						PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
						assert(playerComponent != nullptr);

						// Make sure we are connected before parsing commands.
						if (playerComponent->TeamID != 0)
						{
							ECS::Entity* aimingEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_AIMING_DEVICE_ENTITY));
							assert(aimingEntity != nullptr);

							// Set the actions for the client, to be parsed by the action system later. Preserve ActiveAbility.
							PlayerActionComponent* playerAction = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
							assert(playerAction != nullptr);

							playerAction->MovePower = m.MovePower;
							playerAction->StrafePower = m.StrafePower;
							playerAction->Angle = m.Angle;
							playerAction->JumpTime = m.JumpTime;
							playerAction->SelectedAbility = m.SelectedAbility;

							// Set the position of the player, as given by the other client.
							PlayerPhysics* playerPhysics = m_world->GetEntityManager()->GetComponent<PlayerPhysics>(playerEntity);
							assert(playerPhysics != nullptr);
							
							Transform* playerTransform = m_world->GetEntityManager()->GetComponent<Transform>(playerEntity);
							assert(playerTransform != nullptr);

							Transform* aimingTransform = m_world->GetEntityManager()->GetComponent<Transform>(aimingEntity);
							assert(aimingTransform != nullptr);
							
							// Set the position of the player
							Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(playerEntity);
							assert(collision != nullptr);
							float verticalVelocity = g_engineContext.m_physics->GetPlayerVerticalVelocity(*collision->m_handle);
							glm::vec3 displacement = m.Position - playerTransform->m_position;
								
							// Check if the displacement has the same sign as the local velocity.
							//if (displacement.y * verticalVelocity >= 0.0f)
								playerTransform->m_position.y = m.Position.y;
							playerTransform->m_position.x = m.Position.x;
							playerTransform->m_position.z = m.Position.z;
							playerTransform->m_orientation.SetOrientation(m.Orientation);
							aimingTransform->m_orientation.SetOrientation(m.AimingDeviceOrientation);


							// Extrapolate the position using ping time.
							// TODO: Possibly let the action system and physics do the extrapolation.
							/*
							glm::vec3 facing = playerTransform->m_orientation.GetFront();
							glm::vec3 right = playerTransform->m_orientation.GetRight();
							glm::vec3 movement = facing * playerAction->MovePower + right * playerAction->StrafePower;
							if (movement != glm::vec3(0))
								movement = glm::normalize(movement) * playerPhysics->MovementSpeed * halfPing;

							playerTransform->m_position += movement;
							*/
							// Log the action (debug)
							//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "PlayerCommand received from user %d", m.User);
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "PlayerCommand received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "PlayerCommand received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::JumpStart:
				{
					NetworkMessage::JumpStart m;
					m.Serialize(false, p_bs);

					// Only remote clients need to parse. A local server would already have updated for us.
					if (clientComponent->IsRemote)
					{
						// Make sure we are connected before parsing user commands.
						if (ClientState::IsConnected(clientComponent->State))
						{
							// Make sure we have the entity associated with this player.
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							if (playerEntity != nullptr)
							{
								PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								assert(action != nullptr);
								action->JumpTime = halfPing;

								// Log the action (debug)
								//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "JumpStart received from user %d", m.User);
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "JumpStart received without the player entity existing. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "JumpStart received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "JumpStart received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::JumpStop:
				{
					NetworkMessage::JumpStop m;
					m.Serialize(false, p_bs);

					// Only remote clients need to parse. A local server would already have updated for us.
					if (clientComponent->IsRemote)
					{
						// Make sure we are connected before parsing user commands.
						if (ClientState::IsConnected(clientComponent->State))
						{
							// Make sure we have the entity associated with this player.
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							if (playerEntity != nullptr)
							{
								PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								assert(action != nullptr);
								action->JumpTime = 0.0f;

								// TODO: Maybe consider the time passed along with the message.

								// Log the action (debug)
								//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "JumpStop received from user %d", m.User);
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "JumpStop received without the player entity existing. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "JumpStop received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "JumpStop received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::AbilityEvent:
				{
					NetworkMessage::AbilityEvent m;
					m.Serialize(false, p_bs);

					// Only remote clients need to parse. A local server would already have updated for us.
					if (clientComponent->IsRemote)
					{
						// Make sure we are connected before parsing user commands.
						if (ClientState::IsConnected(clientComponent->State))
						{
							// Make sure we have the entity associated with this player.
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							if (playerEntity != nullptr)
							{
								PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								assert(action != nullptr);

								// Push the event.
								action->AbilityEvents.push(m.Event);
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "AbilityEvent received without the player entity existing. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "AbilityEvent received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "AbilityEvent received for client in invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
					}
				} return true;

				case NetworkMessage::MessageType::AbilityCooldownOff:
				{
					NetworkMessage::AbilityCooldownOff m;
					m.Serialize(false, p_bs);

					// Only remote clients are required to parse.
					if (clientComponent->IsRemote)
					{
						// Make sure we are connected.
						if (ClientState::IsConnected(clientComponent->State))
						{
							// Make sure we have the entity associated with this player.
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							if (playerEntity != nullptr)
							{
								PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
								assert(playerComponent != nullptr);

								playerComponent->AbilityScripts[m.AbilityIndex].Cooldown = 0.0f;
								playerComponent->AbilityScripts[m.AbilityIndex].OnCooldown = false;

								// Log the action (debug)
								//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "AbilityCooldownOff received from user %d", m.User);
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "AbilityCooldownOff received without the player entity existing. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "AbilityCooldownOff received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "AbilityCooldownOff received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::AbilityClaimedBy:
				{
					NetworkMessage::AbilityClaimedBy m;
					m.Serialize(false, p_bs);

					// Only remote clients need to handle this message. Local ones have already set spawnPoint->Claimed in AbilityRespawnSystem.
					if(clientComponent->IsRemote)
					{
						AbilitySpawnComponent* spawnPoint = m_world->GetEntityManager()->GetComponent<AbilitySpawnComponent>(FindEntity(g_networkEntityMap, m.AbilitySpawnPointID));
						assert(spawnPoint);
						spawnPoint->Claimed = m.User;

						ECS::Entity* player = RootForce::Network::FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
					
						// Don't set stuff on entities that don't exist.
						if (player)
						{
							TryPickupComponent* tryPickup = m_world->GetEntityManager()->GetComponent<TryPickupComponent>(player);
							assert(tryPickup);
							tryPickup->TryPickup = false;
							
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Tried to set TryPickup on a null player.");
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "AbilityClaimedBy received as a local client.");
					}
					
				} return true;

				case NetworkMessage::MessageType::DestroyEntities:
				{
					NetworkMessage::DestroyEntities m;
					m.Serialize(false, p_bs);

					// Only remote clients are allowed to remove entities.
					if (clientComponent->IsRemote)
					{
						Network::DeleteEntities(g_networkEntityMap, m.ID, m_world->GetEntityManager());
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "DestroyEntities received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::SpawnUser:
				{
					// TODO: Check if a local client really needs to receive this message.
					NetworkMessage::SpawnUser m;
					m.Serialize(false, p_bs);

					// Only remote clients should spawn players. Local players will just let the server spawn them.
					if (clientComponent->IsRemote)
					{
						// Make sure we are in the correct states.
						if (clientComponent->State == ClientState::AWAITING_SPAWN_POINT ||
							clientComponent->State == ClientState::CONNECTED)
						{
							// Make sure we have the entity associated with this player.
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							if (playerEntity != nullptr)
							{
								HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(playerEntity);
								assert(health != nullptr);

								health->SpawnIndex = m.SpawnPointIndex;
								health->SpawnPointReceived = true;

								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Received spawn location for player %u", m.User);
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "SpawnUser received without the player entity existing. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "SpawnUser received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}

					// If we were awaiting a spawn message for connection sequence, set us to connected now. For both local and remote players.
					if (clientComponent->State == ClientState::AWAITING_SPAWN_POINT)
					{
						ECS::Entity* playerEntity = m_world->GetTagManager()->GetEntityByTag("Player");
						assert(playerEntity != nullptr);

						NetworkComponent* network = m_world->GetEntityManager()->GetComponent<NetworkComponent>(playerEntity);
						assert(network != nullptr);

						// Make sure this message is about ourselves.
						if (network->ID.UserID == m.User)
						{
							clientComponent->State = ClientState::CONNECTED;
						}
					}
				} return true;

				case NetworkMessage::MessageType::Suicide:
				{
					NetworkMessage::Suicide m;
					m.Serialize(false, p_bs);

					// Only remote clients need to parse this message.
					if (clientComponent->IsRemote)
					{
						// Make sure we are in the correct state.
						if (clientComponent->State == ClientState::AWAITING_USER_CONNECT ||
							clientComponent->State == ClientState::AWAITING_FIRST_GAMESTATE_DELTA ||
							clientComponent->State == ClientState::AWAITING_SPAWN_POINT ||
							ClientState::IsConnected(clientComponent->State))
						{
							// Make sure we have the entity associated with this player.
							ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
							if (playerEntity != nullptr)
							{
								HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(playerEntity);
								PlayerComponent* playerComp = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
								TDMRuleSet* rules = m_world->GetEntityManager()->GetComponent<TDMRuleSet>(m_world->GetTagManager()->GetEntityByTag("MatchState"));
								assert(health != nullptr);
								assert(playerComp != nullptr);
								assert(rules != nullptr);

								health->Health = 0;
								playerComp->Score --;
								playerComp->Deaths ++;
								rules->TeamScore[playerComp->TeamID] --;
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Suicide received without the player entity existing. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Suicide received in an invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "Suicide received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::LoadMap:
				{
					NetworkMessage::LoadMap m;
					m.Serialize(false, p_bs);

					// Make sure we are in a state where we already have loaded a level. Otherwise the server information message will load the level anyway.
					if (clientComponent->State == ClientState::AWAITING_USER_CONNECT ||
						clientComponent->State == ClientState::AWAITING_FIRST_GAMESTATE_DELTA ||
						clientComponent->State == ClientState::AWAITING_SPAWN_POINT ||
						ClientState::IsConnected(clientComponent->State))
					{
						
						if (clientComponent->IsRemote)
						{
							// Update server information
							ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
							Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);
							serverInfo->Information.MapName = m.MapName;

							// Load the map
							m_worldSystem->LoadWorld(serverInfo->Information.MapName.C_String());
							m_abilitySpawnSystem->LoadAbilities(serverInfo->Information.AbilityPack.C_String()); 
							m_abilitySpawnSystem->AttachComponentToPoints();
						}
						// Send load map status
						NetworkMessage::LoadMapStatus n;
						n.Status = NetworkMessage::LoadMapStatus::STATUS_COMPLETED;
					
						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) ID_TIMESTAMP);
						bs.Write(RakNet::GetTime());
						bs.Write((RakNet::MessageID) NetworkMessage::MessageType::LoadMapStatus);
						n.Serialize(true, &bs);

						m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "LoadMap received in an invalid state (%d).", clientComponent->State);
					}
				} return true;

				case NetworkMessage::MessageType::SetMaxPlayers:
				{
					NetworkMessage::SetMaxPlayers m;
					m.Serialize(false, p_bs);

					if (clientComponent->IsRemote)
					{
						// Update server information
						ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
						assert(serverInfoEntity != nullptr);

						Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);
						assert(serverInfo != nullptr);

						serverInfo->Information.MaxPlayers = m.MaxPlayers;
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "SetMaxPlayers received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::SetGameMode:
				{
					NetworkMessage::SetGameMode m;
					m.Serialize(false, p_bs);

					if (clientComponent->IsRemote)
					{
						// Update server information
						ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
						assert(serverInfoEntity != nullptr);

						Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);
						assert(serverInfo != nullptr);

						serverInfo->Information.GameMode = m.GameMode;

						// TODO: Can we change game mode?
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "SetGameMode received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::SetMatchTime:
				{
					NetworkMessage::SetMatchTime m;
					m.Serialize(false, p_bs);

					if (clientComponent->IsRemote)
					{
						// Update server information
						ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
						assert(serverInfoEntity != nullptr);

						Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);
						assert(serverInfo != nullptr);

						serverInfo->Information.MatchTimeSeconds = m.Seconds;

						// Update rules
						ECS::Entity* matchStateEntity = m_world->GetTagManager()->GetEntityByTag("MatchState");
						assert(matchStateEntity != nullptr);

						TDMRuleSet* rules = m_world->GetEntityManager()->GetComponent<TDMRuleSet>(matchStateEntity);
						assert(rules != nullptr);

						rules->TimeLeft = (float) m.Seconds;
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "SetMatchTime received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::SetKillCount:
				{
					NetworkMessage::SetKillCount m;
					m.Serialize(false, p_bs);

					if (clientComponent->IsRemote)
					{
						// Update server information
						ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
						assert(serverInfoEntity != nullptr);

						Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);
						assert(serverInfo != nullptr);

						serverInfo->Information.KillCount = m.Count;

						// Update rules
						ECS::Entity* matchStateEntity = m_world->GetTagManager()->GetEntityByTag("MatchState");
						assert(matchStateEntity != nullptr);

						TDMRuleSet* rules = m_world->GetEntityManager()->GetComponent<TDMRuleSet>(matchStateEntity);
						assert(rules != nullptr);

						rules->ScoreLimit = (int) m.Count;
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "SetKillCount received as a local client.");
					}
				} return true;

				case NetworkMessage::MessageType::ServerInformation:
				{
					// Make sure we are in the correct state.
					if (clientComponent->State == ClientState::AWAITING_SERVER_INFO)
					{
						// Local clients already have the map loaded by the local server.
						if (clientComponent->IsRemote)
						{
							// Set the server information.
							ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
							assert(serverInfoEntity != nullptr);

							Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);
							assert(serverInfo != nullptr);

							serverInfo->Information.Serialize(false, p_bs);

							// Set the rules
							ECS::Entity* ruleEntity = m_world->GetTagManager()->GetEntityByTag("MatchState");
							assert(ruleEntity != nullptr);

							RootForce::TDMRuleSet* rules = m_world->GetEntityManager()->GetComponent<RootForce::TDMRuleSet>(ruleEntity);
							assert(rules != nullptr);

							rules->ScoreLimit = (int) serverInfo->Information.KillCount;
							rules->TimeLeft = (float) serverInfo->Information.MatchTimeSeconds;

							// Load the map
							m_worldSystem->LoadWorld(serverInfo->Information.MapName.C_String());
							m_abilitySpawnSystem->LoadAbilities(serverInfo->Information.AbilityPack.C_String());
							m_abilitySpawnSystem->AttachComponentToPoints();
						}

						// Send load map status
						NetworkMessage::LoadMapStatus m;
						m.Status = NetworkMessage::LoadMapStatus::STATUS_COMPLETED;
					
						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) ID_TIMESTAMP);
						bs.Write(RakNet::GetTime());
						bs.Write((RakNet::MessageID) NetworkMessage::MessageType::LoadMapStatus);
						m.Serialize(true, &bs);

						m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peer->GetSystemAddressFromIndex(0), false);

						// Set the new client state
						clientComponent->State = ClientState::AWAITING_USER_CONNECT;
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "ServerInformation received in an invalid state (%d).", clientComponent->State);
					}
				} return true;
				case NetworkMessage::MessageType::TimeUp:
				{
					NetworkMessage::TimeUp m;
					m.Serialize(false, p_bs);

					ECS::Entity* entity = FindEntity(g_networkEntityMap, m.ID);
					if(entity != nullptr)
					{
						TimerComponent* timer = m_world->GetEntityManager()->GetComponent<TimerComponent>(entity);
						if(timer != nullptr)
							timer->TimeUp = true;

					}

				} return true;

				case NetworkMessage::MessageType::AbilitySpawn:
				{
					NetworkMessage::AbilitySpawn m;
					m.Serialize(false, p_bs);

					ECS::Entity* point = FindEntity(g_networkEntityMap, m.ID);

					// Entity needs to exist.
					if (point)
					{
						RootForce::AbilitySpawnComponent* component = m_world->GetEntityManager()->GetComponent<RootForce::AbilitySpawnComponent>(point);
						assert(component);
						component->AbilityReceived = m.AbilityName;
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "AbilitySpawn message received but no spawn point exists.");
					}

				} return true;

				case NetworkMessage::MessageType::Death:
				{
					NetworkMessage::Death m;
					m.Serialize(false, p_bs);

					ECS::Entity* player = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
					if(player)
					{
						RootForce::HealthComponent* health = m_world->GetEntityManager()->GetComponent<RootForce::HealthComponent>(player);
						assert(health);
						health->Health = 0;
						health->IsDead = true;
						health->RespawnDelay = 3.0f;
						health->LastDamageSourceID = m.LastDamageSource;
						health->LastDamageAbilityName = m.LastDamageSourceName;
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::PINK_PRINT, "Received death message.");
					}

				} return true;
				case NetworkMessage::MessageType::PlayerTeamSelect:
				{
					NetworkMessage::PlayerTeamSelect m;
					m.Serialize(false, p_bs);
					
					// Make sure we are in the correct state.
					if(clientComponent->IsRemote && clientComponent->State == ClientState::CONNECTED)
					{

						ECS::Entity* player = FindEntity(g_networkEntityMap, m.UserID);

						// Call the OnCreate script
						if(player)
						{
							g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnTeamSelect");
							g_engineContext.m_script->AddParameterUserData(player, sizeof(ECS::Entity*), "Entity");
							g_engineContext.m_script->AddParameterNumber(m.TeamID);
							g_engineContext.m_script->ExecuteScript();
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::WARNING, "PlayerTeamSelect received in an invalid state (%d).", clientComponent->State);
					}


				} return true;
				case NetworkMessage::MessageType::PlayerNameChange:
					{
						NetworkMessage::PlayerNameChange m;
						m.Serialize(false, p_bs);

						// Make sure we are in the correct state.
						if(clientComponent->IsRemote && clientComponent->State == ClientState::CONNECTED)
						{
							ECS::Entity* player = FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(m.UserID, RootForce::Network::ReservedActionID::CONNECT, RootForce::Network::SEQUENCE_PLAYER_ENTITY));
							PlayerComponent* playerComp = m_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
							playerComp->Name = m.Name.C_String();
						}
					} return true;
			}

			return false;
		}



		ServerMessageHandler::ServerMessageHandler(RakNet::RakPeerInterface* p_peer, ECS::World* p_world)
			: MessageHandler(p_peer)
			, m_world(p_world) {}

		bool ServerMessageHandler::ParsePacket(RakNet::MessageID p_id, RakNet::BitStream* p_bs, RakNet::Packet* p_packet, RakNet::Time p_timestamp)
		{
			// Get the server information.
			ECS::Entity* serverInfoEntity = m_world->GetTagManager()->GetEntityByTag("ServerInformation");
			assert(serverInfoEntity != nullptr);

			Network::ServerInformationComponent* serverInfo = m_world->GetEntityManager()->GetComponent<Network::ServerInformationComponent>(serverInfoEntity);
			assert(serverInfo != nullptr);

			// Calculate the time it took for the message to get to us.
			float halfPing = float(RakNet::GetTime() - p_timestamp) * 0.001f;

			// Parse the different types of messages.
			switch (p_id)
			{
				case ID_NEW_INCOMING_CONNECTION:
				{
					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::SUCCESS, "Client connected (%d: %s)", m_peer->GetIndexFromSystemAddress(p_packet->systemAddress), p_packet->systemAddress.ToString());

					// Remove any entities associated with a previous player holding this ID.
					int systemIndex = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					assert(systemIndex != -1);
					Network::DeleteEntities(g_networkEntityMap, NetworkEntityID(systemIndex, ReservedActionID::ALL, ReservedSequenceID::ALL), m_world->GetEntityManager());
					Network::NetworkComponent::ResetSequenceForUser(systemIndex);

					// Create a new client entity for the connectee.
					ECS::Entity* clientEntity = m_world->GetEntityManager()->CreateEntity();
					ClientComponent* clientComponent = m_world->GetEntityManager()->CreateComponent<ClientComponent>(clientEntity);
					clientComponent->State = ClientState::SERVER_AWAITING_USER_INFO;
					clientComponent->IsRemote = !p_packet->systemAddress.IsLoopback(); // TODO: Find a better way to know if a client is in the same application or not.

					g_networkEntityMap[NetworkEntityID((UserID_t)systemIndex, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY)] = clientEntity;
				} return true;

				case ID_DISCONNECTION_NOTIFICATION:
				case ID_CONNECTION_LOST:
				{
					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client disconnected (%s)", p_packet->systemAddress.ToString());

					int systemIndex = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					if (systemIndex != -1)
					{
						// Remove all entities associated with that player
						Network::DeleteEntities(g_networkEntityMap, NetworkEntityID(systemIndex, ReservedActionID::ALL, ReservedSequenceID::ALL), m_world->GetEntityManager());
						Network::NetworkComponent::ResetSequenceForUser(systemIndex);

						// Notify clients of disconnected client
						NetworkMessage::UserDisconnected m;
						m.User = systemIndex;
					
						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) ID_TIMESTAMP);
						bs.Write(RakNet::GetTime());
						bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserDisconnected);
						m.Serialize(true, &bs);

						// Send only to clients who have received user connected for all other clients.
						DataStructures::List<RakNet::SystemAddress> addresses;
						DataStructures::List<RakNet::RakNetGUID> guids;
						m_peer->GetSystemList(addresses, guids);
						
						for (unsigned int i = 0; i < addresses.Size(); ++i)
						{
							ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
							if (clientEntity != nullptr)
							{
								ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
								assert(clientComponent != nullptr);

								// Just send to connected clients. This means after we have received map load status successful and have sent a user connect message for everyone.
								if (ClientState::IsConnected(clientComponent->State))
								{
									m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
								}
							}
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Disconnected system has already been removed. Cannot remove entities.");
					}
				} return true;

				case ID_UNCONNECTED_PING:
				{
					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "A LAN-discovery request has been received from %s.", p_packet->systemAddress.ToString());
				} return true;

				case NetworkMessage::MessageType::Chat:
				{
					NetworkMessage::Chat m;
					m.Serialize(false, p_bs);

					g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Chat from client (%s): %s", p_packet->systemAddress.ToString(), m.Message.C_String());

					// Forward the message to broadcast
					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID) ID_TIMESTAMP);
					bs.Write(RakNet::GetTime());
					bs.Write((RakNet::MessageID) NetworkMessage::MessageType::Chat);
					
					m.Sender = (UserID_t) m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
					m.Serialize(true, &bs);

					// Send to only connected clients.
					DataStructures::List<RakNet::SystemAddress> addresses;
					DataStructures::List<RakNet::RakNetGUID> guids;
					m_peer->GetSystemList(addresses, guids);
						
					for (unsigned int i = 0; i < addresses.Size(); ++i)
					{
						ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
						if (clientEntity != nullptr)
						{
							ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
							assert(clientComponent != nullptr);

							if (ClientState::IsConnected(clientComponent->State))
							{
								m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
							}
						}
					}
				} return true;

				case NetworkMessage::MessageType::UserInformation:
				{
					NetworkMessage::UserInformation m;
					m.Serialize(false, p_bs);

					// Make sure we have a client entity associated with the peer.
					ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(p_packet->systemAddress), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
					if (clientEntity != nullptr)
					{
						// Make sure the client is in the correct state.
						ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
						assert(clientComponent != nullptr);

						if (clientComponent->State == ClientState::SERVER_AWAITING_USER_INFO)
						{
							// Update the client entity.
							clientComponent->Name = m.Name.C_String();

							// Send server information
							RakNet::BitStream bs;
							bs.Write((RakNet::MessageID) ID_TIMESTAMP);
							bs.Write(RakNet::GetTime());
							bs.Write((RakNet::MessageID) NetworkMessage::MessageType::ServerInformation);
							serverInfo->Information.Serialize(true, &bs);

							m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);

							// Set state for user, awaiting user connect.
							clientComponent->State = ClientState::SERVER_AWAITING_MAP_LOAD_STATUS;
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "User information received in invalid state (%d)", clientComponent->State);
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "No client entity associated with peer (%d: %s)", m_peer->GetIndexFromSystemAddress(p_packet->systemAddress), p_packet->systemAddress.ToString());
					}
				} return true;

				case NetworkMessage::MessageType::PlayerCommand:
				{
					NetworkMessage::PlayerCommand m;
					m.Serialize(false, p_bs);
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);

					// Make sure we have a client entity associated with the peer.
					ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
					if (clientEntity != nullptr)
					{
						// Make sure the client is connected before parsing user commands.
						ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
						assert(clientComponent != nullptr);

						ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
						assert(playerEntity != nullptr);

						PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
						assert(playerComponent != nullptr);

						if (ClientState::IsConnected(clientComponent->State) && playerComponent->TeamID != 0)
						{

							ECS::Entity* aimingEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_AIMING_DEVICE_ENTITY));
							assert(aimingEntity != nullptr);

							PlayerPhysics* playerPhysics = m_world->GetEntityManager()->GetComponent<PlayerPhysics>(playerEntity);
							assert(playerPhysics != nullptr);

							Transform* playerTransform = m_world->GetEntityManager()->GetComponent<Transform>(playerEntity);
							assert(playerTransform != nullptr);
							
							Transform* aimingTransform = m_world->GetEntityManager()->GetComponent<Transform>(aimingEntity);
							assert(aimingTransform != nullptr);

							// If this client is remote, update the action component and extrapolate. Local clients update their own entity.
							// TODO: See if we can update local client on server instead...
							// TODO: Let the extrapolation be done by the action system and physics instead.
							if (clientComponent->IsRemote)
							{
								// Update the action for the user (preserve ActiveAbility).
								PlayerActionComponent* playerAction = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								assert(playerAction != nullptr);
								
								playerAction->MovePower = m.MovePower;
								playerAction->StrafePower = m.StrafePower;
								playerAction->Angle = m.Angle;
								playerAction->JumpTime = m.JumpTime;
								playerAction->SelectedAbility = m.SelectedAbility;

								// Set the position of the player
								Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(playerEntity);
								assert(collision != nullptr);
								float verticalVelocity = g_engineContext.m_physics->GetPlayerVerticalVelocity(*collision->m_handle);
								glm::vec3 displacement = m.Position - playerTransform->m_position;
								
								// Check if the displacement has the same sign as the local velocity.
								//if (displacement.y * verticalVelocity >= 0.0f)
									playerTransform->m_position.y = m.Position.y;
								playerTransform->m_position.x = m.Position.x;
								playerTransform->m_position.z = m.Position.z;
								playerTransform->m_orientation.SetOrientation(m.Orientation);
								aimingTransform->m_orientation.SetOrientation(m.AimingDeviceOrientation);

								// Extrapolate the position using ping time
								/*
								glm::vec3 facing = playerTransform->m_orientation.GetFront();
								glm::vec3 right = playerTransform->m_orientation.GetRight();
								glm::vec3 movement = facing * playerAction->MovePower + right * playerAction->StrafePower;
								if (movement != glm::vec3(0))
									movement = glm::normalize(movement) * playerPhysics->MovementSpeed * halfPing;

								playerTransform->m_position += movement;
								*/
							}

							// Broadcast the action to all other connected clients.
							DataStructures::List<RakNet::SystemAddress> addresses;
							DataStructures::List<RakNet::RakNetGUID> guids;
							m_peer->GetSystemList(addresses, guids);

							m.Position = playerTransform->m_position;
							m.Orientation = playerTransform->m_orientation.GetQuaternion();
							m.AimingDeviceOrientation = aimingTransform->m_orientation.GetQuaternion();

							for (unsigned int i = 0; i < addresses.Size(); ++i)
							{
								ECS::Entity* otherClientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
								if (otherClientEntity != nullptr)
								{
									ClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(otherClientEntity);
									assert(otherClientComponent != nullptr);

									if (p_packet->systemAddress != addresses[i] && 
										otherClientComponent->IsRemote &&
										ClientState::IsConnected(otherClientComponent->State))
									{
										RakNet::BitStream bs;
										bs.Write((RakNet::MessageID) ID_TIMESTAMP);
										bs.Write(RakNet::GetTime());
										bs.Write((RakNet::MessageID) NetworkMessage::MessageType::PlayerCommand);
										m.Serialize(true, &bs);

										m_peer->Send(&bs, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, 0, addresses[i], false);
									}
								}
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Player command received from connecting client. Should be connected before sending player commands.");
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Player command received from unconnected client without client entity.");
					}
				} return true;

				case NetworkMessage::MessageType::JumpStart:
				{
					NetworkMessage::JumpStart m;
					m.Serialize(false, p_bs);
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);

					// Make sure we have a client entity associated with this peer.
					ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
					if (clientEntity != nullptr)
					{
						// Local clients will handle their own user commands. Only acknowledge this if it is a remote client and make sure it is connected.
						ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
						assert(clientComponent != nullptr);

						if (ClientState::IsConnected(clientComponent->State))
						{
							// Update the action component only for remote clients.
							if (clientComponent->IsRemote)
							{
								ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
								assert(playerEntity != nullptr);

								PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								assert(action != nullptr);

								action->JumpTime = halfPing;
							}

							// Broadcast the action to all other connected clients.
							DataStructures::List<RakNet::SystemAddress> addresses;
							DataStructures::List<RakNet::RakNetGUID> guids;
							m_peer->GetSystemList(addresses, guids);

							for (unsigned int i = 0; i < addresses.Size(); ++i)
							{
								ECS::Entity* otherClientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
								if (otherClientEntity != nullptr)
								{
									ClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(otherClientEntity);
									assert(otherClientComponent != nullptr);

									if (p_packet->systemAddress != addresses[i] && 
										otherClientComponent->IsRemote &&
										ClientState::IsConnected(otherClientComponent->State))
									{
										RakNet::BitStream bs;
										bs.Write((RakNet::MessageID) ID_TIMESTAMP);
										bs.Write(RakNet::GetTime());
										bs.Write((RakNet::MessageID) NetworkMessage::MessageType::JumpStart);
										m.Serialize(true, &bs);

										m_peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
									}
								}
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "JumpStart received for client in invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "JumpStart received for client without client entity. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
					}
				} return true;

				case NetworkMessage::MessageType::JumpStop:
				{
					NetworkMessage::JumpStop m;
					m.Serialize(false, p_bs);
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);

					// Make sure we have a client entity associated with this peer.
					ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
					if (clientEntity != nullptr)
					{
						// Local clients will handle their own user commands. Only acknowledge this if it is a remote client and make sure it is connected.
						ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
						assert(clientComponent != nullptr);

						if (ClientState::IsConnected(clientComponent->State))
						{
							// Update the action component only for remote clients.
							if (clientComponent->IsRemote)
							{
								ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
								assert(playerEntity != nullptr);

								PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								assert(action != nullptr);

								action->JumpTime = m.Time;
							}

							// Broadcast the action to all other connected clients.
							DataStructures::List<RakNet::SystemAddress> addresses;
							DataStructures::List<RakNet::RakNetGUID> guids;
							m_peer->GetSystemList(addresses, guids);

							for (unsigned int i = 0; i < addresses.Size(); ++i)
							{
								ECS::Entity* otherClientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
								if (otherClientEntity != nullptr)
								{
									ClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(otherClientEntity);
									assert(otherClientComponent != nullptr);

									if (p_packet->systemAddress != addresses[i] && 
										otherClientComponent->IsRemote &&
										ClientState::IsConnected(otherClientComponent->State))
									{
										RakNet::BitStream bs;
										bs.Write((RakNet::MessageID) ID_TIMESTAMP);
										bs.Write(RakNet::GetTime());
										bs.Write((RakNet::MessageID) NetworkMessage::MessageType::JumpStop);
										m.Serialize(true, &bs);

										m_peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
									}
								}
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "JumpStop received for client in invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "JumpStop received for client without client entity. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
					}
				} return true;

				case NetworkMessage::MessageType::AbilityEvent:
				{
					NetworkMessage::AbilityEvent m;
					m.Serialize(false, p_bs);
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);

					// Make sure we have a client entity associated with this peer.
					ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
					if (clientEntity != nullptr)
					{
						// Local clients will handle their own user commands. Only acknowledge this if it is a remote client and make sure it is connected.
						ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
						assert(clientComponent != nullptr);

						if (ClientState::IsConnected(clientComponent->State))
						{
							// Update the action component only for remote clients.
							if (clientComponent->IsRemote)
							{
								ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
								assert(playerEntity != nullptr);

								PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								assert(action != nullptr);

								// Push the event.
								action->AbilityEvents.push(m.Event);
							}

							// Forward the event to all the other clients.
							DataStructures::List<RakNet::SystemAddress> addresses;
							DataStructures::List<RakNet::RakNetGUID> guids;
							m_peer->GetSystemList(addresses, guids);

							RakNet::BitStream bs;
							bs.Write((RakNet::MessageID) ID_TIMESTAMP);
							bs.Write(RakNet::GetTime());
							bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityEvent);
							m.Serialize(true, &bs);

							for (unsigned int i = 0; i < addresses.Size(); ++i)
							{
								ECS::Entity* otherClientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
								if (otherClientEntity != nullptr)
								{
									ClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(otherClientEntity);
									assert(otherClientComponent != nullptr);

									if (p_packet->systemAddress != addresses[i] && 
										otherClientComponent->IsRemote &&
										ClientState::IsConnected(otherClientComponent->State))
									{
										m_peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
									}
								}
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "AbilityEvent received for client in invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "AbilityEvent received for client without client entity. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
					}
				} return true;

				case NetworkMessage::MessageType::AbilityTryClaim:
				{
					NetworkMessage::AbilityTryClaim m;
					m.Serialize(false, p_bs);

					ECS::Entity* player = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
					if (player)
					{
						TryPickupComponent* tryPickup = m_world->GetEntityManager()->GetComponent<TryPickupComponent>(player);
						assert(tryPickup);
						tryPickup->TryPickup = true;
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "AbilityTryClaim received but the player entity doesn't exist.");
					}
				} return true;

				case NetworkMessage::MessageType::RespawnRequest:
				{
					NetworkMessage::RespawnRequest m;
					m.Serialize(false, p_bs);
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);

					// Make sure this client has a client entity.
					ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
					if (clientEntity != nullptr)
					{
						// Make sure the client is a remote player. A local player will already have updated its health component and have a respawn system with a server peer.
						ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
						assert(clientComponent != nullptr);

						if (clientComponent->IsRemote)
						{
							// Make sure the client is connected.
							if (ClientState::IsConnected(clientComponent->State))
							{
								ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
								assert(playerEntity != nullptr);

								PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(playerEntity);
								if(action != nullptr)
									action->WantRespawn = true;
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "RespawnRequest received for client in invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "RespawnRequest received for local client.");
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "RespawnRequest received for client without client entity. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
					}
				} return true;

				case NetworkMessage::MessageType::Suicide:
				{
					NetworkMessage::Suicide m;
					m.Serialize(false, p_bs);
					m.User = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);

					// Make sure this client has a client entity.
					ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
					if (clientEntity != nullptr)
					{
						// Make sure the client is a remote player. A local player will already have updated its health component and have a respawn system with a server peer.
						ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
						assert(clientComponent != nullptr);

						// Make sure the client is connected.
						if (ClientState::IsConnected(clientComponent->State))
						{
							// Kill the player only for remote clients.
							if (clientComponent->IsRemote)
							{
								ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m.User, ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
								assert(playerEntity != nullptr);

								HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(playerEntity);
								PlayerComponent* playerComp = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
								TDMRuleSet* rules = m_world->GetEntityManager()->GetComponent<TDMRuleSet>(m_world->GetTagManager()->GetEntityByTag("MatchState"));
								assert(health != nullptr);
								assert(playerComp != nullptr);
								assert(rules != nullptr);

								health->Health = 0;
								playerComp->Score --;
								playerComp->Deaths ++;
								rules->TeamScore[playerComp->TeamID] --;
							}

							// Broadcast the suicide to all other connected clients.
							DataStructures::List<RakNet::SystemAddress> addresses;
							DataStructures::List<RakNet::RakNetGUID> guids;
							m_peer->GetSystemList(addresses, guids);

							for (unsigned int i = 0; i < addresses.Size(); ++i)
							{
								ECS::Entity* otherClientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
								if (otherClientEntity != nullptr)
								{
									ClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(otherClientEntity);
									assert(otherClientComponent != nullptr);

									if (addresses[i] != p_packet->systemAddress &&
										otherClientComponent->IsRemote &&
										ClientState::IsConnected(otherClientComponent->State))
									{
										RakNet::BitStream bs;
										bs.Write((RakNet::MessageID) ID_TIMESTAMP);
										bs.Write(RakNet::GetTime());
										bs.Write((RakNet::MessageID) NetworkMessage::MessageType::Suicide);
										m.Serialize(true, &bs);

										m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
									}
								}
							}
						}
						else
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Suicide received for client in invalid state (%d). User (%d: %s).", clientComponent->State, m.User, p_packet->systemAddress.ToString());
						}
					}
					else
					{
						g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Suicide received for client without client entity. User (%d: %s).", m.User, p_packet->systemAddress.ToString());
					}
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
							// Make sure the peer has a client entity.
							UserID_t userID = m_peer->GetIndexFromSystemAddress(p_packet->systemAddress);
							ECS::Entity* clientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(userID, ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
							if (clientEntity != nullptr)
							{
								// Make sure the client is in the correct state.
								ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(clientEntity);
								assert(clientComponent != nullptr);

								if (clientComponent->State == ClientState::SERVER_AWAITING_MAP_LOAD_STATUS)
								{
									g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::DEBUG_PRINT, "Client (%s) successfully loaded map", p_packet->systemAddress.ToString());

									// If this client is a local client, set the UserID global first.
									if (!clientComponent->IsRemote)
									{
										g_engineContext.m_script->SetGlobalNumber("UserID", m_peer->GetIndexFromSystemAddress(p_packet->systemAddress));
									}

									// Create the player.
									g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
									g_engineContext.m_script->AddParameterNumber(userID);
									g_engineContext.m_script->AddParameterNumber(Network::ReservedActionID::CONNECT);
									g_engineContext.m_script->ExecuteScript();

									// Get the newly created player entity.
									ECS::Entity* playerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(userID, Network::ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
									assert(playerEntity != nullptr);

									PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(playerEntity);
									assert(playerComponent != nullptr);
									playerComponent->Name = clientComponent->Name;

									// Send a user connected message to all clients except connectee.
									DataStructures::List<RakNet::SystemAddress> addresses;
									DataStructures::List<RakNet::RakNetGUID> guids;
									m_peer->GetSystemList(addresses, guids);

									for (unsigned int i = 0; i < addresses.Size(); ++i)
									{
										if (addresses[i] != p_packet->systemAddress)
										{
											NetworkMessage::UserConnected n;
											n.User = userID;
											n.IsYou = false;
											n.Name = RakNet::RakString(playerComponent->Name.c_str());
											n.TeamID = 0;

											ECS::Entity* otherClientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
											if (otherClientEntity != nullptr)
											{
												ClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(otherClientEntity);
												assert(otherClientComponent != nullptr);

												if (ClientState::IsConnected(otherClientComponent->State) || otherClientComponent->State == ClientState::AWAITING_SPAWN_POINT)
												{
													RakNet::BitStream bs;
													bs.Write((RakNet::MessageID) ID_TIMESTAMP);
													bs.Write(RakNet::GetTime());
													bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserConnected);
													n.Serialize(true, &bs);

													m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addresses[i], false);
												}
											}
										}
									}

									// Send a user connected message for all connected clients to the connectee.
									for (unsigned int i = 0; i < addresses.Size(); ++i)
									{
										ECS::Entity* otherClientEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, ReservedSequenceID::CLIENT_ENTITY));
										ECS::Entity* otherPlayerEntity = FindEntity(g_networkEntityMap, NetworkEntityID(m_peer->GetIndexFromSystemAddress(addresses[i]), ReservedActionID::CONNECT, SEQUENCE_PLAYER_ENTITY));
										if (otherClientEntity != nullptr && otherPlayerEntity != nullptr)
										{
											ClientComponent* otherClientComponent = m_world->GetEntityManager()->GetComponent<ClientComponent>(otherClientEntity);
											assert(otherClientComponent != nullptr);

											// Make sure the client is either connected or is the client in question who is connecting.
											if (ClientState::IsConnected(otherClientComponent->State) || 
												otherClientComponent->State == ClientState::AWAITING_SPAWN_POINT ||
												addresses[i] == p_packet->systemAddress)
											{
												PlayerComponent* otherPlayerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(otherPlayerEntity);
												assert(otherPlayerComponent != nullptr);

												NetworkMessage::UserConnected n;
												n.User = m_peer->GetIndexFromSystemAddress(addresses[i]);
												n.IsYou = addresses[i] == p_packet->systemAddress;
												n.Name = RakNet::RakString(otherPlayerComponent->Name.c_str());
												n.TeamID = otherPlayerComponent->TeamID;

												RakNet::BitStream bs;
												bs.Write((RakNet::MessageID) ID_TIMESTAMP);
												bs.Write(RakNet::GetTime());
												bs.Write((RakNet::MessageID) NetworkMessage::MessageType::UserConnected);
												n.Serialize(true, &bs);

												m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);
											}
										}
									}

									// Send a game state snapshot to the connectee
									{
										RakNet::BitStream bs;
										bs.Write((RakNet::MessageID) ID_TIMESTAMP);
										bs.Write(RakNet::GetTime());
										bs.Write((RakNet::MessageID) NetworkMessage::MessageType::GameStateDelta);
										NetworkMessage::SerializeWorld(&bs, m_world, g_networkEntityMap);

										m_peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p_packet->systemAddress, false);
									}

									// Make sure the player will be spawned at a spawn point.
									/*HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(playerEntity);
									assert(health != nullptr);
									health->WantsRespawn = true;
									health->RespawnDelay = 0.0f;*/ //TODO: This must happen when selecting team instead!

									clientComponent->State = ClientState::CONNECTED;
								}
								else
								{
									g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "LoadMapStatus received for client in invalid state (%d). User (%d: %s).", clientComponent->State, m_peer->GetIndexFromSystemAddress(p_packet->systemAddress), p_packet->systemAddress.ToString());
								}
							}
							else
							{
								g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "LoadMapStatus received for client without client entity. User (%d: %s).", m_peer->GetIndexFromSystemAddress(p_packet->systemAddress), p_packet->systemAddress.ToString());
							}
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_FAILED_MAP_NOT_FOUND:
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client (%s) failed to load map: Map not found", p_packet->systemAddress.ToString());
							m_peer->CloseConnection(p_packet->systemAddress, true);
						} break;

						case NetworkMessage::LoadMapStatus::STATUS_FAILED_INVALID_HASH:
						{
							g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client (%s) failed to load map: Invalid map hash", p_packet->systemAddress.ToString());
							m_peer->CloseConnection(p_packet->systemAddress, true);
						} break;
					}
				} return true;
				case NetworkMessage::MessageType::PlayerTeamSelect:
					{
						NetworkMessage::PlayerTeamSelect m;
						m.Serialize(false, p_bs);

						//Only do these calculations if we want to join a playing team, not the spectators
						if(m.TeamID == 1 || m.TeamID == 2)
						{
							//Save down the number of players in each team
							int team1, team2;
							team1 = team2 = 0;
							for(auto pair : g_networkEntityMap)
							{
								if(pair.first.ActionID != Network::ReservedActionID::CONNECT || pair.first.SequenceID != RootForce::Network::SEQUENCE_PLAYER_ENTITY || !pair.second)
									continue;

								RootForce::PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<RootForce::PlayerComponent>(pair.second);
								if(playerComponent->TeamID == 1)
									team1 ++;
								else if(playerComponent->TeamID == 2)
									team2 ++;
							}

							//if a team has two or more players than the other team, joining it is not allowed
							if(m.TeamID == 1) 
								if(team1 > team2)
								{
									g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client tried to join a full team 1: %d vs %d", team1, team2);
									break;
								}
							else if(m.TeamID == 2)
								if(team2 > team1)
								{
									g_engineContext.m_logger->LogText(LogTag::SERVER, LogLevel::WARNING, "Client tried to join a full team 2: %d vs %d", team1, team2);
									break;
								}
						}

						ECS::Entity* player = FindEntity(g_networkEntityMap, m.UserID);

						// Call the OnCreate script
						g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->GetScript("Player"), "OnTeamSelect");
						g_engineContext.m_script->AddParameterUserData(player, sizeof(ECS::Entity*), "Entity");
						g_engineContext.m_script->AddParameterNumber(m.TeamID);
						g_engineContext.m_script->ExecuteScript();

						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) ID_TIMESTAMP);
						bs.Write(RakNet::GetTime());
						bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::PlayerTeamSelect);
						m.Serialize(true, &bs);

						m_peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
					} return true;
				case NetworkMessage::MessageType::PlayerNameChange:
					{
						NetworkMessage::PlayerNameChange m;
						m.Serialize(false, p_bs);

						ECS::Entity* player = FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(m.UserID, RootForce::Network::ReservedActionID::CONNECT, RootForce::Network::SEQUENCE_PLAYER_ENTITY));
						ECS::Entity* client = FindEntity(g_networkEntityMap, RootForce::Network::NetworkEntityID(m.UserID, RootForce::Network::ReservedActionID::CONNECT, RootForce::Network::ReservedSequenceID::CLIENT_ENTITY));
						PlayerComponent* playerComp = m_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
						ClientComponent* clientComp = m_world->GetEntityManager()->GetComponent<ClientComponent>(client);
						if(playerComp != nullptr && clientComp != nullptr)
						{
							playerComp->Name = m.Name.C_String();
							clientComp->Name = m.Name;
						}

						
					} return true;
			}
			
			return false;
		}
	}
}

#endif
