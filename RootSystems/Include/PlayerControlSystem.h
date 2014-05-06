#pragma once

#ifndef COMPILE_LEVEL_EDITOR

#include <map>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/PhysicsSystem.h>
#include <RootSystems/Include/Transform.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootEngine/InputManager/Include/InputInterface.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RakNet/RakPeerInterface.h>
#include <RootForce/Include/HUD.h>

namespace RootForce
{
	/** Component owned by a player entity that will be controlled via input. */
	

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
			SELECT_ABILITY1,
			SELECT_ABILITY2,
			SELECT_ABILITY3,
			ACTIVATE_ABILITY_PRESSED,
			ACTIVATE_ABILITY_RELEASED,
			JUMP_PRESSED,
			JUMP_RELEASED,
			ACTIVATE_PUSH_ABILITY_PRESSED,
			ACTIVATE_PUSH_ABILITY_RELEASED,
			PICK_UP_ABILITY,
			SCROLL_ABILITY_FORWARD,
			SCROLL_ABILITY_BACKWARD,
			SWITCH_ABILITY_FORWARD,
			MOVE_FORWARDS_EDGE,
			MOVE_BACKWARDS_EDGE,

			END
		};
	}

	/** An association between a set of keys and a player action. */
	struct Keybinding
	{
		PlayerAction::PlayerAction Action;
		PlayerAction::PlayerAction ActionUp;
		std::vector<SDL_Scancode> Bindings;
		bool Edge;
		float PressedTime;
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
		void SetHUD(HUD* p_hud) {m_hud = p_hud;}
		
		void Process();
		void SetClientPeer(RakNet::RakPeerInterface* p_clientPeer);
	private:
		void HandleAbilityPressed(float p_dt, bool p_push);
		void HandleAbilityReleased();
		void SetDefaultKeybindings();

		struct PlayerActionData
		{
			float ActiveTime;
		} m_playerActionData[PlayerAction::END];

		static Network::ActionID_t s_nextActionID;
		std::vector<Keybinding> m_keybindings;

		glm::ivec2 m_deltaMouseMovement;
		glm::vec2 m_angle;
		std::vector<PlayerAction::PlayerAction> m_inputtedActionsCurrentFrame;
		std::vector<PlayerAction::PlayerAction> m_inputtedActionsPreviousFrame;

		Logging* m_logger;
		RootEngine::InputManager::InputInterface* m_inputManager;
		RootEngine::Physics::PhysicsInterface* m_physics;
		RakNet::RakPeerInterface* m_clientPeer;
		
		HUD* m_hud;
	};
}

#endif