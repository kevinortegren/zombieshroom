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
			INIT_ALL = INIT_NETWORK | INIT_INPUT | INIT_RENDER | INIT_GUI
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
		void LoadNetwork();
		void LoadRender();
		void LoadGUI();

		void* m_networkModule;
		void* m_renderModule;
		void* m_guiModule;

		SubsystemSharedContext m_subsystemSharedContext;
		GameSharedContext m_gameSharedContext;

		MemoryTracker*	m_memTracker;
		ResourceManager m_resourceManager;
		Network::NetworkManager*		m_network;
		Render::RendererInterface*		m_renderer;
		GUISystem::GUISystemInterface*	m_gui;
	};
}

extern "C"
{
	typedef RootEngine::GameSharedContext ( *INITIALIZEENGINE )(int, std::string);
	ROOTENGINE_DLL_EXPORT RootEngine::GameSharedContext InitializeEngine(int p_flags, std::string p_workingDirectory);
}