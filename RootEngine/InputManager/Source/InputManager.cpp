#include "InputManager.h"

namespace RootEngine
{
	namespace InputManager
	{
		RootEngine::SubsystemSharedContext g_context;
		InputManager* InputManager::s_inputSys = nullptr;
		void InputManager::Startup(void)
		{
			m_lockDownInput = false;
			int err = SDL_SetRelativeMouseMode(SDL_TRUE);
			m_globMousePos = glm::ivec2(0, 0);
			m_deltaMousePos = glm::ivec2(0, 0);

			for(int i = 0; i < MAX_KEYS; i++)
				m_keyState[i] = KeyState::UP;

			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::INIT_PRINT, "InputManager subsystem initialized!");
		}


		void InputManager::Shutdown(void)
		{
			delete s_inputSys;
		}

		void InputManager::HandleInput(SDL_Event& p_event)
		{
			if(m_lockDownInput)
				return;
			switch(p_event.type)
			{
			case SDL_MOUSEWHEEL:
				m_scroll += p_event.wheel.y;
				break;
			case SDL_KEYDOWN:
				if(!p_event.key.repeat)
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
				//m_deltaMousePos.x = p_event.motion.x - m_globMousePos.x;
				//m_deltaMousePos.y = p_event.motion.y - m_globMousePos.y;
				if(SDL_GetRelativeMouseMode())
				{
					m_globMousePos.x = p_event.motion.x;
					m_globMousePos.y = p_event.motion.y;

					m_deltaMousePos.x = p_event.motion.xrel;
					m_deltaMousePos.y = p_event.motion.yrel;
				}
				else
				{
					m_globMousePos.x = -p_event.motion.x;
					m_globMousePos.y = -p_event.motion.y;

					m_deltaMousePos.x = -p_event.motion.xrel;
					m_deltaMousePos.y = -p_event.motion.yrel;
				}
				if(m_lockMouseEnabled)
				{
					
					//int w, h;
					//SDL_GetWindowSize(NULL, &w, &h);
					SDL_WarpMouseInWindow(NULL, 500, 400);
				}
				break;
			default:
				g_context.m_logger->LogText(LogTag::INPUT, LogLevel::MASS_DATA_PRINT, "Event %d did not match any case", p_event.type);
			}
		}

		KeyState::KeyState InputManager::GetKeyState(SDL_Scancode p_key)
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
			return GetKeyState((SDL_Scancode)p_button);
		}

		glm::ivec2 InputManager::GetGlobalMousePos()
		{
			if(g_context.m_configManager->GetConfigValueAsBool("settings-mouse-invert"))
				return glm::ivec2(m_globMousePos.x, -m_globMousePos.y);
			return m_globMousePos;
		}

		glm::ivec2 InputManager::GetDeltaMousePos()
		{
			glm::ivec2 temp = m_deltaMousePos;
			if(g_context.m_configManager->GetConfigValueAsBool("settings-mouse-invert"))
				temp.y = -temp.y;
			return temp;
		}

		InputManager* InputManager::GetInstance()
		{ 
			if(!s_inputSys)
				s_inputSys = new InputManager();

				return s_inputSys;
		}

		void InputManager::Reset()
		{
			m_deltaMousePos = glm::vec2(0, 0);
			m_scroll = 0;
		}

		

		void InputManager::LockMouseToCenter(bool p_enable)
		{
			m_lockMouseEnabled = p_enable;
			SDL_SetRelativeMouseMode((SDL_bool) p_enable);
			SDL_ShowCursor(!p_enable);
		}
		//Get scroll count, negative is scroll down (backwards) and positive is scroll up (forward)
		int InputManager::GetScroll()
		{
			return m_scroll;
		}

	}

}

RootEngine::InputManager::InputInterface* CreateInputSystem(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::InputManager::g_context = p_context;

	return RootEngine::InputManager::InputManager::GetInstance();
}


