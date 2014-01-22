#include "UnitTesting.h"
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/PlayerSystem.h>

TEST(PlayerControlSystem, Process)
{
	ECS::World* world = CreateWorld();
	g_world = world;

	// Initialize the keybindings we need for the test(not the complete list that is normally used)
	std::vector<RootForce::Keybinding> keybindings(6);
	keybindings[0].Bindings.push_back(SDL_SCANCODE_UP);
	keybindings[0].Bindings.push_back(SDL_SCANCODE_W);
	keybindings[0].Action = RootForce::PlayerAction::MOVE_FORWARDS;
	keybindings[0].ActionUp = RootForce::PlayerAction::MOVE_FORWARDS_STOP;
	keybindings[0].Edge = true;

	keybindings[1].Bindings.push_back(SDL_SCANCODE_DOWN);
	keybindings[1].Bindings.push_back(SDL_SCANCODE_S);
	keybindings[1].Action = RootForce::PlayerAction::MOVE_BACKWARDS;
	keybindings[1].ActionUp = RootForce::PlayerAction::MOVE_BACKWARDS_STOP;
	keybindings[1].Edge = true;

	keybindings[4].Bindings.push_back(SDL_SCANCODE_SPACE);
	keybindings[4].Action = RootForce::PlayerAction::JUMP;
	keybindings[4].Edge = true;

	keybindings[5].Bindings.push_back((SDL_Scancode)RootEngine::InputManager::MouseButton::LEFT);
	keybindings[5].Action = RootForce::PlayerAction::ACTIVATE_ABILITY;
	keybindings[5].Edge = true;
	keybindings.push_back(RootForce::Keybinding());
	keybindings[keybindings.size()-1].Bindings.push_back(SDL_SCANCODE_1);
	keybindings[keybindings.size()-1].Action = RootForce::PlayerAction::SELECT_ABILITY1;
	keybindings[keybindings.size()-1].Edge = true;
	keybindings.push_back(RootForce::Keybinding());
	keybindings[keybindings.size()-1].Bindings.push_back(SDL_SCANCODE_2);
	keybindings[keybindings.size()-1].Action = RootForce::PlayerAction::SELECT_ABILITY2;
	keybindings[keybindings.size()-1].Edge = true;
	keybindings.push_back(RootForce::Keybinding());
	keybindings[keybindings.size()-1].Bindings.push_back(SDL_SCANCODE_3);
	keybindings[keybindings.size()-1].Action = RootForce::PlayerAction::SELECT_ABILITY3;
	keybindings[keybindings.size()-1].Edge = true;


	RootForce::PlayerControlSystem* system = new RootForce::PlayerControlSystem(world);
	system->SetInputInterface(g_engineContext.m_inputSys);
	system->SetLoggingInterface(g_engineContext.m_logger);
	system->SetPhysicsInterface(g_engineContext.m_physics);
	system->SetKeybindings(keybindings);
	
	ECS::Entity* testity = world->GetEntityManager()->CreateEntity();

	// Call the OnCreate script
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "OnCreate");
	g_engineContext.m_script->AddParameterUserData(testity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->AddParameterNumber(0);
	g_engineContext.m_script->ExecuteScript();

	// Add client components onto the entity
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "AddClientComponents");
	g_engineContext.m_script->AddParameterUserData(testity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->ExecuteScript();
	
	// Add client components onto the entity
	g_engineContext.m_script->SetFunction(g_engineContext.m_resourceManager->LoadScript("Player"), "AddLocalClientComponents");
	g_engineContext.m_script->AddParameterUserData(testity, sizeof(ECS::Entity*), "Entity");
	g_engineContext.m_script->ExecuteScript();

	RootEngine::InputManager::InputInterface* ii = g_engineContext.m_inputSys;

	RootForce::PlayerActionComponent* action = world->GetEntityManager()->GetComponent<RootForce::PlayerActionComponent>(testity);

	//Fake event to test different keys
	SDL_Event falseevent;
	falseevent.type = SDL_KEYDOWN;
	falseevent.key.repeat = false;

	//Movement test
	{
		
		falseevent.key.keysym.scancode = SDL_SCANCODE_UP;
		ii->HandleInput(falseevent);

		system->Process();

		//Pushing just forward should set movepower to 1
		EXPECT_EQ(action->MovePower, 1);

		falseevent.key.keysym.scancode = SDL_SCANCODE_DOWN;
		ii->HandleInput(falseevent);

		system->Process();

		//Pushing back and forth at the same time should cancel each other out
		EXPECT_EQ(action->MovePower, 0);

		
		falseevent.key.keysym.scancode = SDL_SCANCODE_UP;
		falseevent.type = SDL_KEYUP;
		ii->HandleInput(falseevent);

		system->Process();

		//We have now released the up key and should be moving backwards
		EXPECT_EQ(action->MovePower, -1);
	}


	//Jump test
	{
		falseevent.key.keysym.scancode = SDL_SCANCODE_SPACE;
		falseevent.type = SDL_KEYDOWN;

		ii->HandleInput(falseevent);

		system->Process();

		EXPECT_TRUE(action->Jump);
	}

	//Orientation test
	{
		SDL_Event falseMouseEvent;
		falseMouseEvent.type = SDL_MOUSEMOTION;
		falseMouseEvent.motion.x = 5;
		falseMouseEvent.motion.y = 140;
		falseMouseEvent.motion.xrel = 5;
		falseMouseEvent.motion.yrel = 140;
		ii->HandleInput(falseMouseEvent);

		system->Process();

		EXPECT_LT(action->Angle.y, 0);
		EXPECT_GT(action->Angle.x, 0);
	}

	world->GetEntityManager()->RemoveAllEntitiesAndComponents();
	world->GetTagManager()->UnregisterAll();
	world->GetGroupManager()->UnregisterAll();
	g_engineContext.m_physics->RemoveAll();
	delete world;
}