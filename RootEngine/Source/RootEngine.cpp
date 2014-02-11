#include <RootEngine.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <iostream>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Script/Include/RootScript.h>

#ifndef COMPILING_LEVEL_EDITOR
#include <RootEngine/Network/Include/NetworkManager.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/Include/DebugOverlay/DebugOverlay.h>
#include <RootEngine/Include/Profiling.h>
#include <RootEngine/InputManager/Include/InputManager.h>
#endif

Logging	g_logger;

namespace RootEngine
{
	EngineMain::EngineMain()
	{

	}
	EngineMain::~EngineMain()
	{
		
#ifndef COMPILING_LEVEL_EDITOR

		if(m_network != nullptr)
		{
			m_network->Shutdown();
			DynamicLoader::FreeSharedLibrary(m_networkModule);
		}

		if(m_physics != nullptr)
		{
			m_physics->Shutdown();
			DynamicLoader::FreeSharedLibrary(m_physicsModule);
		}

		if(m_scriptEngine != nullptr)
		{
			m_scriptEngine->Shutdown();
			DynamicLoader::FreeSharedLibrary(m_scriptModule);
		}

		if(m_gui != nullptr)
		{
			m_gui->Shutdown();
			DynamicLoader::FreeSharedLibrary(m_guiModule);
		}
		
#endif
		if(m_renderer != nullptr)
		{
			m_renderer->Shutdown();
			DynamicLoader::FreeSharedLibrary(m_renderModule);
		}
	}

	void EngineMain::Initialize(int p_flags, std::string p_workingDirectory)
	{
		g_logger.LogText(LogTag::GENERAL, LogLevel::INIT_PRINT, "Started initializing engine context!");

		m_configManager.LoadConfig("config.yaml");

#ifndef COMPILING_LEVEL_EDITOR
		m_network = nullptr;
		m_gui = nullptr;
		m_physics = nullptr;
		m_scriptEngine = nullptr;
#endif
		m_renderer = nullptr;

		// Setup the subsystem context
		m_subsystemSharedContext.m_logger = &g_logger;
		m_subsystemSharedContext.m_memTracker = m_memTracker;
		m_subsystemSharedContext.m_resourceManager = &m_resourceManager;
		m_subsystemSharedContext.m_configManager = &m_configManager;
		m_subsystemSharedContext.m_profiler = &m_profiler;

		if((p_flags & SubsystemInit::INIT_RENDER) == SubsystemInit::INIT_RENDER)
		{
			LoadRender();
		}

#ifndef COMPILING_LEVEL_EDITOR
				
		m_subsystemSharedContext.m_debugOverlay = &m_debugOverlay;

		// Load external dlls.
		if((p_flags & SubsystemInit::INIT_NETWORK) == SubsystemInit::INIT_NETWORK)
		{
			LoadNetwork();
		}
		if((p_flags & SubsystemInit::INIT_INPUT) == SubsystemInit::INIT_INPUT)
		{
			LoadInput();
		}
		if((p_flags & SubsystemInit::INIT_GUI) == SubsystemInit::INIT_GUI)
		{
			LoadGUI();
			m_gui->SetWorkingDir(p_workingDirectory);
		}
		if((p_flags & SubsystemInit::INIT_PHYSICS) == SubsystemInit::INIT_PHYSICS)
		{
			LoadPhysics();
		}

		if((p_flags & SubsystemInit::INIT_SCRIPTING) == SubsystemInit::INIT_SCRIPTING)
		{
			LoadScriptEngine();
			m_scriptEngine->SetWorkingDir(p_workingDirectory);
		}
#endif
		// TODO: Load the rest of the submodules

		// Setup the game context

		m_memTracker = new MemoryTracker(&g_logger);
		m_profiler.SetMemoryTracker(m_memTracker);
		m_profiler.SetRenderInterface(m_renderer);

		m_gameSharedContext.m_logger = &g_logger;
		m_gameSharedContext.m_memTracker = m_memTracker;
		m_gameSharedContext.m_configManager = &m_configManager;
		m_gameSharedContext.m_resourceManager = &m_resourceManager;
		m_gameSharedContext.m_renderer = m_renderer;
		m_gameSharedContext.m_profiler = &m_profiler;
#ifndef COMPILING_LEVEL_EDITOR
		m_gameSharedContext.m_debugOverlay = m_subsystemSharedContext.m_debugOverlay;
		m_gameSharedContext.m_inputSys = m_inputSys;
		m_gameSharedContext.m_network = m_network;
		m_gameSharedContext.m_gui = m_gui;
		m_gameSharedContext.m_physics = m_physics;
		m_gameSharedContext.m_inputSys = m_inputSys;
		m_gameSharedContext.m_script = m_scriptEngine;
		m_gameSharedContext.m_profiler->SetDebugOverlay(m_subsystemSharedContext.m_debugOverlay);

#endif

		m_resourceManager.Init(p_workingDirectory, &m_gameSharedContext);

		g_logger.LogText(LogTag::GENERAL, LogLevel::INIT_PRINT, "Engine Context initialized!");
	}

