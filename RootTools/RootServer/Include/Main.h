#pragma once

#include <string>
#include <memory>
#include <WinSock2.h>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/MessageHandlers.h>
#include <RootSystems/Include/Network/NetworkTypes.h>

RootEngine::GameSharedContext g_engineContext;

class Main {
public:
	Main(std::string p_workingDirectory);
	~Main();

	void Start();
private:

	void HandleEvents();

	bool m_running;
	void* m_engineModule;

	ECS::World m_world;
	RootForce::Network::NetworkEntityMap m_networkEntityMap;
	std::shared_ptr<RootForce::Network::Server> m_server;
	std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;

	std::string m_workingDir;

};