#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>
#include <string>

#if defined(_WINDLL)
    #define SUBSYSTEM_DLL_EXPORT __declspec(dllexport)
#else
    #define SUBSYSTEM_DLL_EXPORT __declspec(dllimport)
#endif

namespace RootEngine
{
	namespace GUISystem
	{
		class GUISystemInterface abstract : public RootEngine::SubsystemInterface
		{
		public:

		};

		class guiInstance : public GUISystemInterface
		{
		public:
			void Startup();
			void Shutdown();

			static guiInstance* GetInstance();

		private:
			static guiInstance* s_gui;
		};
	}

}

extern "C"
{
	typedef RootEngine::GUISystem::GUISystemInterface* (*CREATEGUI)(RootEngine::SubsystemSharedContext);
	SUBSYSTEM_DLL_EXPORT RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context);
}