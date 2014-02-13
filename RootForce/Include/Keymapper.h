#pragma once

#include <map>
#include <SDL2/SDL.h>

#include <RootSystems/Include/PlayerControlSystem.h>

namespace RootForce
{
	class Keymapper
	{
	public:
		Keymapper()
			: m_bindAction(nullptr)
		{
		}
		SDL_Scancode GetActionBinding(std::string p_actionId);
		void SetActionBinding(std::string p_actionId, SDL_Scancode p_key);
		std::vector<Keybinding> GetKeybindings();
		
		// Used by menu to focus/unfocus keybinding
		void FocusBindAction(std::string p_actionId);
		void UnfocusEvent();
		// Used by C++ to unfocus the gui indicator as well (separated because of race condition issues with JavaScript)
		void UnfocusBindAction();
		// Attempt to process a key assignment if a keybinding is being changed (has focus)
		//   otherwise do nothing and return false (to indicate the event should be processed further)
		bool ProcessKey(SDL_Event p_event);

		void SetMenu(RootEngine::GUISystem::WebView* p_menu);

	private:
		// The current action to bind, 0 for none
		Keybinding* m_bindAction;

		std::map<std::string, Keybinding> m_actionMap;
		RootEngine::GUISystem::WebView* m_menu;
	};
}