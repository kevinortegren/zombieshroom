#pragma once

#include <map>
#include <SDL2/SDL.h>

#include <RootSystems/Include/PlayerControlSystem.h>

namespace RootForce
{
	class Keymapper
	{
	public:
		SDL_Scancode GetActionBinding(PlayerAction::PlayerAction p_actionId);
		void SetActionBinding(PlayerAction::PlayerAction p_actionId, SDL_Scancode p_key);
		std::vector<Keybinding> GetKeybindings();
		
		// Used by menu to focus/unfocus keybinding
		void FocusBindAction(PlayerAction::PlayerAction p_actionId);
		void UnfocusBindAction();
		// Attempt to process a key assignment if a keybinding is being changed (has focus)
		//   otherwise do nothing and return false (to indicate the event should be processed further)
		bool ProcessKey(SDL_Event p_event);

	private:
		// The current action to bind, 0 for none
		PlayerAction::PlayerAction m_bindAction;

		std::map<PlayerAction::PlayerAction, SDL_Scancode> m_actionMap;
	};
}