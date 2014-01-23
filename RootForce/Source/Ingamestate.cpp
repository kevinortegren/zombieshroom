#include <RootForce/Include/IngameState.h>
#include <RootForce/Include/LuaAPI.h>

extern RootEngine::GameSharedContext g_engineContext;
extern ECS::World* g_world;

namespace RootForce
{
	IngameState::IngameState(NetworkContext& p_networkContext, SharedSystems& p_sharedSystems)
		: m_networkContext(p_networkContext)
		, m_sharedSystems(p_sharedSystems)
	{	
		ComponentType::Initialize();

		m_hud = std::shared_ptr<RootForce::HUD>(new HUD());
	}

	void IngameState::Initialize()
	{
		//Bind c++ functions and members to Lua
		LuaAPI::RegisterLuaTypes(g_engineContext.m_script->GetLuaState());
		
		g_engineContext.m_resourceManager->LoadScript("Global");
		g_engineContext.m_resourceManager->LoadScript("AbilityTest");
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
		g_world->GetSystemManager()->AddSystem<RootForce::PhysicsTransformCorrectionSystem>(m_physicsTransformCorrectionSystem, "PhysicsTransformCorrectionSystem");

		m_physicsSystem = new RootForce::PhysicsSystem(g_world);
		m_physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
		m_physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
		g_world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem, "PhysicsSystem");

		m_collisionSystem = new RootForce::CollisionSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::CollisionSystem>(m_collisionSystem, "CollisionSystem");

