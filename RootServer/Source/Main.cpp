#include <Main.h>

#include <stdexcept>
#include <exception>
#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include "ConsoleInput.h"

//#include <Network/Messages.h>

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
	: m_running(true)
{

	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL ^ RootEngine::SubsystemInit::INIT_GUI ^ RootEngine::SubsystemInit::INIT_RENDER, p_workingDirectory);
}

Main::~Main() 
{
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	//m_engineContext.m_resourceManager->LoadEffect("Mesh");
	//m_engineContext.m_resourceManager->LoadCollada("testchar");

	//// Cube mesh.
	//std::shared_ptr<Render::Mesh> cubeMesh = m_engineContext.m_renderer->CreateMesh();
	//Utility::Cube cube(Render::VertexType::VERTEXTYPE_1P);
	//cubeMesh->m_vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
	//cubeMesh->m_elementBuffer = m_engineContext.m_renderer->CreateBuffer();
	//cubeMesh->m_vertexAttributes = m_engineContext.m_renderer->CreateVertexAttributes();
	//cubeMesh->CreateIndexBuffer(cube.m_indices, cube.m_numberOfIndices);
	//cubeMesh->CreateVertexBuffer1P(reinterpret_cast<Render::Vertex1P*>(cube.m_vertices), cube.m_numberOfVertices);

	// Initialize the network system
	//m_networkHandler = std::shared_ptr<RootForce::Network::MessageHandler>(new RootForce::Network::MessageHandler());
	//m_networkHandler->Init(&m_world, m_engineContext.m_logger, m_engineContext.m_network, RootForce::Network::ServerType::LOCAL, 5567);
	
	// Test message sending
	/*{
		RootForce::Network::MessageChat* chat = new RootForce::Network::MessageChat;
		chat->Type = RootForce::Network::MessageChat::TYPE_CHAT;
		chat->SenderID = 0;
		chat->Message = "Hello world of doom";

		size_t size = 0;
		size += sizeof(chat->Type);
		size += sizeof(chat->SenderID);
		size += strlen(chat->Message);

		RootEngine::Network::Message m;
		m.MessageID = RootForce::Network::MessageType::ChatToServer;
		m.RecipientID = 1;
		m.Reliability = RELIABLE;
		m.DataSize = size;
		m.Data = (uint8_t*) chat;

		m_engineContext.m_network->GetNetworkSystem()->Send(m);
	}*/

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
		//m_networkHandler->Update();
		std::vector<std::string> command;
		while( command = m_console.PollCommand(), command.size() > 0 )
		{
			if( command[0].compare("quit") == 0
				|| command[0].compare("exit") == 0 )
			{
				m_running = false;
			}
		}

		m_engineContext.m_physics->Update(dt);
	}

	m_console.Shutdown();
}
