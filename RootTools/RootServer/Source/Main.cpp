#include <Main.h>

#include <stdexcept>
#include <exception>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>
#include <RootSystems/Include/Components.h>

#include <RootSystems/Include/Network/Messages.h>
#include "ConfigLoader.h"
#include "ConsoleInput.h"

#undef main

/*
TEST(Test, Foo) 
{
	int a = 0;
	EXPECT_TRUE(a == 0);
}*/

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Rootserver.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		/*
		if (argc > 1 && strcmp(argv[1], "-test") == 0)
		{
			testing::InitGoogleTest(&argc, argv);

			int result = RUN_ALL_TESTS();
			//std::cin.get();
			return result;
		}
		else
		{*/
			Main m(path);
			m.Start();
		//}
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		std::cout << e.what() << "\n";
		//std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		//std::cin.get();
		return 1;
	}
	
	return 0;
}

Main::Main(std::string p_workingDirectory) 
	: m_running(true)
{

	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL ^ RootEngine::SubsystemInit::INIT_GUI ^ RootEngine::SubsystemInit::INIT_RENDER, p_workingDirectory);
	
	m_workingDir = p_workingDirectory;
}

Main::~Main() 
{
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	// Register the components the server uses
    RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
    RootForce::Player::SetTypeId(RootForce::ComponentType::PLAYER);
    RootForce::Physics::SetTypeId(RootForce::ComponentType::PHYSICS);
    RootForce::Network::NetworkClientComponent::SetTypeId(RootForce::ComponentType::NETWORKCLIENT);
    RootForce::Network::NetworkComponent::SetTypeId(RootForce::ComponentType::NETWORK);
    RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
    RootForce::Script::SetTypeId(RootForce::ComponentType::SCRIPT);
    RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
    RootForce::CollisionResponder::SetTypeId(RootForce::ComponentType::COLLISIONRESPONDER);

	RootServer::ConsoleInput m_console;
	m_console.Startup( );
		
	// Load the server config file
	RootSystems::ServerConfig conf = RootServer::ConfigLoader(m_engineContext.m_logger).Load(m_workingDir + "server.conf");
	
	RootForce::Network::MessagePlayData response;
	response.ServerName = "Dedicated Server";
	response.MapName = conf.LevelFile.c_str();
	response.MaxPlayers = (uint8_t) conf.MaxPlayers;
	response.MatchLength = (uint16_t) conf.MatchTime;
	response.KillCount = (uint8_t) conf.KillCount;

	// Initialize the server
	m_networkEntityMap = std::shared_ptr<RootForce::Network::NetworkEntityMap>(new RootForce::Network::NetworkEntityMap);
	m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(m_engineContext.m_logger, "Dedicated Server", 5567));
	m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_server->GetPeerInterface(), m_engineContext.m_logger, &m_world));
	m_serverMessageHandler->SetNetworkEntityMap(m_networkEntityMap.get());
	m_serverMessageHandler->SetPlayDataResponse(response);
	m_server->SetMessageHandler(m_serverMessageHandler.get());

	// Start the main loop
	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;
	
		m_world.SetDelta(dt);
		
		//m_playerControlSystem->Process();
		m_server->Update();
		m_engineContext.m_physics->Update(dt);

		// ToDo: Move event handling to shared server-logic class
		RootServer::EventData consoleEvent = m_console.PollEvent();
		switch(consoleEvent.EventType)
		{
		case RootServer::ServerEvents::NEW_PLAYER:
			// TODO: 
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "NEW_PLAYER not yet implemented.");
			break;
		case RootServer::ServerEvents::REMOVE_PLAYER:
			// TODO
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "REMOVE_PLAYER not yet implemented.");
			break;
		case RootServer::ServerEvents::KICK_PLAYER:
			// TODO
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "KICK_PLAYER not yet implemented.");
			break;
		case RootServer::ServerEvents::BROADCAST_MESSAGE:
			// TODO
			//Message m;
			//m.messageID = CHAT_TO_CLIENT;
			//m_networkHandler->Send(m);
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "say not yet implemented.");
			break;
		case RootServer::ServerEvents::RESET_SERVER:
			// TODO: How to handle server reset?
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "reset not yet implemented.");
			break;
		case RootServer::ServerEvents::SET_MAXPLAYERS:
			if(consoleEvent.DataSize == sizeof(uint8_t))
				conf.MaxPlayers = *consoleEvent.Data;
			// TODO: Call the network to change limit of accepted connections
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_maxplayers not yet implemented.");
			break;
		case RootServer::ServerEvents::SET_PASSWORD:
			if(consoleEvent.DataSize > 0)
				conf.Password = std::string((char*)consoleEvent.Data, consoleEvent.DataSize);
			// TODO: Call the network to change the password required for the connection
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_password not yet implemented.");
			break;
		case RootServer::ServerEvents::SET_MAP:
			if(consoleEvent.DataSize > 0)
				conf.LevelFile = std::string((const char*)consoleEvent.Data, consoleEvent.DataSize);
			// TODO: Reset the server with a new map loaded
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_map not yet implemented.");
			break;
		case RootServer::ServerEvents::SET_GAMEMODE:
			if(consoleEvent.DataSize == sizeof(uint8_t))
				conf.GameMode = (RootSystems::GameMode::GameMode)*consoleEvent.Data;
			// TODO: Reset the server with a different gamemode
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_gamemode not yet implemented.");
			break;
		case RootServer::ServerEvents::SET_MATCHTIME:
			if(consoleEvent.DataSize == sizeof(uint32_t))
				conf.MatchTime = *(uint32_t*)consoleEvent.Data;
			break;
		case RootServer::ServerEvents::SET_KILLCOUNT:
			if(consoleEvent.DataSize == sizeof(uint32_t))
				conf.MatchTime = *(uint32_t*)consoleEvent.Data;
			break;
		case RootServer::ServerEvents::SHUTDOWN:
			m_running = false;
			break;
		case RootServer::ServerEvents::CHEATMODE:
			// TODO: Unleash your wildest imagination!
			m_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "Secret cheatmode not yet secretly implemented.");
			break;
		default:
			break;
		}
	}

	m_console.Shutdown();
}
