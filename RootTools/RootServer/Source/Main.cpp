#include <Main.h>

#include <stdexcept>
#include <exception>
#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <RootSystems/Include/Network/Messages.h>
#include "ConfigLoader.h"
#include "ConsoleInput.h"

#undef main

TEST(Test, Foo) 
{
	int a = 0;
	EXPECT_TRUE(a == 0);
}

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Rootserver.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		if (argc > 1 && strcmp(argv[1], "-test") == 0)
		{
			testing::InitGoogleTest(&argc, argv);

			int result = RUN_ALL_TESTS();
			//std::cin.get();
			return result;
		}
		else
		{
			Main m(path);
			m.Start();
		}
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
	: m_running(true), m_world(5)
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
	// Initialize the network system
	m_networkHandler = std::shared_ptr<RootForce::Network::MessageHandler>(new RootForce::Network::MessageHandler(&m_world, m_engineContext.m_logger, m_engineContext.m_network, RootForce::Network::MessageHandler::DEDICATED, 5567));
	
	RootServer::ConsoleInput m_console;
	m_console.Startup( );

	// Start the main loop
	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;
	
		m_world.SetDelta(dt);
		
		//m_playerControlSystem->Process();
		m_networkHandler->Update();
		m_engineContext.m_physics->Update(dt);
		
		// Load the server config file
		RootSystems::ServerConfig conf = RootServer::ConfigLoader(m_engineContext.m_logger).Load(m_workingDir + "server.conf");
		

	}

	m_console.Shutdown();
}
