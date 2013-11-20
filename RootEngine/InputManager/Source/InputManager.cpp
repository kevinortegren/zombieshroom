#include "InputManager.h"


InputManager::InputManager(void)
{
	m_globMousePos = vector2(0,0);
	m_deltaMousePos = vector2(0,0);

	for(int i = 0; i < MAX_KEYS; i++)
		m_keyState[i] = KeyState::UP;
}


InputManager::~InputManager(void)
{
}

void InputManager::HandleInput(SDL_Event& p_event)
{
	switch(p_event.type)
	{
	case SDL_KEYDOWN:
		m_keyState[p_event.key.keysym.scancode] = KeyState::DOWN_EDGE;
		break;
	case SDL_KEYUP:
		m_keyState[p_event.key.keysym.scancode] = KeyState::UP_EDGE;
		break;
	case SDL_MOUSEBUTTONDOWN:
		m_keyState[p_event.button.button-SDL_BUTTON_LEFT+MouseButton::LEFT] = KeyState::DOWN_EDGE;
		break;
	case SDL_MOUSEBUTTONUP:
		m_keyState[p_event.button.button-SDL_BUTTON_LEFT+MouseButton::LEFT] = KeyState::UP_EDGE;
		break;
	case SDL_MOUSEMOTION:
		m_deltaMousePos.x = m_globMousePos.x - p_event.motion.x;
		m_deltaMousePos.y = m_globMousePos.y - p_event.motion.y;
		m_globMousePos.x = p_event.motion.x;
		m_globMousePos.y = p_event.motion.y;
		break;
	}
}

KeyState::KeyState InputManager::GetKeyState(SDL_Keycode p_key)
{
	if(m_keyState[p_key] == KeyState::DOWN_EDGE)
	{
		m_keyState[p_key] = KeyState::DOWN;
		return KeyState::DOWN_EDGE;
	}
	if(m_keyState[p_key] == KeyState::UP_EDGE)
	{
		m_keyState[p_key] = KeyState::UP;
		return KeyState::UP_EDGE;
	}
	return m_keyState[p_key];
}

KeyState::KeyState InputManager::GetKeyState(MouseButton::MouseButton p_button)
{
	return GetKeyState((SDL_Keycode)p_button);
}

vector2 InputManager::GetGlobalMousePos()
{
	return m_globMousePos;
}

vector2 InputManager::GetDeltaMousePos()
{
	return m_deltaMousePos;
}

InputInterface* APIENTRY GetInputInterface() { static InputManager s_im; return &s_im; }
