#pragma once

#include <memory>
#include <WinSock2.h>
#include <SDL2/SDL.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <Utility/ECS/Include/World.h>

//#include <RootSystems/Include/Network/MessageHandler.h>
#include <RootSystems/Include/Network/Server.h>
#include <RootSystems/Include/Network/MessageHandlers.h>

#include <string>

class Main {
public:
	Main(std::string p_workingDirectory);
	~Main();

	void Start();
private:

	void HandleEvents();
	void testfuncofawesome(std::vector<glm::vec3> p_debugVectors);
	bool m_running;
	void* m_engineModule;
	RootEngine::GameSharedContext m_engineContext;

	ECS::World m_world;
	std::shared_ptr<RootForce::Network::Server> m_server;
	std::shared_ptr<RootForce::Network::ServerMessageHandler> m_serverMessageHandler;

	std::string m_workingDir;
};