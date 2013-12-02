#pragma once

#if defined(_WINDLL)
	#define ROOTENGINE_DLL_EXPORT __declspec(dllexport)
#else
	#define ROOTENGINE_DLL_EXPORT __declspec(dllimport)
#endif

#include <memory>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/Memory/MemoryTracker.h>
#include <RootEngine/Include/YamlParser.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

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
		EngineMain();
		~EngineMain();

		void Initialize(int p_flags, std::string p_workingDirectory);
		GameSharedContext GetGameSharedContext();
		SubsystemSharedContext GetSubsystemSharedContext();
	private:
		void LoadRender();
#ifndef COMPILE_LEVEL_EDITOR
		void LoadNetwork();
		void LoadInput();
		void LoadGUI();
		void LoadPhysics();
#endif

		void* m_networkModule;
		void* m_renderModule;
		void* m_guiModule;
		void* m_inputModule;
		void* m_physicsModule;
		SubsystemSharedContext m_subsystemSharedContext;
		GameSharedContext m_gameSharedContext;

		Logging m_logger;
		MemoryTracker* m_memTracker;
		ResourceManager m_resourceManager;
		Render::RendererInterface* m_renderer;

#ifndef COMPILE_LEVEL_EDITOR
		Network::NetworkManager* m_network;
		GUISystem::GUISystemInterface* m_gui;
		InputManager::InputInterface* m_inputSys;
		Physics::PhysicsInterface* m_physics;
#endif

	};
}

extern "C"
{
	typedef RootEngine::GameSharedContext ( *INITIALIZEENGINE )(int, std::string);
	ROOTENGINE_DLL_EXPORT RootEngine::GameSharedContext InitializeEngine(int p_flags, std::string p_workingDirectory);
}