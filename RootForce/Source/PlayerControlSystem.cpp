#include <PlayerControlSystem.h>
#include <gtest/gtest.h>

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

	void PlayerControlSystem::SetLoggingInterface(Logging* logger)
	{
		m_logger = logger;
	}

	void PlayerControlSystem::SetInputInterface(RootEngine::InputManager::InputInterface* inputManager)
	{
		m_inputManager = inputManager;
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
		if (m_deltaMouseMovement != glm::vec2(0, 0))
		{
			m_inputtedActionsCurrentFrame.push_back(PlayerAction::ORIENTATE);
		}

		// TODO: Check for selection of abilities (whether the keypad has been pressed or mouse wheel been scrolled?)


		// Get the properties we need.
		float dt = m_world->GetDelta();
		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(m_world->GetTagManager()->GetEntityByTag("Player"));
		PlayerInputControlComponent* controller = m_world->GetEntityManager()->GetComponent<PlayerInputControlComponent>(m_world->GetTagManager()->GetEntityByTag("Player"));

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = glm::normalize(glm::cross(facing, glm::vec3(0.0f, 1.0f, 0.0f)));

		// Get the speed of the player
		float speed = controller->speed;

		for (PlayerAction::PlayerAction currentAction : m_inputtedActionsCurrentFrame)
		{
			switch (currentAction)
			{
				case PlayerAction::MOVE_FORWARDS:
					transform->m_position += facing * speed * dt;
					break;
				case PlayerAction::MOVE_BACKWARDS:
					transform->m_position -= facing * speed * dt;
					break;
				case PlayerAction::STRAFE_RIGHT:
					transform->m_position += right * speed * dt;
					//transform->m_orientation.YawGlobal(-90.0f * dt);
					break;
				case PlayerAction::STRAFE_LEFT:
					transform->m_position -= right * speed * dt;
					//transform->m_orientation.YawGlobal(90.0f * dt);
					break;
				case PlayerAction::ORIENTATE:
					//m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Reorienting");
					// TODO: Update a camera controller with m_deltaMouseMovement.
					break;
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