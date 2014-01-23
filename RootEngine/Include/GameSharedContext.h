#pragma once

//#include <RootEngine/Include/Logging/Logging.h>
//#include <RootEngine/Include/ResourceManager/ResourceManager.h>
//#include <RootEngine/Render/Include/Renderer.h>
//#include <RootEngine/Include/Profiling.h>
//#ifndef COMPILE_LEVEL_EDITOR
//#include <RootEngine/Network/Include/NetworkManager.h>
//#include <RootEngine/GUI/Include/guiInstance.h>
//#include <RootEngine/Physics/Include/RootPhysics.h>
//#include <RootEngine/Include/DebugOverlay/DebugOverlayInterface.h>
//#include <RootEngine/InputManager/Include/InputManager.h>
//#include <RootEngine/Script/Include/ScriptManager.h>
//#include <RootEngine/Include/ConfigManager.h>
//#endif

namespace Render
{
	class RendererInterface;
}

#ifndef COMPILE_LEVEL_EDITOR
namespace Network
{
	class NetworkInterface;
}

namespace RootEngine
{
	namespace GUISystem
	{
		class GUISystemInterface;
	}
	namespace InputManager
	{
		class InputInterface;
	}
	namespace Physics
	{
		class PhysicsInterface;
	}
	namespace Script
	{
		class ScriptInterface;
	}
}

#endif

class Logging;

namespace RootEngine
{
	class MemoryTracker;
	struct ConfigManagerInterface;
	class ResourceManager;
	class ProfilingInterface;
	class DebugOverlayInterface;

	struct GameSharedContext
	{
		Logging* m_logger;
		MemoryTracker* m_memTracker;
		ConfigManagerInterface* m_configManager;
		ResourceManager* m_resourceManager;
		Render::RendererInterface* m_renderer;
		ProfilingInterface* m_profiler;
#ifndef COMPILE_LEVEL_EDITOR
		Network::NetworkInterface* m_network;
		GUISystem::GUISystemInterface* m_gui;
		DebugOverlayInterface* m_debugOverlay;
		InputManager::InputInterface* m_inputSys;
		Physics::PhysicsInterface* m_physics;
		Script::ScriptInterface* m_script;
#endif


	};
}