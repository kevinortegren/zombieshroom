#pragma once

#include <map>
#include <Utility\ECS\Include\World.h>
#include <Transform.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace RootForce
{
	/** Component owned by a player entity that will be controlled via input. */
	struct PlayerInputControlComponent : public ECS::Component<PlayerInputControlComponent>
	{
		float speed;
	};


	/** 
		Enumerate all the actions a player can do via input. This is actions that should be reconfigurable via keybindings and has no extra parameters. 
		
		Examples of actions that should not be enumerated here:
			* Turn player/camera (not usually reconfigurable, has a parameter: the delta position of the mouse).
			* Reactions, for instance an animation that happens when hit.
	*/
	namespace PlayerAction
	{
		enum PlayerAction
		{
			NONE,
			MOVE_FORWARDS,
			MOVE_BACKWARDS,
			STRAFE_LEFT,
			STRAFE_RIGHT,
			ACTIVATE_ABILITY	// TODO: Not implemented, need to consider mouse presses too.
		};
	}

	/** An association between a set of keys and a player action. */
	struct Keybinding
	{
		PlayerAction::PlayerAction Action;
		std::vector<SDL_Scancode> Bindings;

		Keybinding();
		Keybinding(SDL_Scancode binding, PlayerAction::PlayerAction action);
	};


	/** This system will update the player entity depending on input received. */
	class PlayerControlSystem : public ECS::ComponentSystem
	{
	public:
		PlayerControlSystem(ECS::World* p_world);

		void SetKeybindings(const std::vector<Keybinding>& keybindings);
		void SetLoggingInterface(Logging* p_logger);
		void SetInputInterface(RootEngine::InputManager::InputInterface* p_inputManager);

		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
	private:
		std::vector<Keybinding> m_keybindings;
		ECS::ComponentMapper<Transform> m_transforms;
		ECS::ComponentMapper<PlayerInputControlComponent> m_controllers;

		std::vector<PlayerAction::PlayerAction> m_inputtedActionsCurrentFrame;
		std::vector<PlayerAction::PlayerAction> m_inputtedActionsPreviousFrame;

		Logging* m_logger;
		RootEngine::InputManager::InputInterface* m_inputManager;
	};
}