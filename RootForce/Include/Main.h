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
#include <RootSystems/Include/Network/MessageHandler.h>
#include "ChatSystem.h"
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
		
		std::shared_ptr<RootForce::Network::MessageHandler> m_networkHandler;

		GameStates::GameStates m_currentState;
		std::shared_ptr<RootForce::Menustate> m_menustate;
		std::shared_ptr<RootForce::Ingamestate> m_gamestate;
	};
}

