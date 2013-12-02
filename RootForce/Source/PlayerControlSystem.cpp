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
		: ECS::EntitySystem(p_world) 
	{
		SetUsage<Transform>();
		SetUsage<PlayerInputControlComponent>();
	}

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

	void PlayerControlSystem::Init()
	{
		m_transforms.Init(m_world->GetEntityManager());
		m_controllers.Init(m_world->GetEntityManager());
	}

	void PlayerControlSystem::Begin()
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
	}

	void PlayerControlSystem::ProcessEntity(ECS::Entity* p_entity)
	{
		float dt = m_world->GetDelta();
		Transform* transform = m_transforms.Get(p_entity);
		PlayerInputControlComponent* controller = m_controllers.Get(p_entity);

		// TODO: The facing needs to be in the transform component
		glm::vec3 facing = transform->m_orientation.GetFront();
		
		// TODO: The speed needs to be variable (in what component?) and centralized.
		float speed = controller->speed;

		for (PlayerAction::PlayerAction currentAction : m_inputtedActionsCurrentFrame)
		{
			switch (currentAction)
			{
				case PlayerAction::MOVE_FORWARDS:
					transform->m_position += facing * speed * dt;
					//m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Player position: (%f, %f, %f)", transform->m_position.x, transform->m_position.y, transform->m_position.z);

					break;
				case PlayerAction::MOVE_BACKWARDS:
					transform->m_position -= facing * speed * dt;
					//m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Player position: (%f, %f, %f)", transform->m_position.x, transform->m_position.y, transform->m_position.z);
					break;
				case PlayerAction::STRAFE_RIGHT:
					transform->m_orientation.YawGlobal(-90.0f * dt);
					break;
				case PlayerAction::STRAFE_LEFT:
					// TODO: Implement strafing
					transform->m_orientation.YawGlobal(90.0f * dt);
					break;
				case PlayerAction::ACTIVATE_ABILITY:
					// TODO: Implement activation of abilities
					

					break;
				default:
					break;
			}
		}
	}

	void PlayerControlSystem::End()
	{
		m_inputtedActionsPreviousFrame = m_inputtedActionsCurrentFrame;
	}
}