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
		HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(entity);
		PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(entity);
		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);
		Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<Transform>(aimingDevice);
		Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(entity);
		Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(entity);

		if(!controller || !action || !health || !playerComponent || !transform || !aimingDeviceTransform || !network || !collision)
			return;

		bool onGround = g_engineContext.m_physics->IsOnGround(*collision->m_handle);
		float power = 0;
		action->MovePower = 0;
		action->StrafePower = 0;
		if(onGround)
		{
			power = 1;
		}
		else
		{
			power = 0.03f;
		}
		for (PlayerAction::PlayerAction currentAction : m_inputtedActionsCurrentFrame)
		{
			switch (currentAction)
			{
			case PlayerAction::MOVE_FORWARDS:
					action->MovePower += power;
				
				break;
			case PlayerAction::MOVE_BACKWARDS:
					action->MovePower -= power;
				break;
			case PlayerAction::STRAFE_RIGHT:
					action->StrafePower += power;
				break;
			case PlayerAction::STRAFE_LEFT:
					action->StrafePower -= power;
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

			case PlayerAction::ACTIVATE_ABILITY_PRESSED:
			{
				if (health->IsDead)
				{
					action->WantRespawn = true;
				}
				else
				{
					HandleAbilityPressed(dt, false);
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
				else if (playerComponent->AbilityState == AbilityState::OFF)
				{
					HandleAbilityPressed(dt, true);
				}
			}
			break;

			case PlayerAction::ACTIVATE_PUSH_ABILITY_RELEASED:
			{
				HandleAbilityReleased();
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

		// Determine the ability we want to activate.
		uint8_t activeAbility = p_push ? PUSH_ABILITY_INDEX : playerComponent->SelectedAbility;

		// Make sure the ability is off cooldown.
		if (!playerComponent->AbilityScripts[activeAbility].OnCooldown)
		{
			// Check if we already have an ability active that needs to be cancelled.
			if (action->CurrentAbilityEvent.ActiveAbility != activeAbility)
			{
				// Cancel the ability and reset the current ability.
				HandleAbilityReleased();
			}

			// Check if we should start charging.
			if (playerComponent->AbilityState == AbilityState::OFF)
			{
				action->CurrentAbilityEvent.ActionID = s_nextActionID++;
				action->CurrentAbilityEvent.ActiveAbility = activeAbility;
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

				g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChargeStart with ActionID: %d", action->CurrentAbilityEvent.ActionID);
			}

			// Increase the time the ability has been activated.
			action->CurrentAbilityEvent.Time += p_dt;

			// Check for time limits.
			std::string abilityName = playerComponent->AbilityScripts[action->CurrentAbilityEvent.ActiveAbility].Name;
			if (abilityName != "")
			{
				float abilityChargeTime = (float) g_engineContext.m_script->GetGlobalNumber("chargeTime", abilityName);
				float abilityChannelingTime = (float) g_engineContext.m_script->GetGlobalNumber("channelingTime", abilityName);
				float abilityCooldownTime = (float) g_engineContext.m_script->GetGlobalNumber("cooldown", abilityName);

				// Update the HUD (charging/channeling bar). TODO: Perhaps move this outside?
				if(action->CurrentAbilityEvent.Time <= abilityChargeTime)
                    m_hud->SetValue("ChargeBarValue", std::to_string(action->CurrentAbilityEvent.Time/abilityChargeTime));
                else if(abilityChannelingTime > 0)
                    m_hud->SetValue("ChargeBarValue", std::to_string((abilityChargeTime + abilityChannelingTime - action->CurrentAbilityEvent.Time)/abilityChannelingTime));
                else if(abilityChargeTime > 0) // Make sure the charge bar reaches the end before fading out
                    m_hud->SetValue("ChargeBarValue", "1");

				// Check to see if the charge time is up.
				if (action->CurrentAbilityEvent.Time >= abilityChargeTime && playerComponent->AbilityState == AbilityState::CHARGING)
				{
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
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChargeDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);
				}

				if (action->CurrentAbilityEvent.Time >= abilityChargeTime + abilityChannelingTime && playerComponent->AbilityState == AbilityState::CHANNELING)
				{
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
					g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChannelingDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);
				}
			}
			else
			{
				g_engineContext.m_logger->LogText(LogTag::GAME, LogLevel::NON_FATAL_ERROR, "No script associated with active ability %d for user %u", action->CurrentAbilityEvent.ActiveAbility, network->ID.UserID);
			}

		}
	}

	void PlayerControlSystem::HandleAbilityReleased()
	{
		ECS::Entity* player = m_world->GetTagManager()->GetEntityByTag("Player");
		
		PlayerComponent* playerComponent = m_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
		PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(player);
		Network::NetworkComponent* network = m_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(player);
		HealthComponent* health = m_world->GetEntityManager()->GetComponent<HealthComponent>(player);

        m_hud->SetValue("ChargeBarValue", "0");

		switch (playerComponent->AbilityState)
		{
			case AbilityState::CHARGING:
			{
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
				g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChargeDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);
			} 
			
			// Fall through here!

			case AbilityState::CHANNELING:
			{
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
				g_engineContext.m_logger->LogText(LogTag::CLIENT, LogLevel::DEBUG_PRINT, "Sending AbilityChannelingDone with ActionID: %d", action->CurrentAbilityEvent.ActionID);

				// Reset the current event.
				action->CurrentAbilityEvent = AbilityEvent();
			} break;
		}
	}

}

#endif
