#pragma once

#if defined(_WINDLL)
	#define ROOTENGINE_DLL_EXPORT __declspec(dllexport)
#else
	#define ROOTENGINE_DLL_EXPORT __declspec(dllimport)
#endif

#include <memory>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/Memory/MemoryTracker.h>

namespace RootEngine
{
	namespace SubsystemInit
	{
		enum SubsystemInit
		{
			INIT_NETWORK = 1,
			INIT_INPUT = 2,
			INIT_RENDER = 4,
			INIT_GUI = 8,
			INIT_PHYSICS = 16,
			INIT_ALL = INIT_NETWORK | INIT_INPUT | INIT_RENDER | INIT_GUI | INIT_PHYSICS
		};
	}

	class EngineMain
	{
	public:
		~EngineMain();

		void Initialize(int flags);
		GameSharedContext GetGameSharedContext();
		SubsystemSharedContext GetSubsystemSharedContext();
	private:
		void LoadNetwork();
		void LoadRender();
		void LoadGUI();
		void LoadPhysics();
		void* m_networkModule;
		void* m_renderModule;
		void* m_guiModule;
		void* m_physicsModule;
		SubsystemSharedContext m_subsystemSharedContext;
		GameSharedContext m_gameSharedContext;

		Logging m_logger;
		MemoryTracker* m_memTracker;
		Network::NetworkManager* m_network;
		Render::RendererInterface* m_renderer;
		GUISystem::GUISystemInterface* m_gui;
		Physics::PhysicsInterface* m_physics;
		
	};
}

extern "C"
{
	typedef RootEngine::GameSharedContext ( *INITIALIZEENGINE )(int);
	ROOTENGINE_DLL_EXPORT RootEngine::GameSharedContext InitializeEngine(int flags);
}