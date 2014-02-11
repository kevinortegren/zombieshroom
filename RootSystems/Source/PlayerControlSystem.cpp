#ifndef COMPILE_LEVEL_EDITOR

#include <RakNet/GetTime.h>
#include <PlayerControlSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootSystems/Include/Network/NetworkComponents.h>
#include <RootEngine/Script/Include/RootScript.h>
extern RootEngine::GameSharedContext g_engineContext;
extern RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootForce
{
	Network::ActionID_t PlayerControlSystem::s_nextActionID = 0;

	Keybinding::Keybinding()
		: Action(PlayerAction::NONE), Edge(false)
	{}

	Keybinding::Keybinding(SDL_Scancode binding, PlayerAction::PlayerAction action)
	{
		Bindings.push_back(binding);
		Action = action;
	}

	PlayerControlSystem::PlayerControlSystem(ECS::World* p_world)
		: ECS::VoidSystem(p_world) 
		, m_clientPeer(nullptr)
	{}

	void PlayerControlSystem::SetKeybindings(const std::vector<Keybinding>& keybindings)
	{
		m_keybindings = keybindings;
	}

	void PlayerControlSystem::SetLoggingInterface(Logging* p_logger)
	{
		m_logger = p_logger;
	}

	void PlayerControlSystem::SetInputInterface(RootEngine::InputManager::InputInterface* p_inputManager)
	{
		m_inputManager = p_inputManager;
	}

	void PlayerControlSystem::SetPhysicsInterface(RootEngine::Physics::PhysicsInterface* p_physics)
	{
		m_physics = p_physics;
	}

	void PlayerControlSystem::SetClientPeer(RakNet::RakPeerInterface* p_clientPeer)
	{
		m_clientPeer = p_clientPeer;
	}

	void PlayerControlSystem::Process()
	{
		// Check what keys have been pressed this frame and update the action list.
		m_inputtedActionsCurrentFrame.clear();
		for (Keybinding kb : m_keybindings)
		{
			for (SDL_Scancode sc : kb.Bindings)
			{
				if (kb.Edge)
				{
					RootEngine::InputManager::KeyState::KeyState keystate = m_inputManager->GetKeyState(sc);
					if (keystate == RootEngine::InputManager::KeyState::DOWN_EDGE)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.Action);
						break;
					}

					if (keystate == RootEngine::InputManager::KeyState::UP_EDGE)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.ActionUp);
						break;
					}
				}
				else
				{
					RootEngine::InputManager::KeyState::KeyState keystate = m_inputManager->GetKeyState(sc);
					if (keystate == RootEngine::InputManager::KeyState::DOWN)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.Action);
						break;
					}
					
					if (keystate == RootEngine::InputManager::KeyState::UP_EDGE)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.ActionUp);
						break;
					}
				}
			}
		}

		// Check if the mouse has been moved this frame.
		m_deltaMouseMovement = m_inputManager->GetDeltaMousePos();
		if (m_deltaMouseMovement != glm::ivec2(0, 0))
		{
			m_inputtedActionsCurrentFrame.push_back(PlayerAction::ORIENTATE);
		}

		// TODO: Check for selection of abilities (whether the keypad has been pressed or mouse wheel been scrolled?)


		// Get the properties we need.
		float dt = m_world->GetDelta();


		ECS::Entity* entity = m_world->GetTagManager()->GetEntityByTag("Player");
		ECS::Entity* aimingDevice = m_world->GetTagManager()->GetEntityByTag("AimingDevice");

		PlayerControl* controller = m_world->GetEntityManager()->GetComponent<PlayerControl>(entity);
		PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(entity);
		PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(entity);
		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);
		Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<Transform>(aimingDevice);
		Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(entity);
		Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(entity);

		glm::vec3 movement(0.0f);

		//action->ActionID = Network::ReservedActionID::NONE;
		action->MovePower = 0;
		action->StrafePower = 0;
		for (PlayerAction::PlayerAction currentAction : m_inputtedActionsCurrentFrame)
		{
			switch (currentAction)
			{
			case PlayerAction::MOVE_FORWARDS:
			case PlayerAction::MOVE_BACKWARDS_STOP:
					action->MovePower += 1;
				break;
			case PlayerAction::MOVE_BACKWARDS:
			case PlayerAction::MOVE_FORWARDS_STOP:
					action->MovePower -= 1;
				break;
			case PlayerAction::STRAFE_RIGHT:
			case PlayerAction::STRAFE_LEFT_STOP:
					action->StrafePower += 1;
				break;
			case PlayerAction::STRAFE_LEFT:
			case PlayerAction::STRAFE_RIGHT_STOP:
					action->StrafePower -= 1;
				break;
			case PlayerAction::ORIENTATE:
				{
					//m_physics->SetPlayerOrientation(playerID, orientation);
					//m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Reorienting: Delta (%d, %d)", m_deltaMouseMovement.x, m_deltaMouseMovement.y);
					// TODO: Update a camera controller with m_deltaMouseMovement.
					//transform->m_orientation.Pitch(m_deltaMouseMovement.y * controller->m_mouseSensitivity);
					//transform->m_orientation.YawGlobal(-m_deltaMouseMovement.x * controller->m_mouseSensitivity);
					action->Angle.x = -m_deltaMouseMovement.x * controller->m_mouseSensitivity;
					action->Angle.y += m_deltaMouseMovement.y * controller->m_mouseSensitivity;

					if(action->Angle.y < -90.0f)
						action->Angle.y = -90;
					else if(action->Angle.y > 90.0f)
						action->Angle.y = 90;
				}
				break;
			case PlayerAction::SELECT_ABILITY1:
				action->SelectedAbility = 1;
				break;
			case PlayerAction::SELECT_ABILITY2:
				action->SelectedAbility = 2;
				break;
			case PlayerAction::SELECT_ABILITY3:
				action->SelectedAbility = 3;
				break;
			case PlayerAction::ACTIVATE_ABILITY_PRESSED:
				{
					if (playerComponent->AbilityState == AbilityState::OFF && !playerComponent->AbilityScripts[playerComponent->SelectedAbility].OnCooldown)
					{
						playerComponent->AbilityState = AbilityState::START_CHARGING;
						action->ActionID = s_nextActionID++;

						if (m_clientPeer != nullptr)
						{
							// Send this action to the server
							RootForce::NetworkMessage::AbilityChargeStart m;
							m.User = network->ID.UserID;
							m.Action = action->ActionID;

							RakNet::BitStream bs;
							bs.Write((RakNet::MessageID) ID_TIMESTAMP);
							bs.Write(RakNet::GetTime());
							bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilityChargeStart);
							m.Serialize(true, &bs);

							m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
						}
					}

					if (playerComponent->AbilityState != AbilityState::OFF)
						action->AbilityTime += dt;

					std::string abilityName = playerComponent->AbilityScripts[playerComponent->SelectedAbility].Name;
					if (abilityName != "")
					{

						float abilityChargeTime = (float) g_engineContext.m_script->GetGlobalNumber("chargeTime", abilityName);
						float abilityChannelingTime = (float) g_engineContext.m_script->GetGlobalNumber("channelingTime", abilityName);
						float abilityCooldownTime = (float) g_engineContext.m_script->GetGlobalNumber("cooldown", abilityName);

						if (action->AbilityTime >= abilityChargeTime && playerComponent->AbilityState == AbilityState::CHARGING)
						{
							playerComponent->AbilityState = AbilityState::START_CHANNELING;

							NetworkMessage::AbilityChargeDone m;
							m.User = network->ID.UserID;
							m.Action = action->ActionID;
							m.Time = action->AbilityTime;

							RakNet::BitStream bs;
							bs.Write((RakNet::MessageID) ID_TIMESTAMP);
							bs.Write(RakNet::GetTime());
							bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityChargeDone);
							m.Serialize(true, &bs);

							m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
						}

						if (action->AbilityTime >= abilityChargeTime + abilityChannelingTime && playerComponent->AbilityState == AbilityState::CHANNELING)
						{
							playerComponent->AbilityState = AbilityState::STOP_CHANNELING;

							NetworkMessage::AbilityChannelingDone m;
							m.User = network->ID.UserID;
							m.Action = action->ActionID;
							m.Time = action->AbilityTime;

							RakNet::BitStream bs;
							bs.Write((RakNet::MessageID) ID_TIMESTAMP);
							bs.Write(RakNet::GetTime());
							bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityChannelingDone);
							m.Serialize(true, &bs);

							m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
						}
					}
				}
				break;
			case PlayerAction::ACTIVATE_ABILITY_RELEASED:
				{
					if (playerComponent->AbilityState != AbilityState::OFF)
					{
						std::string abilityName = playerComponent->AbilityScripts[playerComponent->SelectedAbility].Name;
						if (abilityName != "")
						{

							float abilityChargeTime = (float) g_engineContext.m_script->GetGlobalNumber("chargeTime", abilityName);
							float abilityChannelingTime = (float) g_engineContext.m_script->GetGlobalNumber("channelingTime", abilityName);
							float abilityCooldownTime = (float) g_engineContext.m_script->GetGlobalNumber("cooldown", abilityName);

							if (playerComponent->AbilityState == AbilityState::CHARGING)
							{
								playerComponent->AbilityState = AbilityState::STOP_CHARGING_AND_CHANNELING;

								NetworkMessage::AbilityChargeAndChannelingDone m;
								m.User = network->ID.UserID;
								m.Action = action->ActionID;
								m.Time = action->AbilityTime;

								RakNet::BitStream bs;
								bs.Write((RakNet::MessageID) ID_TIMESTAMP);
								bs.Write(RakNet::GetTime());
								bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityChargeAndChannelingDone);
								m.Serialize(true, &bs);

								m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
							}
							if (playerComponent->AbilityState == AbilityState::CHANNELING)
							{
								playerComponent->AbilityState = AbilityState::STOP_CHANNELING;

								NetworkMessage::AbilityChannelingDone m;
								m.User = network->ID.UserID;
								m.Action = action->ActionID;
								m.Time = action->AbilityTime;

								RakNet::BitStream bs;
								bs.Write((RakNet::MessageID) ID_TIMESTAMP);
								bs.Write(RakNet::GetTime());
								bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityChannelingDone);
								m.Serialize(true, &bs);

								m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
							}
						}
					}
					
					//playerComponent->AbilityState = AbilityState::OFF;
				}
				break;
			case PlayerAction::JUMP_PRESSED:
				{
					if (g_engineContext.m_physics->IsOnGround(*collision->m_handle) || (action->JumpTime > 0.0f && action->JumpTime < JUMP_TIME_LIMIT))
					{
						if (action->JumpTime <= 0.0f && m_clientPeer != nullptr)
						{
							// Send this action to the server
							RootForce::NetworkMessage::JumpStart m;
							m.User = network->ID.UserID;

							RakNet::BitStream bs;
							bs.Write((RakNet::MessageID) ID_TIMESTAMP);
							bs.Write(RakNet::GetTime());
							bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::JumpStart);
							m.Serialize(true, &bs);

							m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
						}

						// Start/Keep jumping
						action->JumpTime += dt;
					}
				}
				break;
			case PlayerAction::JUMP_RELEASED:
				{
					if (m_clientPeer != nullptr)
					{
						// Send this action to the server
						RootForce::NetworkMessage::JumpStop m;
						m.User = network->ID.UserID;
						m.Time = action->JumpTime;

						RakNet::BitStream bs;
						bs.Write((RakNet::MessageID) ID_TIMESTAMP);
						bs.Write(RakNet::GetTime());
						bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::JumpStop);
						m.Serialize(true, &bs);

						m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
					}

					action->JumpTime = 0.0f;
				}
				break;
			default:
				break;
			}
			
			
		}

		
		// Update the action component only if we are a remote client
		//ECS::Entity* clientEntity = m_world->GetTagManager()->GetEntityByTag("Client");
		//Network::ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);

		// Send the action to the server.
		RootForce::NetworkMessage::PlayerCommand m;
		m.User = network->ID.UserID;
		m.Action = *action;
		m.Position = transform->m_position;
		m.Orientation = transform->m_orientation.GetQuaternion();
		m.AimingDeviceOrientation = aimingDeviceTransform->m_orientation.GetQuaternion();

		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID) ID_TIMESTAMP);
		bs.Write(RakNet::GetTime());
		bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::PlayerCommand);
		m.Serialize(true, &bs);

		if (m_clientPeer != nullptr)
			m_clientPeer->Send(&bs, HIGH_PRIORITY, UNRELIABLE, 0, m_clientPeer->GetSystemAddressFromIndex(0), false);



		m_inputtedActionsPreviousFrame = m_inputtedActionsCurrentFrame;
	}
}

#endif
