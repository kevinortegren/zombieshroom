#include "Ingamestate.h"
#include <RootForce/Include/LuaAPI.h>

namespace RootForce
{
	Ingamestate::Ingamestate()
	{
		RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
        RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
        RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
        RootForce::Player::SetTypeId(RootForce::ComponentType::PLAYER);
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
        RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);
		RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);
	}

	void Ingamestate::Initialize(RootEngine::GameSharedContext* p_engineContext, 
			ECS::World* p_world, 
			GameStates::PlayData p_playData, 
			RootForce::Network::Client* p_client, 
			RootForce::Network::ClientMessageHandler* p_clientMessageHandler)
	{
		m_engineContext = p_engineContext;
		m_world = p_world;

		//Bind c++ functions and members to Lua
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::entity_f, RootForce::LuaAPI::entity_m, "Entity");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::renderable_f, RootForce::LuaAPI::renderable_m, "Renderable");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::transformation_f, RootForce::LuaAPI::transformation_m, "Transformation");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::physicsaccessor_f, RootForce::LuaAPI::physicsaccessor_m, "Physics");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::collision_f, RootForce::LuaAPI::collision_m, "Collision");
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::collisionresponder_f, RootForce::LuaAPI::collisionresponder_m, "CollisionResponder");
		RootForce::LuaAPI::LuaSetupTypeNoMethods(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::vec3_f, RootForce::LuaAPI::vec3_m, "Vec3");
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
		
		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(m_world));
		m_playerControlSystem->SetInputInterface(m_engineContext->m_inputSys);
		m_playerControlSystem->SetLoggingInterface(m_engineContext->m_logger);
		m_playerControlSystem->SetKeybindings(keybindings);
		m_playerControlSystem->SetPhysicsInterface(m_engineContext->m_physics);

		// System responsible for updating the world.
		m_worldSystem = std::shared_ptr<RootForce::WorldSystem>(new RootForce::WorldSystem(m_world));

		// System responsible for updating the player.
		m_playerSystem = std::shared_ptr<RootForce::PlayerSystem>(new RootForce::PlayerSystem(m_world, m_engineContext));

		// System responsible for executing script based on actions.
		m_scriptSystem = new RootForce::ScriptSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::ScriptSystem>(m_scriptSystem, "ScriptSystem");

		// Initialize physics system
		m_physicsSystem = new RootForce::PhysicsSystem(m_world);
		m_physicsSystem->SetPhysicsInterface(g_engineContext.m_physics);
		m_physicsSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem, "PhysicsSystem");

		m_collisionSystem = new RootForce::CollisionSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::CollisionSystem>(m_collisionSystem, "CollisionSystem");

		// Initialize render and point light system.
		m_renderingSystem = new RootForce::RenderingSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem, "RenderingSystem");

		m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
		m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

		m_pointLightSystem = new RootForce::PointLightSystem(m_world, g_engineContext.m_renderer);
		m_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem, "PointLightSystem");

		m_particleSystem = new RootForce::ParticleSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem, "ParticleSystem");

		// Initialize camera systems.
		m_cameraSystem = new RootForce::CameraSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem, "CameraSystem");
		m_lookAtSystem = new RootForce::LookAtSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem, "LookAtSystem");
		m_thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(m_thirdPersonBehaviorSystem, "ThirdPersonBehaviorSystem");

        // Create a world and player
        m_worldSystem->CreateWorld("level");
		m_playerSystem->CreatePlayer();


		// Test particle entity.
		ECS::Entity* p = m_world->GetEntityManager()->CreateEntity();
		RootForce::Transform* t = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(p);
		RootForce::ParticleEmitter* e = m_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(p);	
		
		e->m_system = g_engineContext.m_renderer->CreateParticleSystem();	
		e->m_material = g_engineContext.m_resourceManager->GetMaterial("particle");
		e->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("particle_diffuse", Render::TextureType::TEXTURE_2D);
		e->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("ParticleRender");

		m_displayPhysicsDebug = false;
		m_displayNormals = false;

		m_engineContext->m_debugOverlay->SetView(m_engineContext->m_gui->LoadURL("debug.html"));
        
		// Setup the skybox.
		//auto e = m_world->GetTagManager()->GetEntityByTag("Skybox");
		//auto r = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(e);
		//r->m_material.m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(
		//	"SkyBox", Render::TextureType::TEXTURE_CUBEMAP);

		m_chat = std::shared_ptr<RootForce::ChatSystem>(new RootForce::ChatSystem);
		m_chat->Initialize(m_engineContext->m_gui->LoadURL("hud.html"), m_engineContext->m_gui->GetDispatcher());
		
		// Setup the network
		m_client = p_client;
		m_clientMessageHandler = p_clientMessageHandler;

		if (p_playData.Host)
		{
			m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(p_engineContext->m_logger, "Local Server", p_playData.p_port));
			m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_server->GetPeerInterface(), g_engineContext.m_logger));
			m_server->SetMessageHandler(m_serverMessageHandler.get());
			m_client->Connect("127.0.0.1", p_playData.p_port);
		}
		else
		{
			m_client->Connect(p_playData.p_address.c_str(), p_playData.p_port); 
		}

		m_client->SetChatSystem(m_chat.get());
		m_clientMessageHandler->SetChatSystem(m_chat.get());
	}

	void Ingamestate::Update(float p_deltaTime)
	{
		m_world->SetDelta(p_deltaTime);

		m_engineContext->m_renderer->Clear();

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

		{
			PROFILE("Player control system", m_engineContext->m_profiler);
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
			PROFILE("Script system", m_engineContext->m_profiler);
			m_scriptSystem->Process();
		}

		if (m_server != nullptr)
		{
			PROFILE("Server", m_engineContext->m_profiler);
			m_server->Update();
		}

		{
			PROFILE("Client", m_engineContext->m_profiler);
			m_client->Update();
		}
        
		{
            PROFILE("Camera systems", g_engineContext.m_profiler);
            m_playerControlSystem->UpdateAimingDevice();
            m_thirdPersonBehaviorSystem->Process();
            m_lookAtSystem->Process();
            m_cameraSystem->Process();
        }
		
		{ 
			PROFILE("_ParticleSystem", m_engineContext->m_profiler);
			m_particleSystem->Process();
		}

		{
			PROFILE("RenderingSystem", m_engineContext->m_profiler);
            m_pointLightSystem->Process();
			m_renderingSystem->Process();

		}
        
		m_engineContext->m_renderer->Render();

		{
			PROFILE("Render Lines", m_engineContext->m_profiler);
			m_engineContext->m_renderer->RenderLines();
		}
        
        g_engineContext.m_profiler->Update(p_deltaTime);

		{
			PROFILE("GUI", m_engineContext->m_profiler);

			//m_engineContext->m_gui->Update();
			//m_engineContext->m_gui->Render();
		}
        
		m_engineContext->m_debugOverlay->RenderOverlay();
		m_engineContext->m_renderer->Swap();
	}
}