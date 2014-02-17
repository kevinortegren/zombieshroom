#include "Keymapper.h"

#include <RootEngine/GUI/Include/WebView.h>
#include <algorithm>

namespace RootForce
{
	std::string KeybindString(std::string p_action)
	{
		std::string prefix = "settings-key-";
		if(p_action.find(prefix) != std::string::npos)
			p_action = p_action.substr(p_action.find(prefix)+prefix.length());
		std::transform(p_action.begin(), p_action.end(), p_action.begin(), ::toupper);
		return p_action;
	}
	void ActionFromString(Keybinding &p_binding, std::string p_action)
	{
		p_binding.Edge = false;
		p_binding.ActionUp = PlayerAction::NONE;
		if(p_action.compare("MOVE_FORWARDS")==0)
			p_binding.Action = PlayerAction::MOVE_FORWARDS;
		else if(p_action.compare("MOVE_BACKWARDS")==0)
			p_binding.Action = PlayerAction::MOVE_BACKWARDS;
		else if(p_action.compare("STRAFE_RIGHT")==0)
			p_binding.Action = PlayerAction::STRAFE_RIGHT;
		else if(p_action.compare("STRAFE_LEFT")==0)
			p_binding.Action = PlayerAction::STRAFE_LEFT;
		else if(p_action.compare("SELECT_ABILITY1")==0)
			p_binding.Action = PlayerAction::SELECT_ABILITY1;
		else if(p_action.compare("SELECT_ABILITY2")==0)
			p_binding.Action = PlayerAction::SELECT_ABILITY2;
		else if(p_action.compare("SELECT_ABILITY3")==0)
			p_binding.Action = PlayerAction::SELECT_ABILITY3;
		else if(p_action.compare("ACTIVATE_ABILITY")==0)
		{
			p_binding.Action = PlayerAction::ACTIVATE_ABILITY_PRESSED;
			p_binding.ActionUp = PlayerAction::ACTIVATE_ABILITY_RELEASED;
			p_binding.Edge = true;
		}
		else if(p_action.compare("JUMP")==0)
		{
			p_binding.Action = PlayerAction::JUMP_PRESSED;
			p_binding.ActionUp = PlayerAction::JUMP_RELEASED;
			p_binding.Edge = true;
		}
		else
			p_binding.Action = PlayerAction::NONE;

	}
	
	SDL_Scancode Keymapper::GetActionBinding(std::string p_actionId)
	{
		return m_actionMap[KeybindString(p_actionId)].Bindings.at(0);
	}
	void Keymapper::SetActionBinding(std::string p_actionId, SDL_Scancode p_key)
	{
		std::string processedActionId = KeybindString(p_actionId);
		ActionFromString(m_actionMap[processedActionId], processedActionId);
		m_actionMap[processedActionId].Bindings.clear();
		m_actionMap[processedActionId].Bindings.push_back(p_key);
	}
	std::vector<Keybinding> Keymapper::GetKeybindings()
	{
		std::vector<Keybinding> tmp;
		for(auto bind : m_actionMap)
		{
			if(bind.second.Action == PlayerAction::NONE)
				continue;
			tmp.push_back(bind.second);
		}
		return tmp;
	}

	// Used by menu to focus keybinding
	void Keymapper::FocusBindAction(std::string p_actionId)
	{
		std::string processedActionId = KeybindString(p_actionId);
		if(m_actionMap.find(processedActionId) == m_actionMap.end())
			ActionFromString(m_actionMap[processedActionId], processedActionId);
		m_bindAction = &m_actionMap[KeybindString(p_actionId)];
	}
	// Used by menu or C++ code to unfocus keybinding
	void Keymapper::UnfocusEvent()
	{
		m_bindAction = nullptr;
	}
	void Keymapper::UnfocusBindAction()
	{
		m_bindAction = nullptr;
		// Make all the keybind inputs lose focus
		if(m_menu)
			m_menu->BufferJavascript("$('.controls-settings-keybind').blur();");
	}

	// Attempt to process a key assignment if a keybinding is being changed (has focus)
	//   otherwise do nothing and return false (to indicate the event should be processed further)
	bool Keymapper::ProcessKey(SDL_Event p_event)
	{
		if(m_bindAction == nullptr)
			return false;

		SDL_Scancode value = (SDL_Scancode)0;
		if(p_event.type == SDL_KEYDOWN)
			value = p_event.key.keysym.scancode;
		else if(p_event.type == SDL_MOUSEBUTTONDOWN)
			value = (SDL_Scancode)(p_event.button.button-SDL_BUTTON_LEFT+RootEngine::InputManager::MouseButton::LEFT);
		else
			return false;

		if(value == SDL_SCANCODE_ESCAPE || value == SDL_SCANCODE_RETURN)
		{
			UnfocusBindAction();
			return false;
		}

		m_bindAction->Bindings.clear();
		m_bindAction->Bindings.push_back(value);
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