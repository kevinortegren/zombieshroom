#include "InputManager.h"
#include <gtest/gtest.h>
namespace RootEngine
{
	namespace InputManager
	{
		RootEngine::SubsystemSharedContext g_context;
		InputManager* InputManager::s_inputSys = nullptr;
		void InputManager::Startup(void)
		{
			m_globMousePos = glm::vec2(0,0);
			m_deltaMousePos = glm::vec2(0,0);

			for(int i = 0; i < MAX_KEYS; i++)
				m_keyState[i] = KeyState::UP;
		}


		void InputManager::Shutdown(void)
		{
			delete s_inputSys;
		}

		void InputManager::HandleInput(SDL_Event& p_event)
		{
			switch(p_event.type)
			{
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
				m_deltaMousePos.x += m_globMousePos.x - p_event.motion.x;
				m_deltaMousePos.y += m_globMousePos.y - p_event.motion.y;
				m_globMousePos.x = static_cast<float>(p_event.motion.x);
				m_globMousePos.y = static_cast<float>(p_event.motion.y);
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

		glm::vec2 InputManager::GetGlobalMousePos()
		{
			return m_globMousePos;
		}

		glm::vec2 InputManager::GetDeltaMousePos()
		{
			glm::vec2 temp = m_deltaMousePos;
			m_deltaMousePos = glm::vec2(0,0);
			return temp;
		}

		InputManager* InputManager::GetInstance()
		{ 
			if(!s_inputSys)
				s_inputSys = new InputManager();

				return s_inputSys;
		}

		RootEngine::InputManager::InputInterface* CreateInputSystem(RootEngine::SubsystemSharedContext p_context)
		{
			g_context = p_context;

			return RootEngine::InputManager::InputManager::GetInstance();
		}
	}

}

TEST(INPUT, KEYTEST)
{
	RootEngine::InputManager::InputInterface* ii = RootEngine::InputManager::InputManager::GetInstance();
	
	int randkey = rand()%512;
	EXPECT_TRUE(ii->GetKeyState( randkey ) == RootEngine::InputManager::KeyState::UP);
	SDL_Event falseevent;
	falseevent.type = SDL_KEYDOWN;
	falseevent.key.repeat = false;
	falseevent.key.keysym.scancode = (SDL_Scancode)randkey;
	ii->HandleInput(falseevent);
	EXPECT_TRUE(ii->GetKeyState( randkey ) == RootEngine::InputManager::KeyState::DOWN_EDGE);
	EXPECT_TRUE(ii->GetKeyState( randkey ) == RootEngine::InputManager::KeyState::DOWN);
	falseevent.type = SDL_KEYUP;
	ii->HandleInput(falseevent);
	EXPECT_TRUE(ii->GetKeyState( randkey ) == RootEngine::InputManager::KeyState::UP_EDGE);
	EXPECT_TRUE(ii->GetKeyState( randkey ) == RootEngine::InputManager::KeyState::UP);
	falseevent.type = SDL_KEYDOWN;
	ii->HandleInput(falseevent);
	falseevent.type = SDL_KEYUP;
	ii->HandleInput(falseevent);
	EXPECT_TRUE(ii->GetKeyState( randkey ) == RootEngine::InputManager::KeyState::UP_EDGE);
	EXPECT_TRUE(ii->GetKeyState( randkey ) == RootEngine::InputManager::KeyState::UP);
}

TEST(INPUT, MOUSETEST)
{
	RootEngine::InputManager::InputInterface* ii = RootEngine::InputManager::InputManager::GetInstance();

	SDL_Event falseevent;
	EXPECT_TRUE(ii->GetDeltaMousePos() == glm::vec2(0,0));
	falseevent.type = SDL_MOUSEMOTION;
	falseevent.motion.x = 5;
	falseevent.motion.y = 140;
	ii->HandleInput(falseevent);
	EXPECT_TRUE(ii->GetGlobalMousePos() == glm::vec2(5,140));
	EXPECT_TRUE(ii->GetDeltaMousePos() == glm::vec2(5,140));
	EXPECT_TRUE(ii->GetDeltaMousePos() == glm::vec2(0,0));

	falseevent.motion.x = 100;
	falseevent.motion.y = 0;
	ii->HandleInput(falseevent);
	EXPECT_TRUE(ii->GetGlobalMousePos() == glm::vec2(100,0));
	falseevent.motion.y = 20;
	ii->HandleInput(falseevent);
	EXPECT_TRUE(ii->GetDeltaMousePos() == glm::vec2(95,-120));
}