		// Initialize render and point light system.
		m_renderingSystem = new RootForce::RenderingSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem, "RenderingSystem");

		m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

		m_pointLightSystem = new RootForce::PointLightSystem(g_world, g_engineContext.m_renderer);
		g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem, "PointLightSystem");

		// Initialize anim system
		m_animationSystem = new RootForce::AnimationSystem(g_world);
		m_animationSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_animationSystem->SetGameSharedContext(&g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::AnimationSystem>(m_animationSystem, "AnimationSystem");

		m_particleSystem = new RootForce::ParticleSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem, "ParticleSystem");

		// Initialize camera systems.
		m_cameraSystem = new RootForce::CameraSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem, "CameraSystem");
		m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem, "LookAtSystem");
		m_thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(m_thirdPersonBehaviorSystem, "ThirdPersonBehaviorSystem");

		// Action system handles local and remote player issued actions and updates the world accordingly
		m_actionSystem = new RootSystems::ActionSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::ActionSystem>(m_actionSystem, "ActionSystem");

		// Respawn system respawns players after they die
		m_respawnSystem = new RootSystems::RespawnSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::RespawnSystem>(m_respawnSystem, "RespawnSystem");

		// State system updates the current state of an entity for animation purposes
		m_stateSystem = new RootSystems::StateSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootSystems::StateSystem>(m_stateSystem, "StateSystem");


		m_displayPhysicsDebug = false;
		m_displayNormals = false;
		m_displayWorldDebug = false;
	}

	void IngameState::Enter()
	{
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
		m_hud->SetAbility(1, "TestDash");
		m_hud->SetSelectedAbility(0);

		//Set the network context to the matchstatesystem
		m_sharedSystems.m_matchStateSystem->SetNetworkContext(&m_networkContext);

		/* TEMP FROM HERE: */
		
		// Test positions:
		glm::vec3 pos[5] = { 
			glm::vec3(0,0,0),
			glm::vec3(0,0,-8),
			glm::vec3(0,0,8),
			glm::vec3(8,0,0),
			glm::vec3(-8,0,0)
		};

		for(int i = 0; i < 5; i++)
		{
			//Create particle entity
			ECS::Entity* p = g_world->GetEntityManager()->CreateEntity();
			//Add transform
			RootForce::Transform* t = g_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
			t->m_position = pos[i];
			//Add Particle Emitter
			RootForce::ParticleEmitter* e = g_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);	
			//Add particle system to emitter
			RootForce::ParticleSystemStruct particleSystem;
			e->m_particleSystems.push_back(particleSystem);

			//Set Particle system values
			e->m_particleSystems[e->m_particleSystems.size()-1].m_system = g_engineContext.m_renderer->CreateParticleSystem();	
			e->m_particleSystems[e->m_particleSystems.size()-1].m_material = g_engineContext.m_resourceManager->GetMaterial("particle");
			e->m_particleSystems[e->m_particleSystems.size()-1].m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Particle/Particle");
			e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMin = 15.0f;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMax = 15.0f;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMin = 1.0f;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMax = 1.0f;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMin = glm::vec2(0.05f);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMax = glm::vec2(0.05f);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeEnd = glm::vec2(1.0f);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_colorEnd = glm::vec4(1.0f);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_gravity = glm::vec3(0.0f, -9.0f, 0.0f);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_direction = glm::vec3(0.0f);
			e->m_particleSystems[e->m_particleSystems.size()-1].m_spread = 1.0f;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_spawnTime = 0.03f;

			//Point system values to params
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::POSITION]		= &t->m_position;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::LIFETIMEMIN]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMin;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::LIFETIMEMAX]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_lifeTimeMax;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPEEDMIN]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMin;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPEEDMAX]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_speedMax;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SIZEMIN]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMin;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SIZEMAX]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeMax;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SIZEEND]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_sizeEnd;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::COLOR]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_color;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::COLOREND]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_colorEnd;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::GRAVITY]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_gravity;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::DIRECTION]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_direction;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPREAD]			= &e->m_particleSystems[e->m_particleSystems.size()-1].m_spread;
			e->m_particleSystems[e->m_particleSystems.size()-1].m_params[Render::Semantic::SPAWNTIME]		= &e->m_particleSystems[e->m_particleSystems.size()-1].m_spawnTime;
		}

		/* TEMP END. */
	}

	void IngameState::Exit()
	{
		Network::NetworkEntityID id;
		id.UserID = Network::ReservedUserID::ALL;
		id.ActionID = Network::ReservedActionID::ALL;
		id.SequenceID = Network::ReservedSequenceID::ALL;

		Network::DeleteEntities(g_networkEntityMap, id, g_world->GetEntityManager());
		g_networkEntityMap.clear();
		Network::NetworkComponent::s_sequenceIDMap.clear();

		g_engineContext.m_gui->DestroyView(m_hud->GetView());
		g_engineContext.m_gui->DestroyView(g_engineContext.m_debugOverlay->GetView());

		g_world->GetEntityManager()->RemoveAllEntitiesAndComponents();
		g_world->GetTagManager()->UnregisterAll();
		g_world->GetGroupManager()->UnregisterAll();
		g_engineContext.m_physics->RemoveAll();
	}

	GameStates::GameStates IngameState::Update(float p_deltaTime)
	{
		ECS::Entity* clientEntity = g_world->GetTagManager()->GetEntityByTag("Client");
		Network::ClientComponent* clientComponent = g_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);

		// Check for quitting condition
		if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_ESCAPE) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			return GameStates::Menu;
		}

		// Check for disconnection from the server
		if (clientComponent->State == Network::ClientState::DISCONNECTED_SERVER_SHUTDOWN || clientComponent->State == Network::ClientState::DISCONNECTED_TIMEOUT)
		{
			return GameStates::Menu;
		}
		
		g_world->SetDelta(p_deltaTime);
		g_engineContext.m_renderer->Clear();

		//Update all the data that is displayed in the HUD
		ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
		m_hud->SetValue("Health", std::to_string(g_world->GetEntityManager()->GetComponent<HealthComponent>(player)->Health) );
		m_hud->SetValue("PlayerScore", std::to_string(g_world->GetEntityManager()->GetComponent<PlayerComponent>(player)->Score) );
		m_hud->SetValue("PlayerDeaths", std::to_string(g_world->GetEntityManager()->GetComponent<PlayerComponent>(player)->Deaths) );
		m_hud->SetValue("TeamScore",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(1)) ); //TODO: Fix so that we read the player team instead of hardcoding it
		m_hud->SetValue("TeamScore",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(2)) );
		m_hud->SetValue("TimeLeft", std::to_string((int)m_sharedSystems.m_matchStateSystem->GetTimeLeft()));
		m_hud->SetSelectedAbility(g_world->GetEntityManager()->GetComponent<PlayerActionComponent>(player)->SelectedAbility);

		m_hud->Update();
		RootServer::EventData event = m_hud->GetChatSystem()->PollEvent();

		switch (event.EventType)
		{
		case RootServer::UserCommands::CLIENT_RAGEQUIT:
			return GameStates::Menu;
		case RootServer::UserCommands::CLIENT_SUICIDE:
			g_world->GetEntityManager()->GetComponent<HealthComponent>(player)->Health = 0;
			break;
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

		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F12) == RootEngine::InputManager::KeyState::DOWN_EDGE)
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
#endif

		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F5) == RootEngine::InputManager::KeyState::DOWN_EDGE)
			g_engineContext.m_resourceManager->ReloadAllScripts();
		
		{
			PROFILE("World System", g_engineContext.m_profiler);
			m_sharedSystems.m_worldSystem->Process();
		}

		{
			PROFILE("Player control system", g_engineContext.m_profiler);

			g_engineContext.m_inputSys->LockInput(m_hud->GetChatSystem()->IsFocused());
			m_playerControlSystem->Process();
		}

		//m_animationSystem->Process();
        {
            PROFILE("Action system", g_engineContext.m_profiler);
            m_actionSystem->Process();
        }

		std::thread t(&RootForce::AnimationSystem::Process, m_animationSystem);

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
            PROFILE("Camera systems", g_engineContext.m_profiler);
            m_playerControlSystem->UpdateAimingDevice();
            m_thirdPersonBehaviorSystem->Process();
            m_lookAtSystem->Process();
            m_cameraSystem->Process();
        }
		
		{ 
			PROFILE("_ParticleSystem", g_engineContext.m_profiler);
			m_particleSystem->Process();
		}
		{
			PROFILE("RenderingSystem", g_engineContext.m_profiler);
            m_pointLightSystem->Process();
			m_renderingSystem->Process();

		}
		t.join();
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
			g_engineContext.m_gui->Render(m_hud->GetView());
			g_engineContext.m_gui->Render(g_engineContext.m_debugOverlay->GetView());
		}
        
		{
			PROFILE("Swap", g_engineContext.m_profiler);
			g_engineContext.m_renderer->Swap();
	}
		return GameStates::Ingame;
	}
}