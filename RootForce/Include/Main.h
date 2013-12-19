#pragma once

#include <memory>
#include <string>
#include <WinSock2.h>
#include <SDL2/SDL.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

#include <RootForce/Include/GameStates.h>
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
        bool m_running;
		std::string m_workingDirectory;
        void* m_engineModule;
        std::shared_ptr<SDL_Window> m_window;

		ECS::World m_world;

		std::shared_ptr<RootForce::Network::Client> m_client;
		std::shared_ptr<RootForce::Network::ClientMessageHandler> m_clientMessageHandler;

        GameStates::GameStates m_currentState;
		std::shared_ptr<RootForce::Menustate> m_menustate;
		std::shared_ptr<RootForce::Ingamestate> m_gamestate;
        
		bool m_displayNormals;
		bool m_displayPhysicsDebug;


		void HandleEvents();
	};
}

