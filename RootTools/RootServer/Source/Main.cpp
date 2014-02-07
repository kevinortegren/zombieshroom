#include <Main.h>

#include <stdexcept>
#include <exception>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>
#include <RootSystems/Include/Components.h>

#include <RootSystems/Include/Network/Messages.h>
#include "ConfigLoader.h"
#include "ConsoleInput.h"
#include <ComponentExporter.h>
#include <ComponentImporter.h>
#include <RootEngine/Script/Include/RootScript.h>

#include <RootForce/Include/LuaAPI.h>

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

	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL ^ RootEngine::SubsystemInit::INIT_GUI ^ RootEngine::SubsystemInit::INIT_RENDER, p_workingDirectory);
	
	m_workingDir = p_workingDirectory;
}

Main::~Main() 
{
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	// Register the components the server uses
	RootForce::ComponentType::InitializeServerComponents();
	//RootForce::ComponentType::Initialize();

	RootForce::LuaAPI::RegisterLuaTypes(g_engineContext.m_script->GetLuaState());
	g_engineContext.m_resourceManager->LoadScript("Global");

	// Setup the importer/exporter
	g_world = &m_world;
	m_world.GetEntityExporter()->SetExporter(Exporter);
	m_world.GetEntityImporter()->SetImporter(Importer);

	// Startup the console
	RootServer::ConsoleInput m_console;
	m_console.Startup( );
		
	// Load the server config file
	RootSystems::ServerConfig conf = RootServer::ConfigLoader(g_engineContext.m_logger).Load(m_workingDir + "server.conf");

	// Create a server information entity
	ECS::Entity* serverInfoEntity = m_world.GetEntityManager()->CreateEntity();
	m_world.GetEntityManager()->CreateComponent<RootForce::Network::ServerInformationComponent>(serverInfoEntity);
	m_world.GetTagManager()->RegisterEntity("ServerInformation", serverInfoEntity);

	m_worldSystem = std::shared_ptr<RootForce::WorldSystem>(new RootForce::WorldSystem(&m_world, &g_engineContext));

	// Initialize the server
	m_server = std::shared_ptr<RootForce::Network::Server>(new RootForce::Network::Server(g_engineContext.m_logger, &m_world, conf));
	m_serverMessageHandler = std::shared_ptr<RootForce::Network::ServerMessageHandler>(new RootForce::Network::ServerMessageHandler(m_server->GetPeerInterface(), &m_world));
	m_server->SetMessageHandler(m_serverMessageHandler.get());
	m_server->Initialize(m_worldSystem.get(), conf, true);

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
		g_engineContext.m_physics->Update(dt);

		// ToDo: Move event handling to shared server-logic class
		RootServer::EventData consoleEvent = m_console.PollEvent();
		
		if(consoleEvent.EventType.compare("KICK_PLAYER") == 0)
		{
			// TODO
			g_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "KICK_PLAYER not yet implemented.");
		}
		else if(consoleEvent.EventType.compare("BROADCAST_MESSAGE") == 0)
		{
			RootForce::NetworkMessage::Chat c;
			c.Message = consoleEvent.Data.str().c_str();
			c.Sender = 0;
			c.Type = RootForce::NetworkMessage::Chat::TYPE_SERVER_MESSAGE;

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID) RootForce::NetworkMessage::MessageType::Chat);
			c.Serialize(true, &bs);

			m_server->GetPeerInterface()->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}
		else if(consoleEvent.EventType.compare("RESET_SERVER") == 0)
		{
			// TODO: How to handle server reset?
			g_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "reset not yet implemented.");
		}
		else if(consoleEvent.EventType.compare("SET_MAXPLAYERS") == 0)
		{
			consoleEvent.Data >> conf.MaxPlayers;
			// TODO: Call the network to change limit of accepted connections
			g_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_maxplayers not yet implemented.");
		}
		else if(consoleEvent.EventType.compare("SET_PASSWORD") == 0)
		{
			conf.Password = consoleEvent.Data.str().c_str();
			// TODO: Call the network to change the password required for the connection
			g_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_password not yet implemented.");
		}
		else if(consoleEvent.EventType.compare("SET_MAP") == 0)
		{
			consoleEvent.Data >> conf.MapName;
			// TODO: Reset the server with a new map loaded
			g_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_map not yet implemented.");
		}
		else if(consoleEvent.EventType.compare("SET_GAMEMODE") == 0)
		{
			uint8_t tmp;
			consoleEvent.Data >> tmp;
			conf.GameMode = (RootSystems::GameMode::GameMode)tmp;
			// TODO: Reset the server with a different gamemode
			g_engineContext.m_logger->LogText(LogTag::GENERAL, LogLevel::NON_FATAL_ERROR, "set_gamemode not yet implemented.");
		}
		else if(consoleEvent.EventType.compare("SET_MATCHTIME") == 0)
		{
			consoleEvent.Data >> conf.MatchTime;
		}
		else if(consoleEvent.EventType.compare("SET_KILLCOUNT") == 0)
		{
			consoleEvent.Data >> conf.KillCount;
		}
		else if(consoleEvent.EventType.compare("SHUTDOWN") == 0)
		{
			m_running = false;
		}
	}

	m_console.Shutdown();
}
