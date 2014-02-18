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
#include <RootSystems/Include/WorldSystem.h>
#include <RootSystems/Include/AbilityRespawnSystem.h>

RootEngine::GameSharedContext g_engineContext;
ECS::World* g_world;
RootForce::Network::NetworkEntityMap g_networkEntityMap;

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
	std::shared_ptr<RootForce::Network::Server> m_server;
	std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;

	std::string m_workingDir;
	
	// System responsible for creating the world.
	std::shared_ptr<RootForce::WorldSystem> m_worldSystem;
	RootForce::AbilityRespawnSystem* m_abilityRespawnSystem;
};