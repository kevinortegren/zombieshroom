#include <RootForce/Include/IngameState.h>
#include <RootForce/Include/LuaAPI.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootEngine/GUI/Include/guiInstance.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{
	IngameState::IngameState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems, Keymapper* p_keymapper)
		: m_networkContext(p_networkContext)
		, m_sharedSystems(p_sharedSystems)
		, m_keymapper(p_keymapper)
	{	
		ComponentType::Initialize();
		
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(1000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(1000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(1000);
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
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Ragdoll>(100);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::WaterCollider>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::AbilityRespawnComponent>(100);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::TryPickupComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::SoundComponent>(100000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::TimerComponent>(100000);

		m_hud = std::shared_ptr<RootForce::HUD>(new HUD());
	}

	void IngameState::Initialize()
	{
		// Bind c++ functions and members to Lua.
		LuaAPI::RegisterLuaTypes(g_engineContext.m_script->GetLuaState());
		
		g_engineContext.m_resourceManager->LoadScript("Global");
		g_engineContext.m_resourceManager->LoadScript("Push");
		g_engineContext.m_resourceManager->LoadScript("AbilityBall");
		g_engineContext.m_resourceManager->LoadScript("FireBall");
		g_engineContext.m_resourceManager->LoadScript("AbilityDash");
		g_engineContext.m_resourceManager->LoadScript("AbilityTest");
		g_engineContext.m_resourceManager->LoadScript("AbilityRay");
		//g_engineContext.m_resourceManager->LoadScript("CompileChecker");
		g_engineContext.m_resourceManager->LoadScript("Player");
		g_engineContext.m_resourceManager->LoadScript("Explosion");
		g_engineContext.m_resourceManager->LoadScript("AbilitySpawnPoint");
		
		// Initialize the player control system.
		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(g_world));
		m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
		m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_playerControlSystem->SetKeybindings(m_keymapper->GetKeybindings());
		m_playerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

		// Initialize physics systems.
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

		m_pointLightSystem = new RootForce::PointLightSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem);

		m_directionlLightSystem = new RootForce::DirectionalLightSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::DirectionalLightSystem>(m_directionlLightSystem);

		// Initialize anim system.
		m_animationSystem = new RootForce::AnimationSystem(g_world);
		m_animationSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_animationSystem->SetGameSharedContext(&g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::AnimationSystem>(m_animationSystem);

		m_particleSystem = new RootForce::ParticleSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem);

		//Initialize Ragdoll system.
		m_ragdollSystem = new RootForce::RagdollSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::RagdollSystem>(m_ragdollSystem);

		// Initialize camera systems.
		m_cameraSystem = new RootForce::CameraSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem);
		m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem);
		m_thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(m_thirdPersonBehaviorSystem);

		// Action system handles local and remote player issued actions and updates the world accordingly.
		m_actionSystem = new RootSystems::ActionSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::ActionSystem>(m_actionSystem);

		// Respawn system respawns players after they die
		m_sharedSystems.m_respawnSystem = new RootSystems::RespawnSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootSystems::RespawnSystem>(m_sharedSystems.m_respawnSystem);


		// State system updates the current state of an entity for animation purposes.
		m_stateSystem = new RootSystems::StateSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::StateSystem>(m_stateSystem);

		// Initialize the water system.
		m_waterSystem = new RootForce::WaterSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::WaterSystem>(m_waterSystem);

		// Initialize the system for resetting the TryPickupComponent
		m_tryPickupResetSystem = new TryPickupResetSystem(g_world);
		g_world->GetSystemManager()->AddSystem<TryPickupResetSystem>(m_tryPickupResetSystem);

		// Initialize the sound system.
		m_soundSystem = new RootForce::SoundSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::SoundSystem>(m_soundSystem);

		m_timerSystem = new RootForce::TimerSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::TimerSystem>(m_timerSystem);


		// Set debug visualization flags.
		m_displayPhysicsDebug = false;
		m_displayNormals = false;
		m_displayWorldDebug = false;
		m_displayDebugHUD = true;
		m_displayGuiHUD = true;
	}

	void IngameState::Enter()
	{
		m_shadowSystem->SetQuadTree(m_sharedSystems.m_worldSystem->GetQuadTree());

		// Lock the mouse
		g_engineContext.m_inputSys->LockMouseToCenter(true);

		// Setup the network
		m_networkContext.m_client->SetChatSystem(m_hud->GetChatSystem().get());
		m_networkContext.m_clientMessageHandler->SetChatSystem(m_hud->GetChatSystem().get());
		
		// Set network peer interfaces on the systems that needs to send messages.
		m_playerControlSystem->SetClientPeer(m_networkContext.m_client->GetPeerInterface());
		m_actionSystem->SetClientPeerInterface(m_networkContext.m_client->GetPeerInterface());
		m_sharedSystems.m_matchStateSystem->SetNetworkContext(&m_networkContext);
		m_sharedSystems.m_abilitySpawnSystem->SetClientPeerInterface(m_networkContext.m_client->GetPeerInterface());

		// Set the server peer to the action and abilityspawn system, if we are a server.
		if (m_networkContext.m_server != nullptr)
		{
			m_actionSystem->SetServerPeerInterface(m_networkContext.m_server->GetPeerInterface());
			m_sharedSystems.m_abilitySpawnSystem->SetServerPeerInterface(m_networkContext.m_server->GetPeerInterface());
		}	

		// Initialize the debug, setting the html view
		g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->LoadURL("Debug", "debug.html"));

		// Init the hud and set one test ability for now
		m_hud->Initialize(g_engineContext.m_gui->LoadURL("HUD", "hud.html"), &g_engineContext);
		m_hud->SetSelectedAbility(0);

		// Reset the ingame menu before we start the match
		m_ingameMenu = std::shared_ptr<RootForce::IngameMenu>(new IngameMenu(g_engineContext.m_gui->LoadURL("Menu", "ingameMenu.html"), g_engineContext, m_keymapper));
		m_displayIngameMenu = false;
		
		m_animationSystem->Start();

		m_waterSystem->CreateWater(g_world->GetStorage()->GetValueAsFloat("WaterHeight"));

		if(m_networkContext.m_server != nullptr)
			m_timerSystem->SetServerPeer(m_networkContext.m_server->GetPeerInterface());

		m_playerControlSystem->SetKeybindings(m_keymapper->GetKeybindings());
	}

	void IngameState::Exit()
	{
		// Make sure the user is not stuck keymapping in case of disconnects or other abnormal termination
		m_keymapper->UnfocusBindAction();

		m_animationSystem->Terminate();

		// Remove all networked entities and reset the entity map and sequence map.
		Network::DeleteEntities(g_networkEntityMap, Network::NetworkEntityID(Network::ReservedUserID::ALL, Network::ReservedActionID::ALL, Network::ReservedSequenceID::ALL), g_world->GetEntityManager());
		g_networkEntityMap.clear();
		Network::NetworkComponent::s_sequenceIDMap.clear();

		// Destroy the ingame GUI views.
		g_engineContext.m_gui->DestroyView(m_hud->GetView());
		g_engineContext.m_gui->DestroyView(g_engineContext.m_debugOverlay->GetView());
		g_engineContext.m_gui->DestroyView(m_ingameMenu->GetView());

		// Remove all entities.
		g_world->GetEntityManager()->RemoveAllEntitiesAndComponents();
		g_world->GetTagManager()->UnregisterAll();
		g_world->GetGroupManager()->UnregisterAll();
		g_engineContext.m_physics->RemoveAll();

		// Set server peers to null
		m_sharedSystems.m_abilitySpawnSystem->SetServerPeerInterface(nullptr);

		// Disable the message handlers while resetting the server (to avoid null entities etc.)
		if(m_networkContext.m_server != nullptr)
			m_networkContext.m_server->SetMessageHandler(nullptr);
		m_networkContext.m_client->SetMessageHandler(nullptr);
	}

	GameStates::GameStates IngameState::Update(float p_deltaTime)
	{				
		g_world->SetDelta(p_deltaTime);
		g_engineContext.m_renderer->Clear();
		g_engineContext.m_renderer->Render();

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

				//Update Menu to make sure Setting changes are made in the main thread
				m_ingameMenu->Update();
			}
			else
			{
				if(m_displayGuiHUD)
					g_engineContext.m_gui->Render(m_hud->GetView());
				if(m_displayDebugHUD)
					g_engineContext.m_gui->Render(g_engineContext.m_debugOverlay->GetView());
			}
		}


		ECS::Entity* clientEntity = g_world->GetTagManager()->GetEntityByTag("Client");
		Network::ClientComponent* clientComponent = g_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);

		ECS::Entity* debugEntity = g_world->GetTagManager()->GetEntityByTag("LatestBall");
		Transform* debugTransform = nullptr;
		if (debugEntity != nullptr)
			debugTransform = g_world->GetEntityManager()->GetComponent<Transform>(debugEntity);

		// Check for disconnection from the server
		if (clientComponent->State == Network::ClientState::DISCONNECTED_SERVER_SHUTDOWN || clientComponent->State == Network::ClientState::DISCONNECTED_TIMEOUT)
		{
			return GameStates::Menu;
		}
		
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

		if(RootServer::MatchAny(event.EventType, 2, "SORTED", "SO"))
		{
			g_engineContext.m_profiler->ToggleSorted();
		}
		if(RootServer::MatchAny(event.EventType, 2, "PROFILER", "PR"))
		{
			m_displayDebugHUD = m_displayDebugHUD ? false : true;
		}
		if(RootServer::MatchAny(event.EventType, 2, "PHYSICSDEBUG", "PD"))
		{
			m_displayPhysicsDebug = m_displayPhysicsDebug ? false : true;
			g_engineContext.m_physics->EnableDebugDraw(m_displayPhysicsDebug);
		}
		if(RootServer::MatchAny(event.EventType, 2, "NORMALSDEBUG", "ND"))
		{
			m_displayNormals = m_displayNormals ? false : true;
			g_engineContext.m_renderer->DisplayNormals(m_displayNormals);
		}
		if(RootServer::MatchAny(event.EventType, 2, "W", "WATER"))
		{
			m_waterSystem->ParseCommands(m_hud->GetChatSystem().get(), &event.Data);
		}
		if(RootServer::MatchAny(event.EventType, 2, "RS", "RELOADSCRIPTS"))
		{
			g_engineContext.m_resourceManager->ReloadAllScripts();
		}
		if(RootServer::MatchAny(event.EventType, 3, "Q", "QUIT", "RAGEQUIT"))
			return GameStates::Menu;
		else if(RootServer::MatchAny(event.EventType, 2, "KILL","SUICIDE"))
		{
			// Kill ourselves.
			g_world->GetEntityManager()->GetComponent<HealthComponent>(player)->Health = 0;
			MatchStateSystem::AwardPlayerKill(Network::ReservedUserID::NONE, g_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(player)->ID.UserID);

			// Notify the server of our suicide.
			NetworkMessage::Suicide m;
			m.User = Network::ReservedUserID::NONE;

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID) ID_TIMESTAMP);
			bs.Write(RakNet::GetTime());
			bs.Write((RakNet::MessageID) NetworkMessage::MessageType::Suicide);
			m.Serialize(true, &bs);

			m_networkContext.m_client->GetPeerInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}

		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F12) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_displayGuiHUD = m_displayGuiHUD ? false : true;
		}

