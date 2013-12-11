#include <PlayerControlSystem.h>

namespace RootForce
{
	Keybinding::Keybinding()
		: Action(PlayerAction::NONE)
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
				if (m_inputManager->GetKeyState(sc) == RootEngine::InputManager::KeyState::DOWN)
				{
					m_inputtedActionsCurrentFrame.push_back(kb.Action);
					break;
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
		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(m_world->GetTagManager()->GetEntityByTag("Player"));
		PlayerControl* controller = m_world->GetEntityManager()->GetComponent<PlayerControl>(m_world->GetTagManager()->GetEntityByTag("Player"));
		PhysicsAccessor* physAcc = m_world->GetEntityManager()->GetComponent<PhysicsAccessor>(m_world->GetTagManager()->GetEntityByTag("Player"));

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = transform->m_orientation.GetRight();
		
		// Get the speed of the player
		float speed = controller->m_speed;

		for (PlayerAction::PlayerAction currentAction : m_inputtedActionsCurrentFrame)
		{
			switch (currentAction)
			{
				case PlayerAction::MOVE_FORWARDS:
					m_physics->PlayerMoveXZ(*(physAcc->m_handle), facing);
					break;
				case PlayerAction::MOVE_BACKWARDS:
					{
						m_physics->PlayerMoveXZ(*(physAcc->m_handle), -facing);
						//m_physics->PlayerKnockback(*(physAcc->m_handle), &backwards.x, 5.f);
					}
					break;
				case PlayerAction::STRAFE_RIGHT:
					m_physics->PlayerMoveXZ(*(physAcc->m_handle), right);
					transform->m_orientation.LookAt(-transform->m_position/*glm::vec3(0.3f, 0.1f, 0.5f)*/, glm::vec3(0.0f, 1.0f, 0.0f));
					//transform->m_orientation.YawGlobal(-90.0f * dt);
					break;
				case PlayerAction::STRAFE_LEFT:

					{
						m_physics->PlayerMoveXZ(*(physAcc->m_handle), -right);
						transform->m_orientation.LookAt(-transform->m_position/*glm::vec3(0.3f, 0.1f, 0.5f)*/, glm::vec3(0.0f, 1.0f, 0.0f));
					}
					break;
				case PlayerAction::ORIENTATE:
				{
					//m_physics->SetPlayerOrientation(playerID, orientation);
					//m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Reorienting: Delta (%d, %d)", m_deltaMouseMovement.x, m_deltaMouseMovement.y);
					// TODO: Update a camera controller with m_deltaMouseMovement.
					transform->m_orientation.Pitch(m_deltaMouseMovement.y * controller->m_mouseSensitivity);
					transform->m_orientation.YawGlobal(-m_deltaMouseMovement.x * controller->m_mouseSensitivity);
					//m_physics->SetPlayerOrientation(*(physAcc->m_handle), &(transform->m_orientation.GetQuaterion()).w); 
					
				} break;
				case PlayerAction::SELECT_ABILITY:
					// TODO: Implement selection of abilities.

					break;
				case PlayerAction::ACTIVATE_ABILITY:
					// TODO: Implement activation of abilities.
					break;
				default:
					break;
			}
		}

		m_inputtedActionsPreviousFrame = m_inputtedActionsCurrentFrame;
	}
}