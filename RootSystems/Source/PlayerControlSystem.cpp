#ifndef COMPILE_LEVEL_EDITOR

#include <PlayerControlSystem.h>
#include <RootSystems/Include/AnimationSystem.h>
#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
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

	void PlayerControlSystem::Process()
	{
		// Check what keys have been pressed this frame and update the action list.
		m_inputtedActionsCurrentFrame.clear();
		for (Keybinding kb : m_keybindings)
		{
			for (SDL_Scancode sc : kb.Bindings)
			{
				if(kb.Edge)
				{
					RootEngine::InputManager::KeyState::KeyState keystate = m_inputManager->GetKeyState(sc);
					if(keystate == RootEngine::InputManager::KeyState::DOWN_EDGE)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.Action);
						break;
					}
					if(keystate == RootEngine::InputManager::KeyState::UP_EDGE)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.ActionUp);
						break;
					}
				}
				else
				{
					if(m_inputManager->GetKeyState(sc) == RootEngine::InputManager::KeyState::DOWN)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.Action);
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

		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);
		Player* player = m_world->GetEntityManager()->GetComponent<Player>(entity);
		PlayerControl* controller = m_world->GetEntityManager()->GetComponent<PlayerControl>(entity);
		PlayerPhysics* playerphysics = m_world->GetEntityManager()->GetComponent<PlayerPhysics>(entity);
		Collision* collision = m_world->GetEntityManager()->GetComponent<Collision>(entity);
		Animation* animation = m_world->GetEntityManager()->GetComponent<Animation>(entity);
		PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(entity);

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = transform->m_orientation.GetRight();

		glm::vec3 movement(0.0f);

		// Get the speed of the player
		float speed = playerphysics->MovementSpeed;

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
			case PlayerAction::ACTIVATE_ABILITY:
				{
					action->ActivateAbility = true;
				}
				break;
			case PlayerAction::JUMP:
				{
					action->Jump = true;
					animation->m_animClip = AnimationClip::JUMP_START;
				}
				break;
			default:
				break;
			}

			if(action->StrafePower == 0 && action->MovePower == 0)
				animation->m_animClip = AnimationClip::IDLE;
			if(action->MovePower == -1)
				animation->m_animClip = AnimationClip::WALKING;
			else if(action->MovePower == 1)
				animation->m_animClip = AnimationClip::WALKING;
			if(action->StrafePower == 1)
				animation->m_animClip = AnimationClip::STRAFE_RIGHT;
			else if(action->StrafePower == -1)
				animation->m_animClip = AnimationClip::STRAFE_LEFT;
		}

		m_inputtedActionsPreviousFrame = m_inputtedActionsCurrentFrame;
	}

	void PlayerControlSystem::UpdateAimingDevice()
	{
		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(m_world->GetTagManager()->GetEntityByTag("Player"));
		Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<Transform>(m_world->GetTagManager()->GetEntityByTag("AimingDevice"));
		PlayerActionComponent* action = m_world->GetEntityManager()->GetComponent<PlayerActionComponent>(m_world->GetTagManager()->GetEntityByTag("Player"));

		aimingDeviceTransform->m_orientation.SetOrientation(transform->m_orientation.GetQuaternion());
		aimingDeviceTransform->m_orientation.Pitch(action->Angle.y);
		aimingDeviceTransform->m_position = transform->m_position + transform->m_orientation.GetUp() * 4.5f;
	}
}

#endif
