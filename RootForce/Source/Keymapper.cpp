#include "Keymapper.h"

//#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>

namespace RootForce
{
	SDL_Scancode Keymapper::GetActionBinding(PlayerAction::PlayerAction p_actionId)
	{
		return m_actionMap[p_actionId];
	}
	void Keymapper::SetActionBinding(PlayerAction::PlayerAction p_actionId, SDL_Scancode p_key)
	{
		m_actionMap[p_actionId] = p_key;
	}
	std::vector<Keybinding> Keymapper::GetKeybindings()
	{
		std::vector<Keybinding> tmp;
		for(auto bind : m_actionMap)
			tmp.push_back(Keybinding(bind.second,bind.first));
		return tmp;
	}

	// Used by menu to focus/unfocus keybinding
	void Keymapper::FocusBindAction(PlayerAction::PlayerAction p_actionId)
	{
		m_bindAction = p_actionId;
	}

	void Keymapper::UnfocusBindAction()
	{
		m_bindAction = RootForce::PlayerAction::NONE;
	}

	// Attempt to process a key assignment if a keybinding is being changed (has focus)
	//   otherwise do nothing and return false (to indicate the event should be processed further)
	bool Keymapper::ProcessKey(SDL_Event p_event)
	{
		if(m_bindAction == RootForce::PlayerAction::NONE)
			return false;

		SDL_Scancode value = (SDL_Scancode)0;
		if(p_event.type == SDL_KEYDOWN)
			value = p_event.key.keysym.scancode;
		else if(p_event.type == SDL_MOUSEBUTTONDOWN)
			value = (SDL_Scancode)(p_event.button.button-SDL_BUTTON_LEFT+RootEngine::InputManager::MouseButton::LEFT);
		else
			return false;

		if(value == SDL_SCANCODE_ESCAPE)
		{
			UnfocusBindAction();
			return false;
		}

		m_actionMap[m_bindAction] = value;
		m_bindAction = RootForce::PlayerAction::NONE;
		return true;
	}
}