#ifdef _DEBUG
#endif
		
		{
			PROFILE("Water system", g_engineContext.m_profiler);
			m_waterSystem->Process();
		}

		{
			PROFILE("Try-Pickup reset system", g_engineContext.m_profiler);
			m_tryPickupResetSystem->Process();
		}
		
		{
			PROFILE("Player control system", g_engineContext.m_profiler);

			g_engineContext.m_inputSys->LockInput(m_hud->GetChatSystem()->IsFocused() || m_displayIngameMenu);
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
			m_actionSystem->Process();
		}

		{
			PROFILE("Timer system", g_engineContext.m_profiler);
			m_timerSystem->Process();
		}

		m_animationSystem->Run();
		
		{
			PROFILE("Respawn system", g_engineContext.m_profiler);
			m_sharedSystems.m_respawnSystem->Process();
		}

		{
			PROFILE("AbilitySpawn system", g_engineContext.m_profiler);
			m_sharedSystems.m_abilitySpawnSystem->Process();
		}

		{
			PROFILE("Ragdoll system", g_engineContext.m_profiler);
			m_ragdollSystem->Process();
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
			m_actionSystem->UpdateAimingDevice(m_displayIngameMenu);
			m_thirdPersonBehaviorSystem->Process();
			m_lookAtSystem->Process();
			m_cameraSystem->Process();
		}
		{
			PROFILE("Sound System", g_engineContext.m_profiler);
			m_soundSystem->Process();
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
			m_directionlLightSystem->Process();
			m_pointLightSystem->Process();
			m_renderingSystem->Process();
		}

		m_animationSystem->Synch();


		{
			PROFILE("Swap", g_engineContext.m_profiler);
			g_engineContext.m_renderer->Swap();
		}	

		// Check for quitting condition
		if (m_ingameMenu->GetExit())
		{
			return GameStates::Menu;
		}

		//Check status for the display of the ingame menu
		if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_ESCAPE) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_displayIngameMenu = !m_displayIngameMenu;
			g_engineContext.m_inputSys->LockMouseToCenter(!m_displayIngameMenu);
			m_ingameMenu->Reset();
		}
		if (m_ingameMenu->GetReturn())
		{
			m_displayIngameMenu = false;
			g_engineContext.m_inputSys->LockMouseToCenter(true);
			m_ingameMenu->Reset();
			// Update keybindings when returning to game
			m_playerControlSystem->SetKeybindings(m_keymapper->GetKeybindings());
		}
		g_engineContext.m_sound->Update();
		return GameStates::Ingame;
	}
}