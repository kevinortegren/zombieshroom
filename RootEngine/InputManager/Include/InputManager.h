#pragma once
#pragma once
#include "InputInterface.h"
#include <SDL2/SDL.h>

#define MAX_KEYS 512

class InputManager: public InputInterface
{
public:
	InputManager(void);
	~InputManager(void);
	void HandleInput(SDL_Event& p_event);

	KeyState::KeyState GetKeyState(SDL_Keycode p_key);
	KeyState::KeyState GetKeyState(MouseButton::MouseButton p_button);

	vector2 GetGlobalMousePos();
	vector2 GetDeltaMousePos();
private:
	KeyState::KeyState m_keyState[MAX_KEYS];

	vector2 m_globMousePos;
	vector2 m_deltaMousePos;

};

