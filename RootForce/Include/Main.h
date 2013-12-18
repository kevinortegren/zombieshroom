#pragma once

#include <WinSock2.h>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/RenderingSystem.h>
#include <RootSystems/Include/LightSystem.h>
#include <RootSystems/Include/PlayerSystem.h>
#include <RootSystems/Include/PlayerControlSystem.h>
#include <RootSystems/Include/AbilitySystem.h>

//#include <RootSystems/Include/Network/MessageHandler.h>
#include <RootSystems/Include/Network/LanList.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <ChatSystem.h>

#include <memory>
#include <string>

#include <RootForce/Include/Menustate.h>
#include <RootForce/Include/Ingamestate.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;

namespace RootForce
{

	class Main {
	public:
		Main(std::string p_workingDirectory);
		~Main();

		void Start();
	private:
        void HandleEvents();
        void testfuncofawesome(std::vector<glm::vec3> p_debugVectors);
        bool m_running;
		std::string m_workingDirectory;
        void* m_engineModule;
        std::shared_ptr<SDL_Window> m_window;

		ECS::World m_world;
		
        
		std::shared_ptr<RootForce::PlayerControlSystem> m_playerControlSystem;
		std::shared_ptr<RootForce::PlayerSystem> m_playerSystem;

		RootForce::ChatSystem m_chat;
		RootSystems::LanList m_lanList;
		std::shared_ptr<RootForce::Network::Server> m_server;
		std::shared_ptr<RootForce::Network::Client> m_client;
		std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;
		std::shared_ptr<RootForce::Network::ClientMessageHandler> m_clientMessageHandler;

        GameStates::GameStates m_currentState;
		std::shared_ptr<RootForce::Menustate> m_menustate;
		std::shared_ptr<RootForce::Ingamestate> m_gamestate;
        
		bool m_displayNormals;
		bool m_displayPhysicsDebug;
	};
}

