#include <PlayerControlSystem.h>
#include <RootSystems\Include\ScriptSystem.h>

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
					if (m_inputManager->GetKeyState(sc) == RootEngine::InputManager::KeyState::DOWN_EDGE)
					{
						m_inputtedActionsCurrentFrame.push_back(kb.Action);
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
		Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<Transform>(m_world->GetTagManager()->GetEntityByTag("AimingDevice"));

		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(entity);
		PlayerControl* controller = m_world->GetEntityManager()->GetComponent<PlayerControl>(entity);
		PhysicsAccessor* physAcc = m_world->GetEntityManager()->GetComponent<PhysicsAccessor>(entity);
		Player* player = m_world->GetEntityManager()->GetComponent<Player>(entity);

		// Get the facing and calculate the right direction. Facing is assumed to be normalized, and up is assumed to be (0, 1, 0).
		glm::vec3 facing = transform->m_orientation.GetFront();
		glm::vec3 right = transform->m_orientation.GetRight();
		
		glm::vec3 movement(0.0f);
		m_angle.x = 0;

		// Get the speed of the player
		float speed = controller->m_speed;

		for (PlayerAction::PlayerAction currentAction : m_inputtedActionsCurrentFrame)
		{
			switch (currentAction)
			{
				case PlayerAction::MOVE_FORWARDS:
					movement += facing;
					break;
				case PlayerAction::MOVE_BACKWARDS:
					{
						movement += -facing;
					}
					break;
				case PlayerAction::STRAFE_RIGHT:
					movement += right;
					//transform->m_orientation.YawGlobal(-90.0f * dt);
					break;
				case PlayerAction::STRAFE_LEFT:
					{
						movement += -right;
						//m_physics->SetVelocity(*(physAcc->m_handle), -right);
					}
					break;
				case PlayerAction::ORIENTATE:
					{
						//m_physics->SetPlayerOrientation(playerID, orientation);
						//m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Reorienting: Delta (%d, %d)", m_deltaMouseMovement.x, m_deltaMouseMovement.y);
						// TODO: Update a camera controller with m_deltaMouseMovement.
						//transform->m_orientation.Pitch(m_deltaMouseMovement.y * controller->m_mouseSensitivity);
						//transform->m_orientation.YawGlobal(-m_deltaMouseMovement.x * controller->m_mouseSensitivity);
						m_angle.x = -m_deltaMouseMovement.x;
						m_angle.y += m_deltaMouseMovement.y;
					}
					break;
				case PlayerAction::SELECT_ABILITY:
					// TODO: Implement selection of abilities.

					break;
				case PlayerAction::ACTIVATE_ABILITY:
		
					if(player->m_selectedAbility == Abilitiy::ABILITY_TEST)
					{
						ECS::Entity* entity = m_world->GetEntityManager()->CreateEntity();
						Script* script = m_world->GetEntityManager()->CreateComponent<Script>(entity);
						script->m_name = g_engineContext.m_resourceManager->GetScript("AbilityTest");
						script->m_actions.push_back(Action(ActionType::ACTION_CREATE));
					}

					break;
				default:
					break;
			}
		}
		transform->m_orientation.YawGlobal(m_angle.x);

		
		if(movement != glm::vec3(0.0f))
			m_physics->SetPosition(*(physAcc->m_handle), movement);
			//m_physics->SetVelocity(*(physAcc->m_handle), movement);
		m_physics->SetOrientation(*(physAcc->m_handle), transform->m_orientation.GetQuaternion());
		m_inputtedActionsPreviousFrame = m_inputtedActionsCurrentFrame;
	}

	void PlayerControlSystem::UpdateAimingDevice()
	{
		Transform* transform = m_world->GetEntityManager()->GetComponent<Transform>(m_world->GetTagManager()->GetEntityByTag("Player"));
		Transform* aimingDeviceTransform = m_world->GetEntityManager()->GetComponent<Transform>(m_world->GetTagManager()->GetEntityByTag("AimingDevice"));

		aimingDeviceTransform->m_orientation.SetOrientation(transform->m_orientation.GetQuaternion());
		aimingDeviceTransform->m_orientation.Pitch(m_angle.y);
		aimingDeviceTransform->m_position = transform->m_position + transform->m_orientation.GetUp() * 2.5f + transform->m_orientation.GetRight() * 1.0f + transform->m_orientation.GetFront() * 2.0f;
	}
}