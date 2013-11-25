#pragma once

#include "guiSysInterface.h"
#include "guiRenderInterface.h"
#include "guiFileInterface.h"

#include <RootEngine/Include/SubsystemSharedContext.h>

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
			virtual void Initalize(float p_width, float p_height) = 0;

			//Attach a .rml document to the root context
			virtual bool AttachDocument(std::string p_path) = 0; 

			//Updates the elapsed time as well as updates and renders the gui context
			virtual void Update(float p_time) = 0;
		};

		class guiInstance : public GUISystemInterface
		{
		public:
			void Startup();
			void Shutdown();
			void Initalize(float p_width, float p_height);

			bool AttachDocument(std::string p_path); 

			void Update(float p_time);

			static guiInstance* GetInstance();



		private:

			guiSysInterface* m_sysInterface;
			guiRenderInterface* m_renderInterface;
			guiFileInterface* m_fileInterface;

			Rocket::Core::Context* m_rootContext;

			static guiInstance* s_gui;
		};
	}

}

extern "C"
{
	typedef RootEngine::GUISystem::GUISystemInterface* (*CREATEGUI)(RootEngine::SubsystemSharedContext);
	SUBSYSTEM_DLL_EXPORT RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context);
}