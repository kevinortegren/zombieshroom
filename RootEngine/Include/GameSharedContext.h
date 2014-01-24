#pragma once

namespace Render
{
	class RendererInterface;
}

#ifndef COMPILE_LEVEL_EDITOR

namespace RootEngine
{
	namespace Network
	{
		class NetworkInterface;
	}
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