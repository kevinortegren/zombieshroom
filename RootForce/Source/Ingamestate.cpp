#include <RootForce/Include/IngameState.h>
#include <RootForce/Include/LuaAPI.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootEngine/GUI/Include/guiInstance.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{
	IngameState::IngameState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems)
		: m_networkContext(p_networkContext)
		, m_sharedSystems(p_sharedSystems)
	{	
		ComponentType::Initialize();

		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(10);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::HealthComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerControl>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Physics>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::NetworkComponent>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::LookAtBehavior>(20);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ThirdPersonBehavior>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Script>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Collision>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::CollisionResponder>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Animation>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::TDMRuleSet>(1);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerActionComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerPhysics>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::StateComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Shadowcaster>(1);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::DirectionalLight>(10);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ClientComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ServerInformationComponent>(1);

		m_hud = std::shared_ptr<RootForce::HUD>(new HUD());
	}

	void IngameState::Initialize()
	{
		//Bind c++ functions and members to Lua
		LuaAPI::RegisterLuaTypes(g_engineContext.m_script->GetLuaState());
		
		g_engineContext.m_resourceManager->LoadScript("Global");
		g_engineContext.m_resourceManager->LoadScript("AbilityBall");
		g_engineContext.m_resourceManager->LoadScript("AbilityDash");
		g_engineContext.m_resourceManager->LoadScript("MagicMissile");
		g_engineContext.m_resourceManager->LoadScript("Player");

		// Initialize the system for controlling the player.
		std::vector<RootForce::Keybinding> keybindings(6);
		keybindings[0].Bindings.push_back(SDL_SCANCODE_UP);
		keybindings[0].Bindings.push_back(SDL_SCANCODE_W);
		keybindings[0].Action = RootForce::PlayerAction::MOVE_FORWARDS;
		//keybindings[0].ActionUp = RootForce::PlayerAction::MOVE_FORWARDS_STOP;
		//keybindings[0].Edge = true;

		keybindings[1].Bindings.push_back(SDL_SCANCODE_DOWN);
		keybindings[1].Bindings.push_back(SDL_SCANCODE_S);
		keybindings[1].Action = RootForce::PlayerAction::MOVE_BACKWARDS;
		//keybindings[1].ActionUp = RootForce::PlayerAction::MOVE_BACKWARDS_STOP;
		//keybindings[1].Edge = true;

		keybindings[2].Bindings.push_back(SDL_SCANCODE_LEFT);
		keybindings[2].Bindings.push_back(SDL_SCANCODE_A);
		keybindings[2].Action = RootForce::PlayerAction::STRAFE_LEFT;
		//keybindings[2].ActionUp = RootForce::PlayerAction::STRAFE_LEFT_STOP;
		//keybindings[2].Edge = true;

		keybindings[3].Bindings.push_back(SDL_SCANCODE_RIGHT);
		keybindings[3].Bindings.push_back(SDL_SCANCODE_D);
		keybindings[3].Action = RootForce::PlayerAction::STRAFE_RIGHT;
		//keybindings[3].ActionUp = RootForce::PlayerAction::STRAFE_RIGHT_STOP;
		//keybindings[3].Edge = true;

		keybindings[4].Bindings.push_back(SDL_SCANCODE_SPACE);
		keybindings[4].Action = RootForce::PlayerAction::JUMP;
		//keybindings[4].Edge = true;
		
		keybindings[5].Bindings.push_back((SDL_Scancode)RootEngine::InputManager::MouseButton::LEFT);
		keybindings[5].Action = RootForce::PlayerAction::ACTIVATE_ABILITY;
		//keybindings[5].Edge = true;
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
		
		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(g_world));
		m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
		m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_playerControlSystem->SetKeybindings(keybindings);
		m_playerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

		// Initialize physics system
		m_physicsTransformCorrectionSystem = new RootForce::PhysicsTransformCorrectionSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::PhysicsTransformCorrectionSystem>(m_physicsTransformCorrectionSystem);

		m_physicsSystem = new RootForce::PhysicsSystem(g_world);
		m_physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
		m_physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
		g_world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem);

		m_collisionSystem = new RootForce::CollisionSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::CollisionSystem>(m_collisionSystem);

		// Initialize render, shadow and point light system.
		m_renderingSystem = new RootForce::RenderingSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem);

		m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

		m_shadowSystem = new RootForce::ShadowSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::ShadowSystem>(m_shadowSystem);

		m_pointLightSystem = new RootForce::PointLightSystem(g_world, g_engineContext.m_renderer);
		g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem);

		// Initialize anim system
		m_animationSystem = new RootForce::AnimationSystem(g_world);
		m_animationSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_animationSystem->SetGameSharedContext(&g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::AnimationSystem>(m_animationSystem);

		m_particleSystem = new RootForce::ParticleSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem);

		// Initialize camera systems.
		m_cameraSystem = new RootForce::CameraSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem);
		m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem);
		m_thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(m_thirdPersonBehaviorSystem);

		// Action system handles local and remote player issued actions and updates the world accordingly
		m_actionSystem = new RootSystems::ActionSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::ActionSystem>(m_actionSystem);

		// Respawn system respawns players after they die
		m_respawnSystem = new RootSystems::RespawnSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::RespawnSystem>(m_respawnSystem);

		// State system updates the current state of an entity for animation purposes
		m_stateSystem = new RootSystems::StateSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::StateSystem>(m_stateSystem);

		m_displayPhysicsDebug = false;
		m_displayNormals = false;
		m_displayWorldDebug = false;

		
	}

	void IngameState::Enter()
	{
		m_shadowSystem->SetAABB(m_sharedSystems.m_worldSystem->GetWorldAABB());

		// Lock the mouse
		g_engineContext.m_inputSys->LockMouseToCenter(true);

		// Setup the network
		m_networkContext.m_client->SetChatSystem(m_hud->GetChatSystem().get());
		m_networkContext.m_clientMessageHandler->SetChatSystem(m_hud->GetChatSystem().get());
		m_playerControlSystem->SetClientPeer(m_networkContext.m_client->GetPeerInterface());

		//Initialize the debug, setting the html view
		g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->LoadURL("debug.html"));

		//Init the hud and set one test ability for now
		m_hud->Initialize(g_engineContext.m_gui->LoadURL("hud.html"), g_engineContext.m_gui->GetDispatcher(), &g_engineContext);
		m_hud->SetSelectedAbility(0);

		//Reset the ingame menu before we start the match
		m_ingameMenu = std::shared_ptr<RootForce::IngameMenu>(new IngameMenu(g_engineContext.m_gui->LoadURL("ingameMenu.html"), g_engineContext.m_gui->GetDispatcher(), g_engineContext));
		m_displayIngameMenu = false;

		//Set the network context to the matchstatesystem
		m_sharedSystems.m_matchStateSystem->SetNetworkContext(&m_networkContext);

		//Load the level spawn points into the respawn system
		m_respawnSystem->LoadSpawnPoints();

		m_animationSystem->Start();
	}

	void IngameState::Exit()
	{
		m_animationSystem->Terminate();

		Network::NetworkEntityID id;
		id.UserID = Network::ReservedUserID::ALL;
		id.ActionID = Network::ReservedActionID::ALL;
		id.SequenceID = Network::ReservedSequenceID::ALL;

		Network::DeleteEntities(g_networkEntityMap, id, g_world->GetEntityManager());
		g_networkEntityMap.clear();
		Network::NetworkComponent::s_sequenceIDMap.clear();

		g_engineContext.m_gui->DestroyView(m_hud->GetView());
		g_engineContext.m_gui->DestroyView(g_engineContext.m_debugOverlay->GetView());

		g_engineContext.m_gui->DestroyView(m_ingameMenu->GetView());
		m_ingameMenu.reset();

		g_world->GetEntityManager()->RemoveAllEntitiesAndComponents();
		g_world->GetTagManager()->UnregisterAll();
		g_world->GetGroupManager()->UnregisterAll();
		g_engineContext.m_physics->RemoveAll();

		m_networkContext.m_client = nullptr;
		m_networkContext.m_clientMessageHandler = nullptr;
		m_networkContext.m_server = nullptr;
		m_networkContext.m_serverMessageHandler = nullptr;
	}

	GameStates::GameStates IngameState::Update(float p_deltaTime)
	{
		ECS::Entity* clientEntity = g_world->GetTagManager()->GetEntityByTag("Client");
		Network::ClientComponent* clientComponent = g_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);

		ECS::Entity* debugEntity = g_world->GetTagManager()->GetEntityByTag("LatestBall");
		Transform* debugTransform = nullptr;
		if (debugEntity != nullptr)
			debugTransform = g_world->GetEntityManager()->GetComponent<Transform>(debugEntity);

		// Check for quitting condition
		if (m_ingameMenu->GetExit())
		{
			return GameStates::Menu;
		}

		//Check status for the display of the ingame menu
		if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_ESCAPE) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_displayIngameMenu = true;
			g_engineContext.m_inputSys->LockMouseToCenter(false);
		}
		if (m_ingameMenu->GetReturn())
		{
			m_displayIngameMenu = false;
			g_engineContext.m_inputSys->LockMouseToCenter(true);
			m_ingameMenu->Reset();
		}

		// Check for disconnection from the server
		if (clientComponent->State == Network::ClientState::DISCONNECTED_SERVER_SHUTDOWN || clientComponent->State == Network::ClientState::DISCONNECTED_TIMEOUT)
		{
			return GameStates::Menu;
		}
		
		g_world->SetDelta(p_deltaTime);
		g_engineContext.m_renderer->Clear();
		
		if(m_sharedSystems.m_matchStateSystem->GetTimeLeft() <= -10)
		{
			// Do a reconnect, this should force the server to rehost the game (thus starting a new one) and the clients should reconnect automagically
			return GameStates::Connecting;
		}

		// If game is over, set gameover to true and wait for the restart
		bool isGameOver = false;
		if(m_sharedSystems.m_matchStateSystem->IsMatchOver())
		{
			isGameOver = true;
			m_hud->SetValue("EndGame", "true" );
			// If kill victory, set the time to 0 so restart will activate correctly
			if(m_sharedSystems.m_matchStateSystem->GetTimeLeft() > 0)
				g_world->GetEntityManager()->GetComponent<TDMRuleSet>( g_world->GetTagManager()->GetEntityByTag("MatchState") )->TimeLeft = 0;
		}
		if(isGameOver || g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_TAB) == RootEngine::InputManager::KeyState::DOWN)
		{
			m_hud->SetValue("ShowScore", "true" );
			m_hud->SetScoreList(m_sharedSystems.m_matchStateSystem->GetScoreList());
		}
		else if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_TAB) == RootEngine::InputManager::KeyState::UP)
			m_hud->SetValue("ShowScore", "false" );
		ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
		if(!isGameOver)
		{
			PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
			//Update all the data that is displayed in the HUD
			m_hud->SetValue("Health", std::to_string(g_world->GetEntityManager()->GetComponent<HealthComponent>(player)->Health) );
			m_hud->SetValue("PlayerScore", std::to_string(playerComponent->Score) );
			m_hud->SetValue("PlayerDeaths", std::to_string(playerComponent->Deaths) );
			m_hud->SetValue("TeamScore",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(playerComponent->TeamID == 2 ? 2 : 1)) ); //TODO: Fix so that we read the player team instead of hardcoding it
			m_hud->SetValue("EnemyScore",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(playerComponent->TeamID == 2 ? 1 : 2)) );
			m_hud->SetAbility(1, playerComponent->AbilityScripts[0].Name);
			m_hud->SetAbility(2,  playerComponent->AbilityScripts[1].Name);
			m_hud->SetAbility(3,  playerComponent->AbilityScripts[2].Name);
			if(playerComponent->AbilityScripts[0].Cooldown > 0)
				m_hud->StartCooldown(1, playerComponent->AbilityScripts[0].Cooldown);
			if(playerComponent->AbilityScripts[1].Cooldown > 0)
				m_hud->StartCooldown(2, playerComponent->AbilityScripts[1].Cooldown);
			if(playerComponent->AbilityScripts[2].Cooldown > 0)
				m_hud->StartCooldown(3, playerComponent->AbilityScripts[2].Cooldown);
			m_hud->SetSelectedAbility(g_world->GetEntityManager()->GetComponent<PlayerActionComponent>(player)->SelectedAbility);
		}
		m_hud->SetValue("TimeLeft", std::to_string((int)m_sharedSystems.m_matchStateSystem->GetTimeLeft()));
		m_hud->Update(); // Executes either the HUD update or ShowScore if the match is over
		RootServer::EventData event = m_hud->GetChatSystem()->PollEvent();

		switch (event.EventType)
		{
		case RootServer::UserCommands::CLIENT_RAGEQUIT:
			return GameStates::Menu;
		case RootServer::UserCommands::CLIENT_SUICIDE:
			{
				// ToDo: Send a network message to server to indicate a suicide
				g_world->GetEntityManager()->GetComponent<HealthComponent>(player)->Health = 0;
				MatchStateSystem::AwardPlayerKill(Network::ReservedUserID::NONE, g_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(player)->ID.UserID);
			} break;
		default:
			break;
		}

