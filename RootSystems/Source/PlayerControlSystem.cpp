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
		: Action(PlayerAction::NONE), Edge(false), PressedTime(0.0f)
	{}

	Keybinding::Keybinding(SDL_Scancode binding, PlayerAction::PlayerAction action)
	{
		Bindings.push_back(binding);
		Action = action;
		PressedTime = 0.f;
		Edge = false;
	}

	PlayerControlSystem::PlayerControlSystem(ECS::World* p_world)
		: ECS::VoidSystem(p_world) 
		, m_clientPeer(nullptr)
	{
		SetDefaultKeybindings();

		for(int i = 0; i < PlayerAction::END; ++i)
		{
			m_playerActionData[i].ActiveTime = 0.0f;
		}
	}

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
						kb.PressedTime = 0.0f;
						m_playerActionData[kb.Action].ActiveTime = 0.0f;
						m_inputtedActionsCurrentFrame.push_back(kb.Action);
						break;
					}

					if (keystate == RootEngine::InputManager::KeyState::UP_EDGE)
					{
						kb.PressedTime = 0.0f;
						m_playerActionData[kb.Action].ActiveTime = 0.0f;
						m_inputtedActionsCurrentFrame.push_back(kb.ActionUp);
						break;
					}
				}
				else
				{
					RootEngine::InputManager::KeyState::KeyState keystate = m_inputManager->GetKeyState(sc);
					if (keystate == RootEngine::InputManager::KeyState::DOWN)
					{
						kb.PressedTime += m_world->GetDelta();
						m_playerActionData[kb.Action].ActiveTime += m_world->GetDelta();
						m_inputtedActionsCurrentFrame.push_back(kb.Action);
						break;
					}
					
					if (keystate == RootEngine::InputManager::KeyState::UP_EDGE)
					{
						kb.PressedTime = 0.0f;
						m_playerActionData[kb.Action].ActiveTime = 0.0f;
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
		HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(entity);
		PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(entity);
		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);
		Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<Transform>(aimingDevice);
		Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(entity);
		Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(entity);

		if(!controller || !action || !health || !playerComponent || !transform || !aimingDeviceTransform || !network || !collision)
			return;

		bool abilityUpdatedThisFrame = false;

		bool onGround = g_engineContext.m_physics->IsOnGround(*collision->m_handle);
		float forwardPower = 0;
		float strafePower = 0;
		float backPower = 0;

		action->MovePower = 0;
		action->StrafePower = 0;
		if(onGround)
		{
			forwardPower = 1;
			strafePower = 0.7f;
			backPower = 0.5f;
		}
		else
		{
			forwardPower = 0.03f;
			strafePower = 0.03f;
			backPower = 0.03f;
		}
		for (PlayerAction::PlayerAction currentAction : m_inputtedActionsCurrentFrame)
		{
			switch (currentAction)
			{
			case PlayerAction::MOVE_FORWARDS:
					action->MovePower += glm::lerp(0.0f, forwardPower, 1.0f/0.1f * glm::min(m_playerActionData[PlayerAction::MOVE_FORWARDS].ActiveTime, 0.1f));
				break;
			case PlayerAction::MOVE_BACKWARDS:
					action->MovePower += glm::lerp(0.0f, -backPower, 1.0f/0.1f * glm::min(m_playerActionData[PlayerAction::MOVE_BACKWARDS].ActiveTime, 0.1f));
				break;
			case PlayerAction::STRAFE_RIGHT:
					action->StrafePower += glm::lerp(0.0f, strafePower, 1.0f/0.1f * glm::min(m_playerActionData[PlayerAction::STRAFE_RIGHT].ActiveTime, 0.1f));;
				break;
			case PlayerAction::STRAFE_LEFT:
					action->StrafePower += glm::lerp(0.0f, -strafePower, 1.0f/0.1f * glm::min(m_playerActionData[PlayerAction::STRAFE_LEFT].ActiveTime, 0.1f));
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
			{
				//HandleAbilityReleased();
				action->SelectedAbility = 0;
			} break;

			case PlayerAction::SELECT_ABILITY2:
			{
				//HandleAbilityReleased();
				action->SelectedAbility = 1;
			} break;

			case PlayerAction::SELECT_ABILITY3:
			{
				//HandleAbilityReleased();
				action->SelectedAbility = 2;
			} break;
			
			case PlayerAction::SCROLL_ABILITY_FORWARD:
			case PlayerAction::SWITCH_ABILITY_FORWARD:
			{
				action->SelectedAbility = (action->SelectedAbility + 1) % 3;
			} break;
			case PlayerAction::SCROLL_ABILITY_BACKWARD:
			{
				action->SelectedAbility = (action->SelectedAbility + 2) % 3;
			} break;

			case PlayerAction::ACTIVATE_ABILITY_PRESSED:
			{
				if (health->IsDead)
				{
					action->WantRespawn = true;
				}
				else
				{
					if (!abilityUpdatedThisFrame)
					{
						HandleAbilityPressed(dt, false);
						abilityUpdatedThisFrame = true;
					}
				}
			}
			break;

			case PlayerAction::ACTIVATE_ABILITY_RELEASED:
			{
				HandleAbilityReleased();
			}
			break;

			case PlayerAction::ACTIVATE_PUSH_ABILITY_PRESSED:
			{
				if (health->IsDead)
				{
					action->WantRespawn = true;
				}
				else
				{
					if (!abilityUpdatedThisFrame)
					{
						HandleAbilityPressed(dt, true);
						abilityUpdatedThisFrame = true;
					}
				}
			}
			break;

			case PlayerAction::ACTIVATE_PUSH_ABILITY_RELEASED:
			{
				HandleAbilityReleased();
			}
			break;

			case PlayerAction::PICK_UP_ABILITY:
			{
				RootForce::NetworkMessage::AbilityTryClaim m;
				m.User = network->ID.UserID;

				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID) ID_TIMESTAMP);
				bs.Write(RakNet::GetTime());
				bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilityTryClaim);
				m.Serialize(true, &bs);

				m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
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

		// Check if we are dead and are currently charging/channeling, in which case, we should interrupt the ability.
		if (health->IsDead)
		{
			if (playerComponent->AbilityState == AbilityState::CHARGING || playerComponent->AbilityState == AbilityState::CHANNELING)
			{
				assert(action->CurrentAbilityEvent.ActiveAbility != ABILITY_INDEX_NONE);
				action->CurrentAbilityEvent.Type = AbilityEventType::INTERRUPTED;
				action->AbilityEvents.push(action->CurrentAbilityEvent);

				// Send this action to the server
				RootForce::NetworkMessage::AbilityEvent m;
				m.Event = action->CurrentAbilityEvent;

				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID) ID_TIMESTAMP);
				bs.Write(RakNet::GetTime());
				bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilityEvent);
				m.Serialize(true, &bs);

				m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

				//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityInterrupted with ActionID: %d", action->CurrentAbilityEvent.ActionID);

				// Reset the current event.
				playerComponent->AbilityState = AbilityState::OFF;
				action->CurrentAbilityEvent = AbilityEvent();
			}

			return;
		}

		
		// Update the action component only if we are a remote client
		//ECS::Entity* clientEntity = m_world->GetTagManager()->GetEntityByTag("Client");
		//Network::ClientComponent* clientComponent = m_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);

		// Send the action to the server.
		RootForce::NetworkMessage::PlayerCommand m;
		m.User = network->ID.UserID;

		m.MovePower = action->MovePower;
		m.StrafePower = action->StrafePower;
		m.Angle = action->Angle;
		m.JumpTime = action->JumpTime;
		m.SelectedAbility = action->SelectedAbility;

		m.Position = transform->m_position;
		m.Orientation = transform->m_orientation.GetQuaternion();
		m.AimingDeviceOrientation = aimingDeviceTransform->m_orientation.GetQuaternion();

		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID) ID_TIMESTAMP);
		bs.Write(RakNet::GetTime());
		bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::PlayerCommand);
		m.Serialize(true, &bs);

		if (m_clientPeer != nullptr)
			m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, UNRELIABLE_SEQUENCED, 0, m_clientPeer->GetSystemAddressFromIndex(0), false);



		m_inputtedActionsPreviousFrame = m_inputtedActionsCurrentFrame;
	}

	void PlayerControlSystem::HandleAbilityPressed(float p_dt, bool p_push)
	{
		ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
		
		PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
		PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(player);
		Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(player);
		HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(player);

		// Determine the ability we want to activate.
		uint8_t activeAbility = p_push ? PUSH_ABILITY_INDEX : playerComponent->SelectedAbility;

		// Check if we already have an ability active that needs to be cancelled.
		if (action->CurrentAbilityEvent.ActiveAbility != activeAbility)
		{
			// Cancel the ability and reset the current ability.
			//g_engineContext.m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Changed ability while holding mouse button (Active: %d, New: %d)", action->CurrentAbilityEvent.ActiveAbility, activeAbility);
			HandleAbilityReleased();
		}

		// Check if the new active ability is an empty slot, in which case, do not start activating.
		std::string abilityName = playerComponent->AbilityScripts[activeAbility].Name;
		if (abilityName == "")
			return;

		// Make sure the ability is off cooldown.
		if (playerComponent->AbilityScripts[activeAbility].OnCooldown)
			return;

		// Check if we should start charging.
		if (playerComponent->AbilityState == AbilityState::OFF)
		{
			assert(activeAbility != ABILITY_INDEX_NONE);
			action->CurrentAbilityEvent.ActionID = s_nextActionID++;
			action->CurrentAbilityEvent.ActiveAbility = activeAbility;
			action->CurrentAbilityEvent.ActiveAbilityScript = playerComponent->AbilityScripts[activeAbility].Name.c_str();
			action->CurrentAbilityEvent.Time = 0.0f;
			action->CurrentAbilityEvent.Type = AbilityEventType::CHARGE_START;
			action->AbilityEvents.push(action->CurrentAbilityEvent);

			// Send this action to the server
			RootForce::NetworkMessage::AbilityEvent m;
			m.Event = action->CurrentAbilityEvent;

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID) ID_TIMESTAMP);
			bs.Write(RakNet::GetTime());
			bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::AbilityEvent);
			m.Serialize(true, &bs);

			m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

			//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChargeStart with ActionID: %d", action->CurrentAbilityEvent.ActionID);
		}

		// Increase the time the ability has been activated.
		action->CurrentAbilityEvent.Time += p_dt;

		// Check for time limits.
		float abilityChargeTime = (float) g_engineContext.m_script->GetGlobalNumber("chargeTime", action->CurrentAbilityEvent.ActiveAbilityScript.C_String());
		float abilityChannelingTime = (float) g_engineContext.m_script->GetGlobalNumber("channelingTime", action->CurrentAbilityEvent.ActiveAbilityScript.C_String());
		float abilityCooldownTime = (float) g_engineContext.m_script->GetGlobalNumber("cooldown", action->CurrentAbilityEvent.ActiveAbilityScript.C_String());

		// Check to see if the charge time is up.
		if (action->CurrentAbilityEvent.Time >= abilityChargeTime && playerComponent->AbilityState == AbilityState::CHARGING)
		{
			assert(action->CurrentAbilityEvent.ActiveAbility != ABILITY_INDEX_NONE);
			action->CurrentAbilityEvent.Type = AbilityEventType::CHANNELING_START;
			action->AbilityEvents.push(action->CurrentAbilityEvent);

			// Send this action to the server
			RootForce::NetworkMessage::AbilityEvent m;
			m.Event = action->CurrentAbilityEvent;

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID) ID_TIMESTAMP);
			bs.Write(RakNet::GetTime());
			bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityEvent);
			m.Serialize(true, &bs);

			m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChargeDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);
		}

		if (action->CurrentAbilityEvent.Time >= abilityChargeTime + abilityChannelingTime && playerComponent->AbilityState == AbilityState::CHANNELING)
		{
			assert(action->CurrentAbilityEvent.ActiveAbility != ABILITY_INDEX_NONE);
			action->CurrentAbilityEvent.Type = AbilityEventType::CHANNELING_DONE;
			action->AbilityEvents.push(action->CurrentAbilityEvent);

			// Send this action to the server
			NetworkMessage::AbilityEvent m;
			m.Event = action->CurrentAbilityEvent;

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID) ID_TIMESTAMP);
			bs.Write(RakNet::GetTime());
			bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityEvent);
			m.Serialize(true, &bs);

			m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChannelingDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);
		}
	}

	void PlayerControlSystem::HandleAbilityReleased()
	{
		ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
		
		PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
		PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(player);
		Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(player);
		HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(player);

		switch (playerComponent->AbilityState)
		{
			case AbilityState::CHARGING:
			{
				assert(action->CurrentAbilityEvent.ActiveAbility != ABILITY_INDEX_NONE);
				action->CurrentAbilityEvent.Type = AbilityEventType::CHANNELING_START;
				action->AbilityEvents.push(action->CurrentAbilityEvent);

				// Send this action to the server
				NetworkMessage::AbilityEvent m;
				m.Event = action->CurrentAbilityEvent;

				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID) ID_TIMESTAMP);
				bs.Write(RakNet::GetTime());
				bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityEvent);
				m.Serialize(true, &bs);

				m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChargeDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);
			} 
			
			// Fall through here!

			case AbilityState::CHANNELING:
			{
				assert(action->CurrentAbilityEvent.ActiveAbility != ABILITY_INDEX_NONE);
				action->CurrentAbilityEvent.Type = AbilityEventType::CHANNELING_DONE;
				action->AbilityEvents.push(action->CurrentAbilityEvent);

				// Send this action to the server
				NetworkMessage::AbilityEvent m;
				m.Event = action->CurrentAbilityEvent;

				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID) ID_TIMESTAMP);
				bs.Write(RakNet::GetTime());
				bs.Write((RakNet::MessageID) NetworkMessage::MessageType::AbilityEvent);
				m.Serialize(true, &bs);

				m_clientPeer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
				//g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChannelingDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);
			} break;
		}

		// Reset the current event.
		playerComponent->AbilityState = AbilityState::OFF;
		action->CurrentAbilityEvent = AbilityEvent();
	}

	void PlayerControlSystem::SetDefaultKeybindings()
	{
		m_keybindings.clear();

		m_keybindings.push_back(Keybinding());
		m_keybindings.back().Edge = false;
		m_keybindings.back().ActionUp = PlayerAction::NONE;
		m_keybindings.back().Action = PlayerAction::MOVE_FORWARDS;
		m_keybindings.back().Bindings.push_back(SDL_SCANCODE_W);
		
		m_keybindings.push_back(Keybinding());
		m_keybindings.back().Edge = false;
		m_keybindings.back().ActionUp = PlayerAction::NONE;
		m_keybindings.back().Action = PlayerAction::MOVE_BACKWARDS;
		m_keybindings.back().Bindings.push_back(SDL_SCANCODE_S);
		
		m_keybindings.push_back(Keybinding());
		m_keybindings.back().Edge = false;
		m_keybindings.back().ActionUp = PlayerAction::NONE;
		m_keybindings.back().Action = PlayerAction::STRAFE_RIGHT;
		m_keybindings.back().Bindings.push_back(SDL_SCANCODE_D);

		m_keybindings.push_back(Keybinding());
		m_keybindings.back().Edge = false;
		m_keybindings.back().ActionUp = PlayerAction::NONE;
		m_keybindings.back().Action = PlayerAction::STRAFE_LEFT;
		m_keybindings.back().Bindings.push_back(SDL_SCANCODE_A);

		m_keybindings.push_back(Keybinding());
		m_keybindings.back().Edge = false;
		m_keybindings.back().ActionUp = PlayerAction::NONE;
		m_keybindings.back().Action = PlayerAction::STRAFE_LEFT;
		m_keybindings.back().Bindings.push_back(SDL_SCANCODE_A);

		m_keybindings.push_back(Keybinding());
		m_keybindings.back().Edge = false;
		m_keybindings.back().ActionUp = PlayerAction::JUMP_RELEASED;
		m_keybindings.back().Action = PlayerAction::JUMP_PRESSED;
		m_keybindings.back().Bindings.push_back(SDL_SCANCODE_SPACE);

		/*
		kb.Edge = false;
		kb.ActionUp = PlayerAction::NONE;
		kb.Action = PlayerAction::SELECT_ABILITY1;
		kb.Bindings.push_back(SDL_SCANCODE_1);
		m_keybindings.push_back(kb);

		kb.Edge = false;
		kb.ActionUp = PlayerAction::NONE;
		kb.Action = PlayerAction::SELECT_ABILITY2;
		kb.Bindings.push_back(SDL_SCANCODE_2);
		m_keybindings.push_back(kb);

		kb.Edge = false;
		kb.ActionUp = PlayerAction::NONE;
		kb.Action = PlayerAction::SELECT_ABILITY3;
		kb.Bindings.push_back(SDL_SCANCODE_3);
		m_keybindings.push_back(kb);

		kb.Edge = false;
		kb.ActionUp = PlayerAction::ACTIVATE_ABILITY_RELEASED;
		kb.Action = PlayerAction::ACTIVATE_ABILITY_PRESSED;
		kb.Bindings.push_back(SDL_Scancode(490));
		m_keybindings.push_back(kb);

		kb.Edge = false;
		kb.ActionUp = PlayerAction::JUMP_RELEASED;
		kb.Action = PlayerAction::JUMP_PRESSED;
		kb.Bindings.push_back(SDL_Scancode(44));
		m_keybindings.push_back(kb);
		
		kb.Edge = false;
		kb.ActionUp = PlayerAction::NONE;
		kb.Action = PlayerAction::PICK_UP_ABILITY;
		kb.Bindings.push_back(SDL_Scancode(225));
		m_keybindings.push_back(kb);

		kb.Edge = false;
		kb.ActionUp = PlayerAction::ACTIVATE_PUSH_ABILITY_RELEASED;
		kb.Action = PlayerAction::ACTIVATE_PUSH_ABILITY_PRESSED;
		kb.Bindings.push_back(SDL_Scancode(492));
		m_keybindings.push_back(kb);
		
		kb.Edge = true;
		kb.ActionUp = PlayerAction::NONE;
		kb.Action = PlayerAction::SWITCH_ABILITY_FORWARD;
		kb.Bindings.push_back(SDL_Scancode(20));
		m_keybindings.push_back(kb);
		
		kb.Edge = true;
		kb.ActionUp = PlayerAction::NONE;
		kb.Action = PlayerAction::SCROLL_ABILITY_FORWARD;
		kb.Bindings.push_back(SDL_Scancode(493));
		m_keybindings.push_back(kb);

		kb.Edge = false;
		kb.ActionUp = PlayerAction::NONE;
		kb.Action = PlayerAction::SCROLL_ABILITY_BACKWARD;
		kb.Bindings.push_back(SDL_Scancode(494));
		m_keybindings.push_back(kb);
		*/
	}

}

#endif
