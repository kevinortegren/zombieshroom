#include <UnitTesting.h>

TEST(INPUT, KEYTEST)
{
	RootEngine::InputManager::InputInterface* ii = g_engineContext.m_inputSys;

	SDL_Scancode randkey = (SDL_Scancode) (rand() % 512);
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
	RootEngine::InputManager::InputInterface* ii = g_engineContext.m_inputSys;

	SDL_Event falseevent;
	glm::ivec2 tmp = ii->GetDeltaMousePos();
	EXPECT_EQ(tmp.x, 0);
	EXPECT_EQ(tmp.y, 0);
	falseevent.type = SDL_MOUSEMOTION;
	falseevent.motion.x = 5;
	falseevent.motion.y = 140;
	falseevent.motion.xrel = 5;
	falseevent.motion.yrel = 140;
	ii->HandleInput(falseevent);
	tmp = ii->GetGlobalMousePos();
	EXPECT_EQ(tmp.x, -5);
	EXPECT_EQ(tmp.y, -140);
	tmp = ii->GetDeltaMousePos();
	EXPECT_EQ(tmp.x, -5);
	EXPECT_EQ(tmp.y, -140);

	falseevent.motion.x = 100;
	falseevent.motion.y = 0;
	falseevent.motion.xrel = 95;
	falseevent.motion.yrel = -140;
	ii->HandleInput(falseevent);
	tmp = ii->GetGlobalMousePos();
	EXPECT_EQ(tmp.x, -100);
	EXPECT_EQ(tmp.y, 0);
	falseevent.motion.y = 20;
	falseevent.motion.xrel = 0;
	falseevent.motion.yrel = 20;
	ii->HandleInput(falseevent);
	tmp = ii->GetDeltaMousePos();
	EXPECT_EQ(tmp.x, 0);
	EXPECT_EQ(tmp.y, -20);
}