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
		RootForce::PhysicsAccessor::SetTypeId(RootForce::ComponentType::PHYSICS);
		RootForce::Network::NetworkClientComponent::SetTypeId(RootForce::ComponentType::NETWORKCLIENT);
		RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
		RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
		RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
		RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
		RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
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
		RootForce::LuaAPI::LuaSetupType(g_engineContext.m_script->GetLuaState(), RootForce::LuaAPI::physicsaccessor_f, RootForce::LuaAPI::physicsaccessor_m, "PhysicsAccessor");

		g_engineContext.m_resourceManager->LoadScript("AbilityTest");

		// Initialize the system for controlling the player.
		std::vector<RootForce::Keybinding> keybindings(5);
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
		keybindings[4].Action = RootForce::PlayerAction::ACTIVATE_ABILITY;
		keybindings[4].Edge = true;

		m_playerControlSystem = std::shared_ptr<RootForce::PlayerControlSystem>(new RootForce::PlayerControlSystem(m_world));
		m_playerControlSystem->SetInputInterface(m_engineContext->m_inputSys);
		m_playerControlSystem->SetLoggingInterface(m_engineContext->m_logger);
		m_playerControlSystem->SetKeybindings(keybindings);
		m_playerControlSystem->SetPhysicsInterface(m_engineContext->m_physics);

		//Initiate scriptsystem
		m_scriptSystem = new RootForce::ScriptSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::ScriptSystem>(m_scriptSystem, "ScriptSystem");

		// Initialize physics system
		m_physicsSystem = new RootForce::PhysicsSystem(m_world);
		m_physicsSystem->SetPhysicsInterface(m_engineContext->m_physics);
		m_physicsSystem->SetLoggingInterface(m_engineContext->m_logger);
		m_world->GetSystemManager()->AddSystem<RootForce::PhysicsSystem>(m_physicsSystem, "PhysicsSystem");

		m_world->GetEntityImporter()->SetImporter(Importer);
		m_world->GetEntityExporter()->SetExporter(Exporter);

		// Initialize render and point light system.
		m_renderingSystem = new RootForce::RenderingSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem, "RenderingSystem");

		m_renderingSystem->SetLoggingInterface(m_engineContext->m_logger);
		m_renderingSystem->SetRendererInterface(m_engineContext->m_renderer);

		m_cameraSystem = new RootForce::CameraSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem, "CameraSystem");
		m_lookAtSystem = new RootForce::LookAtSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem, "LookAtSystem");
		m_thirdPersonBehaviorSystem = new RootForce::ThirdPersonBehaviorSystem(m_world);
		m_world->GetSystemManager()->AddSystem<RootForce::ThirdPersonBehaviorSystem>(m_thirdPersonBehaviorSystem, "ThirdPersonBehaviorSystem");

		m_pointLightSystem = new RootForce::PointLightSystem(m_world, m_engineContext->m_renderer);
		m_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem, "PointLightSystem");


		//Initialize Abilitysystem
		m_abilitySystem = new RootForce::AbilitySystem(m_world, m_engineContext->m_renderer);
		m_world->GetSystemManager()->AddSystem<RootForce::AbilitySystem>(m_abilitySystem, "AbilitySystem");

		//Create camera
		ECS::Entity* cameraEntity = m_world->GetEntityManager()->CreateEntity();
		m_world->GetTagManager()->RegisterEntity("Camera", cameraEntity);
		RootForce::Camera* camera = m_world->GetEntityManager()->CreateComponent<RootForce::Camera>(cameraEntity);
		camera->m_near = 0.1f;
		camera->m_far = 1000.0f;
		camera->m_fov = 45.0f;
		RootForce::Transform* cameraTransform = m_world->GetEntityManager()->CreateComponent<RootForce::Transform>(cameraEntity);
		RootForce::LookAtBehavior* cameraLookAt = m_world->GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(cameraEntity);
		cameraLookAt->m_targetTag = "AimingDevice";
		cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
		RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world->GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
		cameraThirdPerson->m_targetTag = "AimingDevice";
		cameraThirdPerson->m_displacement = glm::vec3(0.0f, 4.0f, -8.0f);

		// Import test world.
		m_world->GetEntityImporter()->Import(m_engineContext->m_resourceManager->GetWorkingDirectory() + "Assets\\Levels\\test_2.world");

		m_playerSystem = std::shared_ptr<RootForce::PlayerSystem>(new RootForce::PlayerSystem(m_world, p_engineContext));
		m_playerSystem->CreatePlayer();

		m_displayPhysicsDebug = false;
		m_displayNormals = false;

		m_engineContext->m_debugOverlay->SetView(m_engineContext->m_gui->LoadURL("debug.html"));

		//Plane at bottom

		glm::vec3 normal (0,1,0);
		glm::vec3 position (0, -2, 0);

		//g_engineContext.m_physics->CreatePlane(normal, position);
		normal = glm::vec3 (0,0,-1);
		position = glm::vec3 (0,0,9);
		//	g_engineContext.m_physics->CreatePlane(normal, position);
		// Setup the skybox.
		auto e = m_world->GetTagManager()->GetEntityByTag("Skybox");
		auto r = m_world->GetEntityManager()->GetComponent<RootForce::Renderable>(e);
		r->m_material.m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(
			"SkyBox", Render::TextureType::TEXTURE_CUBEMAP);


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
		m_engineContext->m_physics->Update(p_deltaTime);
		m_engineContext->m_profiler->Update(p_deltaTime);
	}

	void Ingamestate::Render()
	{
		m_engineContext->m_renderer->Clear();

		{
			PROFILE("Player control system", m_engineContext->m_profiler);
			m_playerControlSystem->Process();
		}

		{
			PROFILE("Script system", m_engineContext->m_profiler);
			m_scriptSystem->Process();
		}

		{
			PROFILE("Ability system", m_engineContext->m_profiler);
			m_abilitySystem->Process();
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
			PROFILE("Physics", m_engineContext->m_profiler);			
			m_physicsSystem->Process();
		}
		m_playerControlSystem->UpdateAimingDevice();
		m_thirdPersonBehaviorSystem->Process();
		m_lookAtSystem->Process();
		m_cameraSystem->Process();
		m_pointLightSystem->Process();
		{
			PROFILE("RenderingSystem", m_engineContext->m_profiler);
			m_renderingSystem->Process();
		}
		m_engineContext->m_renderer->Render();

		{
			PROFILE("Render Lines", m_engineContext->m_profiler);
			m_engineContext->m_renderer->RenderLines();
		}


		


		{
			PROFILE("GUI", m_engineContext->m_profiler);

			m_engineContext->m_gui->Update();
			m_engineContext->m_gui->Render();
		}
		m_engineContext->m_debugOverlay->RenderOverlay();
		m_engineContext->m_renderer->Swap();
	}

}