#ifdef _DEBUG
		
		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F10) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			if(m_displayWorldDebug)
			{
				m_displayWorldDebug = false;
				m_sharedSystems.m_worldSystem->ShowDebug(m_displayWorldDebug);	
			}
			else
			{
				m_displayWorldDebug = true;
				m_sharedSystems.m_worldSystem->ShowDebug(m_displayWorldDebug);	
			}
		}

		if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F11) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			if(m_displayPhysicsDebug)
			{
				m_displayPhysicsDebug = false;
				g_engineContext.m_physics->EnableDebugDraw(m_displayPhysicsDebug);
			}
			else
			{
				m_displayPhysicsDebug = true;
				g_engineContext.m_physics->EnableDebugDraw(m_displayPhysicsDebug);
			}
		}
#endif
		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F9) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			if(m_displayNormals)
			{
				m_displayNormals = false;
				g_engineContext.m_renderer->DisplayNormals(m_displayNormals);	
			}
			else
			{
				m_displayNormals = true;
				g_engineContext.m_renderer->DisplayNormals(m_displayNormals);	
			}
		}


		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F5) == RootEngine::InputManager::KeyState::DOWN_EDGE)
			g_engineContext.m_resourceManager->ReloadAllScripts();
		

		{
			PROFILE("Player control system", g_engineContext.m_profiler);

			g_engineContext.m_inputSys->LockInput(m_hud->GetChatSystem()->IsFocused());
			m_playerControlSystem->Process();
		}

		if (m_networkContext.m_server != nullptr)
		{
			PROFILE("Server", g_engineContext.m_profiler);
			m_networkContext.m_server->Update();
		}

		{
			PROFILE("Client", g_engineContext.m_profiler);
			m_networkContext.m_client->Update();
		}
		
		{
			PROFILE("Action system", g_engineContext.m_profiler);
			if(!m_displayIngameMenu)
				m_actionSystem->Process();
		}

		

		m_animationSystem->Run();
		
		{
			PROFILE("Respawn system", g_engineContext.m_profiler);
			m_respawnSystem->Process();
		}

		{
			PROFILE("Physics", g_engineContext.m_profiler);
			m_physicsTransformCorrectionSystem->Process();
			g_engineContext.m_physics->Update(p_deltaTime);
			m_physicsSystem->Process();
		}

		
		{
			PROFILE("Collision system", g_engineContext.m_profiler);
			m_collisionSystem->Process();
		}

		{
			PROFILE("StateSystem", g_engineContext.m_profiler);
			m_stateSystem->Process();
		}

		
		
		{
			PROFILE("Camera systems", g_engineContext.m_profiler);
			if(!m_displayIngameMenu)
				m_actionSystem->UpdateAimingDevice();
			m_thirdPersonBehaviorSystem->Process();
			m_lookAtSystem->Process();
			m_cameraSystem->Process();
		}
		
		{
			PROFILE("Shadow system", g_engineContext.m_profiler);
			m_shadowSystem->Process();
		}

		{ 
			PROFILE("ParticleSystem", g_engineContext.m_profiler);
			m_particleSystem->Process();
		}

		{
			PROFILE("World System", g_engineContext.m_profiler);
			m_sharedSystems.m_worldSystem->Process();
		}

		{
			PROFILE("RenderingSystem", g_engineContext.m_profiler);
			m_pointLightSystem->Process();
			m_renderingSystem->Process();

		}

		m_animationSystem->Synch();

		{
			PROFILE("Rendering", g_engineContext.m_profiler);
			g_engineContext.m_renderer->Render();
		}

		m_sharedSystems.m_matchStateSystem->UpdateDeltatime(p_deltaTime);
		m_sharedSystems.m_matchStateSystem->Process();
		
		g_engineContext.m_profiler->Update(p_deltaTime);
		g_engineContext.m_debugOverlay->RenderOverlay();
		{
			PROFILE("GUI", g_engineContext.m_profiler);

			g_engineContext.m_gui->Update();
			if (m_displayIngameMenu)
			{
				g_engineContext.m_gui->Render(m_ingameMenu->GetView());
				m_ingameMenu->GetView()->Focus();
			}
			else
			{
				g_engineContext.m_gui->Render(m_hud->GetView());
				g_engineContext.m_gui->Render(g_engineContext.m_debugOverlay->GetView());
			}
		}
		
		

		{
			PROFILE("Swap", g_engineContext.m_profiler);
			g_engineContext.m_renderer->Swap();
		}

		return GameStates::Ingame;
	}
}