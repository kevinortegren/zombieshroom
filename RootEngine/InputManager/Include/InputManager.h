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
			void SetMousePos(const glm::ivec2& p_mousePos);


			KeyState::KeyState GetKeyState(SDL_Scancode p_key);
			KeyState::KeyState GetKeyState(MouseButton::MouseButton p_button);
			void LockInput(bool p_lock) { m_lockDownInput = p_lock; } 

			glm::ivec2 GetGlobalMousePos();
			glm::ivec2 GetDeltaMousePos();

			int GetScroll();

			void LockMouseToCenter(bool p_enable);

			static InputManager* GetInstance();
		private:
			KeyState::KeyState m_keyState[MAX_KEYS];

			glm::ivec2 m_globMousePos;
			glm::ivec2 m_deltaMousePos;
			bool m_lockMouseEnabled;
			bool m_lockDownInput;
			int m_scroll;
			static InputManager* s_inputSys;
		};
	}
}

