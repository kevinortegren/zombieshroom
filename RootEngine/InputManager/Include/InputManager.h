#pragma once
#pragma once
#include "InputInterface.h"
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

			KeyState::KeyState GetKeyState(SDL_Scancode p_key);
			KeyState::KeyState GetKeyState(MouseButton::MouseButton p_button);

			glm::vec2 GetGlobalMousePos();
			glm::vec2 GetDeltaMousePos();

			static InputManager* GetInstance();
		private:
			KeyState::KeyState m_keyState[MAX_KEYS];

			glm::vec2 m_globMousePos;
			glm::vec2 m_deltaMousePos;

			static InputManager* s_inputSys;
		};
	}
}

