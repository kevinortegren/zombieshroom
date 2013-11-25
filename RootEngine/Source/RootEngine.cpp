#include <RootEngine.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Network/Include/NetworkInterface.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <iostream>

namespace RootEngine
{
	EngineMain::~EngineMain()
	{
		m_network->Shutdown();

		m_renderer->Shutdown();
		m_gui->Shutdown();


		DynamicLoader::FreeSharedLibrary(m_networkModule);
		DynamicLoader::FreeSharedLibrary(m_renderModule);
		DynamicLoader::FreeSharedLibrary(m_guiModule);
	}

	void EngineMain::Initialize(int flags)
	{
		std::cout << "Creating Engine Context" << std::endl;
		
		m_memTracker = new MemoryTracker(&m_logger);
		
		// Setup the subsystem context
		m_subsystemSharedContext.m_logger = &m_logger;
		m_subsystemSharedContext.m_memTracker = m_memTracker;

		// Load external dlls.
		if((flags & SubsystemInit::INIT_NETWORK) == SubsystemInit::INIT_NETWORK)
		{
			LoadNetwork();
		}
		if((flags & SubsystemInit::INIT_RENDER) == SubsystemInit::INIT_RENDER)
		{
			LoadRender();
		}
		if((flags & SubsystemInit::INIT_GUI) == SubsystemInit::INIT_GUI)
		{
			LoadGUI();
		}

		// TODO: Load the rest of the submodules

		// Setup the game context
		m_gameSharedContext.m_logger = &m_logger;
		m_gameSharedContext.m_memTracker = m_memTracker;
		m_gameSharedContext.m_renderer = m_renderer;
		m_gameSharedContext.m_network = m_network;
		m_gameSharedContext.m_gui = m_gui;
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

	void EngineMain::LoadGUI()
	{
		m_guiModule = DynamicLoader::LoadSharedLibrary("GUI.dll");
		if(m_guiModule != nullptr)
		{
			CREATEGUI libGetGUI = (CREATEGUI) DynamicLoader::LoadProcess(m_guiModule, "CreateGUI");
			if (libGetGUI != nullptr)
			{
				m_gui = (GUISystem::guiInstance*)libGetGUI(m_subsystemSharedContext);
				m_gui->Startup();
			}
			else
			{
				m_logger.LogText(LogTag::GUI, 1, "Failed to load GUI subsystem: %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			m_logger.LogText(LogTag::GUI, 1, "Failed to load GUI subsystem: %s", DynamicLoader::GetLastError());
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
