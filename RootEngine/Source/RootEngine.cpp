#include <RootEngine.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Network/Include/NetworkInterface.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <iostream>

#include <RootEngine/Include/EffectParser.h>

namespace RootEngine
{
	EngineMain::~EngineMain()
	{
		m_network->Shutdown();
		m_renderer->Shutdown();


		DynamicLoader::FreeSharedLibrary(m_networkModule);
		DynamicLoader::FreeSharedLibrary(m_renderModule);
	}

	void EngineMain::Initialize(int flags)
	{
		std::cout << "Creating Engine Context" << std::endl;

		RootEngine::EffectParser p;
		p.Load("test.yaml");

		// Setup the subsystem context
		m_subsystemSharedContext.m_logger = &m_logger;

		// Load external dlls.
		if((flags & SubsystemInit::INIT_NETWORK) == SubsystemInit::INIT_NETWORK)
		{
			LoadNetwork();
		}
		if((flags & SubsystemInit::INIT_RENDER) == SubsystemInit::INIT_RENDER)
		{
			LoadRender();
		}

		// TODO: Load the rest of the submodules

		// Setup the game context
		m_gameSharedContext.m_logger = &m_logger;
		m_gameSharedContext.m_renderer = m_renderer;
		m_gameSharedContext.m_network = m_network;
	}

	GameSharedContext EngineMain::GetGameSharedContext()
	{
		return m_gameSharedContext;
	}

	SubsystemSharedContext EngineMain::GetSubsystemSharedContext()
	{
		return m_subsystemSharedContext;
	}


	void EngineMain::LoadNetwork()
	{
		// Load the network module
		m_networkModule = DynamicLoader::LoadSharedLibrary("Network.dll");
		if (m_networkModule != nullptr)
		{
			GETNETWORKINTERFACE libGetNetworkInterface = (GETNETWORKINTERFACE) DynamicLoader::LoadProcess(m_networkModule, "GetNetworkInterface");
			if (libGetNetworkInterface != nullptr)
			{
				m_network = (Network::NetworkManager*)libGetNetworkInterface(m_subsystemSharedContext);
				m_network->Startup();

			}
			else
			{
				m_logger.LogText(LogTag::NETWORK, 1, "Failed to load Network subsystem: %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			m_logger.LogText(LogTag::NETWORK, 1, "Failed to load Network subsystem: %s", DynamicLoader::GetLastError());
		}
	}

	void EngineMain::LoadRender()
	{
		// Load the render module
		m_renderModule = DynamicLoader::LoadSharedLibrary("Render.dll");
		if (m_renderModule != nullptr)
		{
			CREATERENDERER libGetRenderer = (CREATERENDERER) DynamicLoader::LoadProcess(m_renderModule, "CreateRenderer");
			if (libGetRenderer != nullptr)
			{
				m_renderer = (Render::GLRenderer*)libGetRenderer(m_subsystemSharedContext);
				m_renderer->Startup();

			}
			else
			{
				m_logger.LogText(LogTag::RENDER, 1, "Failed to load Render subsystem: %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			m_logger.LogText(LogTag::RENDER, 1, "Failed to load Render subsystem: %s", DynamicLoader::GetLastError());
		}
	}
}

namespace RootEngine
{
	extern RootEngine::EngineMain* g_engineMain;
}

RootEngine::GameSharedContext InitializeEngine(int flags)
{
	RootEngine::g_engineMain->Initialize(flags);
	return RootEngine::g_engineMain->GetGameSharedContext();
}
