#pragma once

#include <memory>
#include <string>
#include <WinSock2.h>
#include <SDL2/SDL.h>

#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

#include <RootSystems/Include/Network/Client.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world; // because LUA needs it
RootForce::Network::NetworkEntityMap g_networkEntityMap;

namespace UnitTesting
{
	class Main 
	{
	public:
		Main(std::string p_workingDirectory);
		~Main();
	private:
		bool m_running;
		std::string m_workingDirectory;
		void* m_engineModule;
		std::shared_ptr<SDL_Window> m_window;
	};
}