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
		RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
        RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
        RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
		RootForce::HealthComponent::SetTypeId(RootForce::ComponentType::HEALTH);
        RootForce::PlayerControl::SetTypeId(RootForce::ComponentType::PLAYERCONTROL);
        RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
        RootForce::Network::NetworkClientComponent::SetTypeId(RootForce::ComponentType::NETWORKCLIENT);
        RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
        RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
        RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
        RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
        RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
        RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
        RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);
		RootForce::ScoreComponent::SetTypeId(RootForce::ComponentType::SCORE);
		RootForce::UserAbility::SetTypeId(RootForce::ComponentType::ABILITY);
		RootForce::Identity::SetTypeId(RootForce::ComponentType::IDENTITY);
		RootForce::TDMRuleSet::SetTypeId(RootForce::ComponentType::TDMRULES);
		RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);

		m_hud = std::shared_ptr<RootForce::HUD>(new HUD());
	}

	void IngameState::Initialize()
	{
		//Bind c++ functions and members to Lua
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::entity_f, RootForce::LuaAPI::entity_m, "Entity");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::renderable_f, RootForce::LuaAPI::renderable_m, "Renderable");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::transformation_f, RootForce::LuaAPI::transformation_m, "Transformation");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::physicsaccessor_f, RootForce::LuaAPI::physicsaccessor_m, "Physics");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::collision_f, RootForce::LuaAPI::collision_m, "Collision");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::collisionresponder_f, RootForce::LuaAPI::collisionresponder_m, "CollisionResponder");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::orient_f, RootForce::LuaAPI::orient_m, "Orientation");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::script_f, RootForce::LuaAPI::script_m, "Script");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::pointLight_f, RootForce::LuaAPI::pointLight_m, "PointLight");
		RootForce::LuaAPI::LuaSetupTypeNoMethods(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::vec3_f, RootForce::LuaAPI::vec3_m, "Vec3");
		RootForce::LuaAPI::LuaSetupTypeNoMethods(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::vec4_f, RootForce::LuaAPI::vec4_m, "Vec4");
		RootForce::LuaAPI::LuaSetupTypeNoMethods(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::quat_f, RootForce::LuaAPI::quat_m, "Quat");

		g_engineContext.m_resourceManager->LoadScript("AbilityTest");
        g_engineContext.m_resourceManager->LoadCollada("AnimationTest");
        
		// Initialize the system for controlling the player.
		std::vector<RootForce::Keybinding> keybindings(6);
		keybindings[0].Bindings.push_back(SDL_SCANCODE_UP);
		keybindings[0].Bindings.push_back(SDL_SCANCODE_W);
		keybindings[0].Action = RootForce::PlayerAction::MOVE_FORWARDS;

		keybindings[1].Bindings.push_back(SDL_SCANCODE_DOWN);
		keybindings[1].Bindings.push_back(SDL_SCANCODE_S);
		keybindings[1].Action = RootForce::PlayerAction::MOVE_BACKWARDS;

		keybindings[2].Bindings.push_back(SDL_SCANCODE_LEFT);
		keybindings[2].Bindings.push_back(SDL_SCANCODE_A);
		keybindings[2].Action = RootForce::PlayerAction::STRAFE_LEFT;

		keybindings[3].Bindings.push_back(SDL_SCANCODE_RIGHT);
		keybindings[3].Bindings.push_back(SDL_SCANCODE_D);
		keybindings[3].Action = RootForce::PlayerAction::STRAFE_RIGHT;

		keybindings[4].Bindings.push_back(SDL_SCANCODE_SPACE);
		keybindings[4].Action = RootForce::PlayerAction::JUMP;
		keybindings[4].Edge = true;
		
		keybindings[5].Bindings.push_back((SDL_Scancode)RootEngine::InputManager::MouseButton::LEFT);
		keybindings[5].Action = RootForce::PlayerAction::ACTIVATE_ABILITY;
		keybindings[5].Edge = true;
		
		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(g_world));
		m_playerControlSystem->SetInputInterface(g_engineContext.m_inputSys);
		m_playerControlSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_playerControlSystem->SetKeybindings(keybindings);
		m_playerControlSystem->SetPhysicsInterface(g_engineContext.m_physics);

		// System responsible for executing script based on actions.
		m_scriptSystem = new RootForce::ScriptSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::ScriptSystem>(m_scriptSystem, "ScriptSystem");

		// Initialize physics system
		m_physicsSystem = new RootForce::PhysicsSystem(g_world);
		m_physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
		m_physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
		g_world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem, "PhysicsSystem");

		m_collisionSystem = new RootForce::CollisionSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::CollisionSystem>(m_collisionSystem, "CollisionSystem");

		// Initialize render and point light system.
		m_renderingSystem = new RootForce::RenderingSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem, "RenderingSystem");

		m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

		m_pointLightSystem = new RootForce::PointLightSystem(g_world, g_engineContext.m_renderer);
		g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem, "PointLightSystem");

		m_particleSystem = new RootForce::ParticleSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem, "ParticleSystem");

		// Initialize camera systems.
		m_cameraSystem = new RootForce::CameraSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem, "CameraSystem");
		m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem, "LookAtSystem");
		m_thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(g_world, &g_engineContext);
		g_world->GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(m_thirdPersonBehaviorSystem, "ThirdPersonBehaviorSystem");

		m_displayPhysicsDebug = false;
		m_displayNormals = false;		
	}

	void IngameState::Enter()
	{
		// Lock the mouse
		g_engineContext.m_inputSys->LockMouseToCenter(true);

		// Setup the network
		m_networkContext.m_client->SetChatSystem(m_hud->GetChatSystem().get());
		m_networkContext.m_clientMessageHandler->SetChatSystem(m_hud->GetChatSystem().get());

		// Load the level and create a world

		//Initialize the debug, setting the html view
		g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->LoadURL("debug.html"));

		//Init the hud and set one test ability for now
		m_hud->Initialize(g_engineContext.m_gui->LoadURL("hud.html"), g_engineContext.m_gui->GetDispatcher());
		m_hud->SetAbility(1, "TestBall");
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
			ECS::Entity* p = g_world->GetEntityManager()->CreateEntity();
			RootForce::Transform* t = g_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
			RootForce::ParticleEmitter* e = g_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);	
		
			Render::ParticleSystemDescription desc;
			desc.m_initalPos = pos[i];
			desc.m_initalVel = glm::vec3(0,0,0);
			desc.m_size = glm::vec2(0.05f, 0.05f);

			e->m_system = g_engineContext.m_renderer->CreateParticleSystem(desc);	
			e->m_material = g_engineContext.m_resourceManager->GetMaterial("particle");
			e->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);
			e->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Particle/Particle");
		}

		/* TEMP END. */
	}

	void IngameState::Exit()
		{
		g_engineContext.m_gui->DestroyView(m_hud->GetView());
		g_engineContext.m_gui->DestroyView(g_engineContext.m_debugOverlay->GetView());

		g_world->GetEntityManager()->RemoveAllEntitiesAndComponents();
		g_world->GetTagManager()->UnregisterAll();
		g_world->GetGroupManager()->UnregisterAll();
		g_engineContext.m_physics->RemoveAll();
		}

	GameStates::GameStates IngameState::Update(float p_deltaTime)
	{
		// Check for quitting condition
		if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_ESCAPE) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			return GameStates::Menu;
	}

		// Check for disconnection from the server
		if (m_networkContext.m_clientMessageHandler->GetClientState() == RootForce::Network::ClientState::CONNECTION_LOST)
	{
			return GameStates::Menu;
		}
		
		g_world->SetDelta(p_deltaTime);
		g_engineContext.m_renderer->Clear();

		//Update all the data that is displayed in the HUD
		m_hud->SetValue("Health", std::to_string(g_world->GetEntityManager()->GetComponent<HealthComponent>( g_world->GetTagManager()->GetEntityByTag("Player") )->Health) );
		m_hud->SetValue("PlayerScore", std::to_string(g_world->GetEntityManager()->GetComponent<ScoreComponent>( g_world->GetTagManager()->GetEntityByTag("Player") )->Score) );
		m_hud->SetValue("PlayerDeaths", std::to_string(g_world->GetEntityManager()->GetComponent<ScoreComponent>( g_world->GetTagManager()->GetEntityByTag("Player") )->Deaths) );
		m_hud->SetValue("TeamScore",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(1)) ); //TODO: Fix so that we read the player team instead of hardcoding it
		m_hud->SetValue("TeamScore",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(2)) );
		m_hud->SetValue("TimeLeft", std::to_string((int)m_sharedSystems.m_matchStateSystem->GetTimeLeft()));

		m_hud->Update();

#ifdef _DEBUG
		//Debug drawing TODO: Remove for release
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
				g_engineContext.m_renderer->DisplayNormals(false);
#endif

		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F5) == RootEngine::InputManager::KeyState::DOWN_EDGE)
			g_engineContext.m_resourceManager->ReloadAllScripts();
		
		
		{
			PROFILE("Player control system", g_engineContext.m_profiler);
			if(!m_hud->GetChatSystem()->IsFocused())
			m_playerControlSystem->Process();
		}

        {
            PROFILE("Physics", g_engineContext.m_profiler);
            g_engineContext.m_physics->Update(p_deltaTime);
            m_physicsSystem->Process();
        }

        {
            PROFILE("Collision system", g_engineContext.m_profiler);
            m_collisionSystem->Process();
        }
        
		{
			PROFILE("Script system", g_engineContext.m_profiler);
			m_scriptSystem->Process();
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

		{
			PROFILE("Rendering", g_engineContext.m_profiler);
			g_engineContext.m_renderer->Render();
		}

		{
			PROFILE("Render Lines", g_engineContext.m_profiler);
			g_engineContext.m_renderer->RenderLines();
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