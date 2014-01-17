#pragma once
#pragma once
#include "InputInterface.h"
#include <WinSock2.h>
#include <SDL2/SDL.h>

#define MAX_KEYS SDL_NUM_SCANCODES

namespace RootEngine
{
	namespace InputManager
	{
		class InputManager: public InputInterface
		{
		public:
			void Startup(void);
			void Shutdown(void);
			void HandleInput(SDL_Event& p_event);
			void Reset();

			KeyState::KeyState GetKeyState(SDL_Scancode p_key);
			KeyState::KeyState GetKeyState(MouseButton::MouseButton p_button);
			void LockInput(bool p_lock) { m_lockDownInput = p_lock; } 

			glm::ivec2 GetGlobalMousePos();
			glm::ivec2 GetDeltaMousePos();

			void LockMouseToCenter(bool p_enable);

			static InputManager* GetInstance();
		private:

			KeyState::KeyState m_keyState[MAX_KEYS];

			glm::ivec2 m_globMousePos;
			glm::ivec2 m_deltaMousePos;
			bool m_lockMouseEnabled;
			bool m_lockDownInput;

			static InputManager* s_inputSys;
		};
	}
}

