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

		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(10);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::HealthComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerControl>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Physics>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::NetworkComponent>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::LookAtBehavior>(20);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ThirdPersonBehavior>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Script>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Collision>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::CollisionResponder>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Animation>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::TDMRuleSet>(1);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerActionComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PlayerPhysics>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::StateComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Shadowcaster>(1);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::DirectionalLight>(10);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ClientComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Network::ServerInformationComponent>(1);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Ragdoll>(100);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::WaterCollider>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::AbilitySpawnComponent>(100);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::TryPickupComponent>(12);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::SoundComponent>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::TimerComponent>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::FollowComponent>(1000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::HomingComponent>(1000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::RayComponent>(1000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::DamageAndKnockback>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Scalable>(5000);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::StatChange>(500);
		g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::KillAnnouncement>(1);

		m_hud = std::shared_ptr<RootForce::HUD>(new HUD());
	}

	void IngameState::Initialize()
	{
		// Bind c++ functions and members to Lua.
		LuaAPI::RegisterLuaTypes(g_engineContext.m_script->GetLuaState());
		
		//Do not add scripts for abilities that are included in the abilitypacks. They will be loaded automatically
		g_engineContext.m_resourceManager->LoadScript("Global");
		g_engineContext.m_resourceManager->LoadScript("TimerEntity");
		g_engineContext.m_resourceManager->LoadScript("Push");
		g_engineContext.m_resourceManager->LoadScript("Identiray");
		//g_engineContext.m_resourceManager->LoadScript("CompileChecker");
		g_engineContext.m_resourceManager->LoadScript("Player");
		g_engineContext.m_resourceManager->LoadScript("Explosion");
		g_engineContext.m_resourceManager->LoadScript("AbilitySpawnPoint");
		g_engineContext.m_resourceManager->LoadScript("XplodingMushroomPlanted");
		g_engineContext.m_resourceManager->LoadScript("ShroomExplosion");

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

		// Initialize the interpolation system
		m_transformInterpolationSystem = new RootForce::TransformInterpolationSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::TransformInterpolationSystem>(m_transformInterpolationSystem);
		
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

		m_botanySystem = new RootForce::BotanySystem(g_world, &g_engineContext);

		// Initialize the timer system.
		m_timerSystem = new RootForce::TimerSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::TimerSystem>(m_timerSystem);

		// Initialize the follow system.
		m_followSystem = new RootForce::FollowSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::FollowSystem>(m_followSystem);

		// Initialize the network debug system.
		m_networkDebugSystem = new RootForce::Network::NetworkDebugSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::Network::NetworkDebugSystem>(m_networkDebugSystem);

		// Initialize the homing system.
		m_homingSystem = new RootForce::HomingSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::HomingSystem>(m_homingSystem);

		// Initialize the ray system.
		m_raySystem = new RootForce::RaySystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::RaySystem>(m_raySystem);

		//Initialize water death system.
		m_waterDeathSystem = new RootForce::WaterDeathSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::WaterDeathSystem>(m_waterDeathSystem);

		//Initialize scale system.
		m_scaleSystem = new RootForce::ScaleSystem(g_world);
		g_world->GetSystemManager()->AddSystem<RootForce::ScaleSystem>(m_scaleSystem);

		// Initialize the deserialization system.
		m_sharedSystems.m_deserializationSystem = std::shared_ptr<RootForce::DeserializationSystem>(new RootForce::DeserializationSystem(g_world));

		// Set debug visualization flags.
		m_displayPhysicsDebug = false;
		m_displayNormals = false;
		m_displayWorldDebug = false;		
		m_displayDebugHUD = true;
		m_displayGuiHUD = true;

#ifndef _DEBUG
		m_displayDebugHUD = false;
#endif
	}

	void IngameState::Enter()
	{
		m_shadowSystem->SetQuadTree(m_sharedSystems.m_worldSystem->GetQuadTree());

#ifndef _DEBUG
		BotanyTextures textures;
		textures.m_diffuse = g_world->GetStorage()->GetValueAsString("GrassDiffuse");
		textures.m_translucency = g_world->GetStorage()->GetValueAsString("GrassTranslucency");
		textures.m_billboard = g_world->GetStorage()->GetValueAsString("GrassBillboard");

		if(textures.m_diffuse != "")
		{
			// Subdivide terrain for grass chunk rendering.
			m_botanySystem->Initialize(textures, 0.0f);
		}


		// Subdivide world.
		//m_sharedSystems.m_worldSystem->SubdivideTree();
#endif

		// Lock the mouse
		g_engineContext.m_inputSys->LockMouseToCenter(true);

		// Setup the network
		m_networkContext.m_client->SetChatSystem(m_hud->GetChatSystem().get());
		m_networkContext.m_clientMessageHandler->SetChatSystem(m_hud->GetChatSystem().get());
		
		// Set server peers to null
		/*
		m_sharedSystems.m_abilitySpawnSystem->SetServerPeerInterface(nullptr);
		m_sharedSystems.m_abilitySpawnSystem->SetClientPeerInterface(nullptr);
		m_sharedSystems.m_respawnSystem->SetServerPeer(nullptr);
		m_sharedSystems.m_respawnSystem->SetClientPeer(nullptr);
		m_playerControlSystem->SetClientPeer(nullptr);
		m_actionSystem->SetServerPeerInterface(nullptr);
		m_actionSystem->SetClientPeerInterface(nullptr);
		m_timerSystem->SetServerPeer(nullptr);
		*/

		// Set the network client peer interfaces.
		m_sharedSystems.m_abilitySpawnSystem->SetClientPeerInterface(m_networkContext.m_client->GetPeerInterface());
		m_sharedSystems.m_respawnSystem->SetClientPeer(m_networkContext.m_client->GetPeerInterface());
		m_playerControlSystem->SetClientPeer(m_networkContext.m_client->GetPeerInterface());
		m_actionSystem->SetClientPeerInterface(m_networkContext.m_client->GetPeerInterface());
		m_sharedSystems.m_matchStateSystem->SetNetworkContext(&m_networkContext);

		// Set the network server peer interfaces if we are a server.
		if (m_networkContext.m_server != nullptr)
		{
			m_sharedSystems.m_abilitySpawnSystem->SetServerPeerInterface(m_networkContext.m_server->GetPeerInterface());
			m_sharedSystems.m_respawnSystem->SetServerPeer(m_networkContext.m_server->GetPeerInterface());
			m_actionSystem->SetServerPeerInterface(m_networkContext.m_server->GetPeerInterface());
			m_timerSystem->SetServerPeer(m_networkContext.m_server->GetPeerInterface());
		}

		// Give the player control system access to the HUD.
		m_playerControlSystem->SetHUD(m_hud.get());

		// Initialize the debug, setting the html view
		g_engineContext.m_debugOverlay->SetView(g_engineContext.m_gui->LoadURL("Debug", "debug.html"));

		// Init the hud and set one test ability for now
		m_hud->Initialize(g_engineContext.m_gui->LoadURL("HUD", "hud.html"), &g_engineContext);
		m_hud->SetSelectedAbility(0);

		m_sharedSystems.m_matchStateSystem->SetHUD(m_hud.get());
		m_sharedSystems.m_matchStateSystem->SetAbilitySpawnSystem(m_sharedSystems.m_abilitySpawnSystem);

		// Reset the ingame menu before we start the match
		m_ingameMenu = std::shared_ptr<RootForce::IngameMenu>(new IngameMenu(g_engineContext.m_gui->LoadURL("Menu", "ingameMenu.html"), g_engineContext, m_keymapper));
		m_ingameMenu->SetClientPeerInterface(m_networkContext.m_client->GetPeerInterface());
		m_displayIngameMenu = false;
		m_ingameMenu->GetView()->SetActive(false);
		
		m_animationSystem->Start();

		m_waterSystem->CreateWater(g_world->GetStorage()->GetValueAsFloat("WaterHeight"));

		if(m_networkContext.m_server != nullptr)
			m_timerSystem->SetServerPeer(m_networkContext.m_server->GetPeerInterface());

		m_playerControlSystem->SetKeybindings(m_keymapper->GetKeybindings());

		//Ray stuff
		g_engineContext.m_resourceManager->LoadEffect("Ray");
		RootEngine::Model* rayModel = g_engineContext.m_resourceManager->CreateModel("rayModel");
		
		Render::Vertex1P rayVertices;
		rayVertices.m_pos = glm::vec3(0.0f);
		
		// Create 1P mesh for shadows.
		Render::MeshInterface* mesh1P = g_engineContext.m_renderer->CreateMesh();
		mesh1P->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ARRAY_BUFFER));	
		mesh1P->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
		mesh1P->CreateVertexBuffer1P((Render::Vertex1P*)(&rayVertices), 1);
		mesh1P->SetPrimitiveType(GL_POINTS);

		rayModel->m_meshes[0] = mesh1P;
		
		//Team selection stuff
		m_ingameMenu->GetView()->BufferJavascript("ShowTeamSelect();");
		m_displayIngameMenu = true;
		g_engineContext.m_debugOverlay->GetView()->SetActive(false);
		g_engineContext.m_inputSys->LockMouseToCenter(false);
		m_ingameMenu->GetView()->SetActive(true);
		g_engineContext.m_inputSys->LockMouseToCenter(false);
		m_ingameMenu->Reset();
	}

	void IngameState::Exit()
	{
		// Make sure the user is not stuck keymapping in case of disconnects or other abnormal termination
		m_keymapper->UnfocusBindAction();

		m_animationSystem->Terminate();

		// Remove all networked entities and reset the entity map and sequence map.
		//Network::DeleteEntities(g_networkEntityMap, Network::NetworkEntityID(Network::ReservedUserID::ALL, Network::ReservedActionID::ALL, Network::ReservedSequenceID::ALL), g_world->GetEntityManager());
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
		g_world->GetSystemManager()->Clear();
		g_engineContext.m_physics->RemoveAll();

		// Set server peers to null
		m_sharedSystems.m_abilitySpawnSystem->SetServerPeerInterface(nullptr);
		m_sharedSystems.m_abilitySpawnSystem->SetClientPeerInterface(nullptr);
		m_sharedSystems.m_respawnSystem->SetServerPeer(nullptr);
		m_sharedSystems.m_respawnSystem->SetClientPeer(nullptr);
		m_playerControlSystem->SetClientPeer(nullptr);
		m_actionSystem->SetServerPeerInterface(nullptr);
		m_actionSystem->SetClientPeerInterface(nullptr);
		m_timerSystem->SetServerPeer(nullptr);

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

		g_engineContext.m_profiler->Update(p_deltaTime);
		g_engineContext.m_debugOverlay->RenderOverlay();
		{
			g_engineContext.m_gui->Update();
			//Update Menu to make sure Setting changes are made in the main thread
			m_ingameMenu->Update();
			if (m_displayIngameMenu)
			{
				g_engineContext.m_gui->Render(m_ingameMenu->GetView());
				m_ingameMenu->GetView()->Focus();

				
			}
			else
			{
				{
					PROFILE("GUI HUD", g_engineContext.m_profiler);

					m_hud->GetView()->SetActive(m_displayGuiHUD);
					m_hud->GetView()->Focus();
					if(m_displayGuiHUD)
						g_engineContext.m_gui->Render(m_hud->GetView());
				}
				{
					PROFILE("GUI Debug", g_engineContext.m_profiler);

					g_engineContext.m_debugOverlay->GetView()->SetActive(m_displayDebugHUD);
					if(m_displayDebugHUD)
						g_engineContext.m_gui->Render(g_engineContext.m_debugOverlay->GetView());
				}
			}
		}

		// Check for disconnection from the server
		ECS::Entity* clientEntity = g_world->GetTagManager()->GetEntityByTag("Client");
		Network::ClientComponent* clientComponent = g_world->GetEntityManager()->GetComponent<Network::ClientComponent>(clientEntity);

		if (clientComponent->State == Network::ClientState::DISCONNECTED_SERVER_SHUTDOWN || clientComponent->State == Network::ClientState::DISCONNECTED_TIMEOUT)
		{
			return GameStates::Menu;
		}
		
		// Check for time for reset
		if(m_sharedSystems.m_matchStateSystem->GetTimeLeft() <= -10)
		{
			// Do a reconnect, this should force the server to rehost the game (thus starting a new one) and the clients should reconnect automagically
			return GameStates::Connecting;
		}

		// If game is over, set gameover to true and wait for the restart
		if(m_sharedSystems.m_matchStateSystem->IsMatchOver())
		{
			// If kill victory, set the time to 0 so restart will activate correctly
			if(m_sharedSystems.m_matchStateSystem->GetTimeLeft() > 0)
				g_world->GetEntityManager()->GetComponent<TDMRuleSet>( g_world->GetTagManager()->GetEntityByTag("MatchState") )->TimeLeft = 0;
		}
		
		// Update the HUD values.
		UpdateHUD();
		
		// Update the console commands.
		GameStates::GameStates consoleGameState = UpdateConsole();
		if (consoleGameState != GameStates::Ingame)
			return consoleGameState;

		// Check for special keypress events.
		if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F12) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_displayGuiHUD = m_displayGuiHUD ? false : true;
		}
		
		{
			PROFILE("Water system", g_engineContext.m_profiler);
			m_waterSystem->Process();
			m_waterDeathSystem->Process();
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
			m_networkDebugSystem->Process();
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
			PROFILE("Match State System", g_engineContext.m_profiler);
			m_sharedSystems.m_matchStateSystem->UpdateDeltatime(p_deltaTime);
			m_sharedSystems.m_matchStateSystem->Process();
		}

		{
			PROFILE("Ragdoll system", g_engineContext.m_profiler);
			m_ragdollSystem->Process();
		}

		{
			PROFILE("Respawn system", g_engineContext.m_profiler);
			m_sharedSystems.m_respawnSystem->Process();
		}

		{
			PROFILE("AbilitySpawn system", g_engineContext.m_profiler);
			m_sharedSystems.m_abilitySpawnSystem->Process();
		}

		{
			PROFILE("Physics", g_engineContext.m_profiler);

			m_physicsTransformCorrectionSystem->Process();
			g_engineContext.m_physics->Update(p_deltaTime);
			m_physicsSystem->Process();
		}

		{
			PROFILE("Follow system", g_engineContext.m_profiler);
			m_followSystem->Process();
		}

		{
			PROFILE("Homing system", g_engineContext.m_profiler);
			m_homingSystem->Process();
		}

		{
			PROFILE("Ray system", g_engineContext.m_profiler);
			m_raySystem->Process();
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
			PROFILE("DeserializationSystem", g_engineContext.m_profiler);
			m_sharedSystems.m_deserializationSystem->Process();
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
			m_transformInterpolationSystem->Process();
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
			PROFILE("Botany System", g_engineContext.m_profiler);
			m_botanySystem->Process();
		}

		{
			PROFILE("RenderingSystem", g_engineContext.m_profiler);
			m_scaleSystem->Process();
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
			m_displayIngameMenu = true;
			m_hud->GetView()->SetActive(false);
			g_engineContext.m_debugOverlay->GetView()->SetActive(false);
			m_ingameMenu->GetView()->SetActive(true);
			g_engineContext.m_inputSys->LockMouseToCenter(false);
			m_ingameMenu->Reset();
		}
		if (!m_sharedSystems.m_matchStateSystem->IsMatchOver() && g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_M) == RootEngine::InputManager::KeyState::DOWN_EDGE)
		{
			m_ingameMenu->GetView()->BufferJavascript("ShowTeamSelect();");
			m_displayIngameMenu = true;
			m_hud->GetView()->SetActive(false);
			g_engineContext.m_debugOverlay->GetView()->SetActive(false);
			m_ingameMenu->GetView()->SetActive(true);
			g_engineContext.m_inputSys->LockMouseToCenter(false);
			m_ingameMenu->Reset();
		}
		if (m_ingameMenu->GetReturn())
		{
			m_displayIngameMenu = false;
			m_ingameMenu->GetView()->SetActive(false);
			g_engineContext.m_inputSys->LockMouseToCenter(true);
			m_hud->GetView()->SetActive(m_displayGuiHUD);
			g_engineContext.m_debugOverlay->GetView()->SetActive(m_displayDebugHUD);
			m_ingameMenu->Reset();
			// Update keybindings when returning to game
			m_playerControlSystem->SetKeybindings(m_keymapper->GetKeybindings());
		}
		g_engineContext.m_sound->Update();
		return GameStates::Ingame;
	}

	GameStates::GameStates IngameState::UpdateConsole()
	{
		RootServer::EventData event = m_hud->GetChatSystem()->PollEvent();

		if(RootServer::MatchAny(event.EventType, 1, "HELP"))
		{
			g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "[COMMANDS]");
			g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/w -Water settings");
			g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/r -Render settings");
			g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/l -Logging settings");
			g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/b -Botany settings");
			PrintGlobalCommandHelp();
		}

		if(RootServer::MatchAny(event.EventType, 1, "HELPALL"))
		{
			std::stringstream helpStream;
			helpStream << "help help ";
			m_waterSystem->ParseCommands(&helpStream);
			helpStream << "help help ";
			g_engineContext.m_renderer->ParseCommands(&helpStream);
			helpStream << "help help ";
			m_botanySystem->ParseCommands(&helpStream);
			helpStream << "help help ";
			g_engineContext.m_logger->ParseCommand(&helpStream);
			PrintGlobalCommandHelp();
		}

		if(RootServer::MatchAny(event.EventType, 2, "R", "RENDER"))
		{
			g_engineContext.m_renderer->ParseCommands(&event.Data);
		}

		if(RootServer::MatchAny(event.EventType, 2, "LOGGER", "L"))
		{
			g_engineContext.m_logger->ParseCommand(&event.Data);
		}

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
			m_waterSystem->ParseCommands(&event.Data);
		}

		if(RootServer::MatchAny(event.EventType, 2, "B", "BOTANY"))
		{
			m_botanySystem->ParseCommands(&event.Data);
		}

		if(RootServer::MatchAny(event.EventType, 2, "RS", "RELOADSCRIPTS"))
		{
			g_engineContext.m_resourceManager->ReloadAllScripts();
		}

		if(RootServer::MatchAny(event.EventType, 3, "Q", "QUIT", "RAGEQUIT"))
		{
			return GameStates::Menu;
		}

		if(RootServer::MatchAny(event.EventType, 1, "KICK"))
		{
			if(m_networkContext.m_server != nullptr)
			{
				RootForce::Network::UserID_t userID = Network::ReservedUserID::NONE;
				std::string name;
				event.Data >> name;
				event.Data >> name;
				for(auto pair : g_networkEntityMap)
				{
					if(pair.first.ActionID != Network::ReservedActionID::CONNECT || pair.first.SequenceID != RootForce::Network::SEQUENCE_PLAYER_ENTITY || !pair.second)
						continue;
					PlayerComponent* playerComp = g_world->GetEntityManager()->GetComponent<PlayerComponent>(pair.second);
					Network::NetworkComponent* networkComp = g_world->GetEntityManager()->GetComponent<Network::NetworkComponent>(pair.second);
					if(playerComp->Name.compare(name) == 0)
					{
						userID = networkComp->ID.UserID;
						break;
					}
				}
				if(userID != Network::ReservedUserID::NONE)
				{
					RakNet::SystemAddress sysAddress = m_networkContext.m_server->GetPeerInterface()->GetSystemAddressFromIndex(userID);
					m_networkContext.m_server->GetPeerInterface()->CloseConnection(sysAddress, true);
				}
				else
				{
					m_hud->GetChatSystem()->JSAddMessage("Player not found, check your speeling!");
				}
			}
		}

		else if(RootServer::MatchAny(event.EventType, 2, "KILL","SUICIDE"))
		{
			// Kill ourselves.
			ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");

			g_world->GetEntityManager()->GetComponent<HealthComponent>(player)->Health = 0;
			PlayerComponent* playerComp =  g_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
			playerComp->Deaths ++;

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


		return GameStates::Ingame;
	}

	void IngameState::UpdateHUD()
	{
		if (m_sharedSystems.m_matchStateSystem->IsMatchOver())
		{
			m_hud->SetValue("EndGame", "true" );
		}

		if (m_sharedSystems.m_matchStateSystem->IsMatchOver() || g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_TAB) == RootEngine::InputManager::KeyState::DOWN)
		{
			m_hud->SetValue("ShowScore", "true" );
			m_hud->SetScoreList(m_sharedSystems.m_matchStateSystem->GetScoreList());
		}
		else if (g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_TAB) == RootEngine::InputManager::KeyState::UP)
		{
			m_hud->SetValue("ShowScore", "false" );
		}
		if(m_displayIngameMenu)
		{
			m_ingameMenu->SetScoreList(m_sharedSystems.m_matchStateSystem->GetScoreList());
		}
		else
		{
			ECS::Entity* player = g_world->GetTagManager()->GetEntityByTag("Player");
			if (!m_sharedSystems.m_matchStateSystem->IsMatchOver())
			{
				PlayerComponent* playerComponent = g_world->GetEntityManager()->GetComponent<PlayerComponent>(player);
				HealthComponent* healthComponent = g_world->GetEntityManager()->GetComponent<HealthComponent>(player);
				PlayerActionComponent* playerActionComponent = g_world->GetEntityManager()->GetComponent<PlayerActionComponent>(player);

				//Update all the data that is displayed in the HUD
				m_hud->SetValue("PlayerScore", std::to_string(playerComponent->Score) );
				m_hud->SetValue("PlayerDeaths", std::to_string(playerComponent->Deaths) );
				m_hud->SetValue("Team1Score",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(1)) );
				m_hud->SetValue("Team2Score",  std::to_string(m_sharedSystems.m_matchStateSystem->GetTeamScore(2)) );
				if(healthComponent && playerActionComponent)
				{
					m_hud->SetValue("Health", std::to_string(healthComponent->Health) );
					m_hud->SetValue("IsDead", healthComponent->IsDead?"true":"false" );

					m_hud->SetAbility(1, playerComponent->AbilityScripts[0].Name);
					m_hud->SetAbility(2,  playerComponent->AbilityScripts[1].Name);
					m_hud->SetAbility(3,  playerComponent->AbilityScripts[2].Name);

					m_hud->SetCharges(1, playerComponent->AbilityScripts[0].Charges);
					m_hud->SetCharges(2, playerComponent->AbilityScripts[1].Charges);
					m_hud->SetCharges(3, playerComponent->AbilityScripts[2].Charges);

					if(playerComponent->AbilityScripts[0].Cooldown > 0 && playerComponent->AbilityScripts[0].Name.compare("") != 0)
						m_hud->SetCooldown(1, playerComponent->AbilityScripts[0].Cooldown/(float) g_engineContext.m_script->GetGlobalNumber("cooldown", playerComponent->AbilityScripts[0].Name));
					else
						m_hud->SetCooldown(1, 0);
					if(playerComponent->AbilityScripts[1].Cooldown > 0 && playerComponent->AbilityScripts[1].Name.compare("") != 0)
						m_hud->SetCooldown(2, playerComponent->AbilityScripts[1].Cooldown/(float) g_engineContext.m_script->GetGlobalNumber("cooldown", playerComponent->AbilityScripts[1].Name));
					else
						m_hud->SetCooldown(2, 0);
					if(playerComponent->AbilityScripts[2].Cooldown > 0 && playerComponent->AbilityScripts[2].Name.compare("") != 0)
						m_hud->SetCooldown(3, playerComponent->AbilityScripts[2].Cooldown/(float) g_engineContext.m_script->GetGlobalNumber("cooldown", playerComponent->AbilityScripts[2].Name));
					else
						m_hud->SetCooldown(3, 0);
					m_hud->SetSelectedAbility(playerActionComponent->SelectedAbility + 1);
				}
			}
		}

		m_hud->SetValue("TimeLeft", std::to_string((int)m_sharedSystems.m_matchStateSystem->GetTimeLeft()));
		m_hud->Update(); // Executes either the HUD update or ShowScore if the match is over
	}

	void IngameState::PrintGlobalCommandHelp()
	{
		//Print help functions for global commands
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "[GLOBAL CONSOLE COMMANDS]");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/helpall - Print help for all subsystems and globals");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/so - Toggle profiler sorting");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/pr - Toggle profiler display");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/pd - Toggle physics debug lines display");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/nd - Toggle normal lines display");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/rs - Reload all scripts");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/kill	- Suicide");
		g_engineContext.m_logger->LogText(LogTag::NOTAG, LogLevel::HELP_PRINT, "/quit	- Disconnect from server");
	}

}