	GameSharedContext EngineMain::GetGameSharedContext()
	{
		return m_gameSharedContext;
	}

	SubsystemSharedContext EngineMain::GetSubsystemSharedContext()
	{
		return m_subsystemSharedContext;
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
				g_logger.LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Failed to load Render subsystem: %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			g_logger.LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Failed to load Render subsystem: %s", DynamicLoader::GetLastError());
		}
	}

#ifndef COMPILING_LEVEL_EDITOR

	void EngineMain::LoadNetwork()
	{
		// Load the network module
		m_networkModule = DynamicLoader::LoadSharedLibrary("Network.dll");
		if (m_networkModule != nullptr)
		{
			GETNETWORKINTERFACE libGetNetworkInterface = (GETNETWORKINTERFACE) DynamicLoader::LoadProcess(m_networkModule, "CreateNetwork");
			if (libGetNetworkInterface != nullptr)
			{
				m_network = (Network::NetworkManager*)libGetNetworkInterface(m_subsystemSharedContext);
				m_network->Startup();

			}
			else
			{
				g_logger.LogText(LogTag::NETWORK,  LogLevel::FATAL_ERROR, "Failed to load Network subsystem: %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			g_logger.LogText(LogTag::NETWORK,  LogLevel::FATAL_ERROR, "Failed to load Network subsystem: %s", DynamicLoader::GetLastError());
		}
	}

	void EngineMain::LoadInput()
	{
		// Load the input module
		m_inputModule = DynamicLoader::LoadSharedLibrary("InputManager.dll");
		if (m_inputModule != nullptr)
		{
			CREATEINPUTINTERFACE libCreateInputInterface = (CREATEINPUTINTERFACE) DynamicLoader::LoadProcess(m_inputModule, "CreateInputSystem");
			if (libCreateInputInterface != nullptr)
			{
				m_inputSys = (RootEngine::InputManager::InputInterface*)libCreateInputInterface(m_subsystemSharedContext);
				m_inputSys->Startup();

			}
			else
			{
				g_logger.LogText(LogTag::INPUT,  LogLevel::FATAL_ERROR, "Failed to load Input subsystem: %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			g_logger.LogText(LogTag::INPUT,  LogLevel::FATAL_ERROR, "Failed to load Input subsystem: %s", DynamicLoader::GetLastError());
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
				g_logger.LogText(LogTag::GUI, LogLevel::FATAL_ERROR, "Failed to load GUI subsystem: %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			g_logger.LogText(LogTag::GUI, LogLevel::FATAL_ERROR, "Failed to load GUI subsystem: %s", DynamicLoader::GetLastError());
		}
	}

	void EngineMain::LoadPhysics()
	{
		m_physicsModule = DynamicLoader::LoadSharedLibrary("Physics.dll");
		if(m_physicsModule != nullptr)
		{
			CREATEPHYSICS libGetPhysics = (CREATEPHYSICS) DynamicLoader::LoadProcess(m_physicsModule, "CreatePhysics");
			if(libGetPhysics != nullptr)
			{
				m_physics = (Physics::RootPhysics*)libGetPhysics(m_subsystemSharedContext, m_renderer, &m_resourceManager);
				m_physics->Startup();
				
			}
			else
			{
				m_logger.LogText(LogTag::PHYSICS, LogLevel::FATAL_ERROR, "Failed to load physics subsystem %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			m_logger.LogText(LogTag::PHYSICS, LogLevel::FATAL_ERROR, "Failed to load physics subsystem %s", DynamicLoader::GetLastError());
		}
	}

	void EngineMain::LoadScriptEngine()
	{
		m_scriptModule = DynamicLoader::LoadSharedLibrary("Script.dll");
		if(m_scriptModule != nullptr)
		{
			CREATESCRIPTINTERFACE libGetScriptEngine = (CREATESCRIPTINTERFACE) DynamicLoader::LoadProcess(m_scriptModule, "CreateScriptInterface");

			if(libGetScriptEngine != nullptr)
			{
				m_scriptEngine = (Script::ScriptInterface*)libGetScriptEngine(m_subsystemSharedContext);
				m_scriptEngine->Startup();
			}
			else
			{
				m_logger.LogText(LogTag::SCRIPT, LogLevel::FATAL_ERROR, "Failed to load script subsystem %s", DynamicLoader::GetLastError());
			}
		}
		else
		{
			m_logger.LogText(LogTag::SCRIPT, LogLevel::FATAL_ERROR, "Failed to load script subsystem %s", DynamicLoader::GetLastError());
		}
		
	}
	
#endif

}

namespace RootEngine
{
	extern RootEngine::EngineMain* g_engineMain;
}

RootEngine::GameSharedContext InitializeEngine(int p_flags, std::string p_workingDirectory)
{
	RootEngine::g_engineMain->Initialize(p_flags, p_workingDirectory);
	return RootEngine::g_engineMain->GetGameSharedContext();
}
