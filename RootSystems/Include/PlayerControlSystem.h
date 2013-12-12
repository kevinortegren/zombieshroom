#pragma once

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Physics.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Physics/Include/RootPhysics.h>

namespace RootForce
{
	/** Component owned by a player entity that will be controlled via input. */
	struct PlayerControl : public ECS::Component<PlayerControl>
	{
		float m_speed;
		float m_mouseSensitivity;
	};

	/** 
		Enumerate all the actions a player can do via input.

		TODO: Need to consider parameters
	*/
	namespace PlayerAction
	{
		enum PlayerAction
		{
			NONE,
			MOVE_FORWARDS,
			MOVE_BACKWARDS,
			STRAFE_RIGHT,
			STRAFE_LEFT,
			ORIENTATE,
			SELECT_ABILITY,
			ACTIVATE_ABILITY
		};
	}

	/** An association between a set of keys and a player action. */
	struct Keybinding
	{
		PlayerAction::PlayerAction Action;
		std::vector<SDL_Scancode> Bindings;
		bool Edge;

		Keybinding();
		Keybinding(SDL_Scancode binding, PlayerAction::PlayerAction action);
	};


	/** This system will update the player entity depending on input received. */
	class PlayerControlSystem : public ECS::VoidSystem
	{
	public:
		PlayerControlSystem(ECS::World* p_world);

		void SetKeybindings(const std::vector<Keybinding>& keybindings);
		void SetLoggingInterface(Logging* p_logger);
		void SetInputInterface(RootEngine::InputManager::InputInterface* p_inputManager);
		void SetPhysicsInterface(RootEngine::Physics::PhysicsInterface* p_physics);
		
		void Process();
		void UpdateAimingDevice();
	private:
		std::vector<Keybinding> m_keybindings;

		glm::ivec2 m_deltaMouseMovement;
		glm::vec2 m_angle;
		std::vector<PlayerAction::PlayerAction> m_inputtedActionsCurrentFrame;
		std::vector<PlayerAction::PlayerAction> m_inputtedActionsPreviousFrame;

		Logging* m_logger;
		RootEngine::InputManager::InputInterface* m_inputManager;
		RootEngine::Physics::PhysicsInterface* m_physics;
	};
}