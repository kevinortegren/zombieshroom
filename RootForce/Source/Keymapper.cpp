#include "Keymapper.h"

#include <RootEngine/GUI/Include/WebView.h>
#include <algorithm>

namespace RootForce
{
	PlayerAction::PlayerAction ActionFromString(std::string p_action)
	{
		// Preprocess string, if prefix found, remove it. Then capitalize string
		std::string prefix = "settings-key-";
		if(p_action.find(prefix) != std::string::npos)
			p_action = p_action.substr(p_action.find(prefix)+prefix.length());
		std::transform(p_action.begin(), p_action.end(), p_action.begin(), ::toupper);

		PlayerAction::PlayerAction action;
		if(p_action.compare("MOVE_FORWARDS")==0)
			action = PlayerAction::MOVE_FORWARDS;
		else if(p_action.compare("MOVE_BACKWARDS")==0)
			action = PlayerAction::MOVE_BACKWARDS;
		else if(p_action.compare("STRAFE_RIGHT")==0)
			action = PlayerAction::STRAFE_RIGHT;
		else if(p_action.compare("STRAFE_LEFT")==0)
			action = PlayerAction::STRAFE_LEFT;
		else if(p_action.compare("SELECT_ABILITY1")==0)
			action = PlayerAction::SELECT_ABILITY1;
		else if(p_action.compare("SELECT_ABILITY2")==0)
			action = PlayerAction::SELECT_ABILITY2;
		else if(p_action.compare("SELECT_ABILITY3")==0)
			action = PlayerAction::SELECT_ABILITY3;
		else if(p_action.compare("ACTIVATE_ABILITY")==0)
			action = PlayerAction::ACTIVATE_ABILITY;
		else if(p_action.compare("JUMP")==0)
			action = PlayerAction::JUMP;
		else
			action = PlayerAction::NONE;
		return action;
	}
	
	SDL_Scancode Keymapper::GetActionBinding(std::string p_actionId)
	{
		return GetActionBinding(ActionFromString(p_actionId));
	}
	SDL_Scancode Keymapper::GetActionBinding(PlayerAction::PlayerAction p_actionId)
	{
		if(p_actionId == PlayerAction::NONE)
			return SDL_SCANCODE_UNKNOWN;
		return m_actionMap[p_actionId];
	}
	void Keymapper::SetActionBinding(std::string p_actionId, SDL_Scancode p_key)
	{
		SetActionBinding(ActionFromString(p_actionId), p_key);
	}
	void Keymapper::SetActionBinding(PlayerAction::PlayerAction p_actionId, SDL_Scancode p_key)
	{
		if(p_actionId == PlayerAction::NONE)
			return;
		m_actionMap[p_actionId] = p_key;
	}
	std::vector<Keybinding> Keymapper::GetKeybindings()
	{
		std::vector<Keybinding> tmp;
		for(auto bind : m_actionMap)
		{
			Keybinding tmpkey;
			tmpkey.Edge = false;
			tmpkey.Action = bind.first;
			tmpkey.Bindings.push_back(bind.second);
			tmp.push_back(tmpkey);
		}
		return tmp;
	}

	// Used by menu to focus keybinding
	void Keymapper::FocusBindAction(std::string p_actionId)
	{
		FocusBindAction(ActionFromString(p_actionId));
	}
	void Keymapper::FocusBindAction(PlayerAction::PlayerAction p_actionId)
	{
		m_bindAction = p_actionId;
	}
	// Used by menu or C++ code to unfocus keybinding
	void Keymapper::UnfocusEvent()
	{
		m_bindAction = RootForce::PlayerAction::NONE;
	}
	void Keymapper::UnfocusBindAction()
	{
		m_bindAction = RootForce::PlayerAction::NONE;
		// Make all the keybind inputs lose focus
		if(m_menu)
			m_menu->BufferJavascript("$('.controls-settings-keybind').blur();");
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
		// Force the GUI to reload the keybindings
		if(m_menu)
			m_menu->BufferJavascript("$('.controls-settings-keybind:focus').val(" + std::to_string(value) + ");");
		UnfocusBindAction();

		return true;
	}

	void Keymapper::SetMenu(RootEngine::GUISystem::WebView* p_menu)
	{
		m_menu = p_menu;
	}
}