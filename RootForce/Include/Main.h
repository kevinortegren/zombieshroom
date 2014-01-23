#pragma once

#include <memory>
#include <string>
#include <WinSock2.h>
#include <SDL2/SDL.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

#include <RootSystems/Include/Network/NetworkTypes.h>
#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

#include <RootForce/Include/GameStates.h>
#include <RootForce/Include/MenuState.h>
#include <RootForce/Include/ConnectingState.h>
#include <RootForce/Include/IngameState.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;
RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace RootForce
{
	class Main 
	{
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

		NetworkContext m_networkContext;
		SharedSystems m_sharedSystems;

        GameStates::GameStates m_currentState;
		std::shared_ptr<RootForce::MenuState> m_menuState;
		std::shared_ptr<RootForce::ConnectingState> m_connectingState;
		std::shared_ptr<RootForce::IngameState> m_ingameState;


		void HandleEvents();
	};
}

