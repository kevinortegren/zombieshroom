#include "guiInstance.h"
#include "Logging\Logging.h"
#include <gtest/gtest.h>
#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>
namespace RootEngine
{
	namespace GUISystem
	{
		guiInstance* guiInstance::s_gui = nullptr;
		RootEngine::SubsystemSharedContext g_context;
		void guiInstance::Startup(void)
		{
			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Succesfull startup of GUI");
		}


		void guiInstance::Shutdown(void)
		{
			delete s_gui;
		}


		guiInstance* guiInstance::GetInstance()
		{
			if(!s_gui)
				s_gui = new guiInstance();

			return s_gui;
		}

		
	}
}

RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::GUISystem::g_context = p_context;

	return RootEngine::GUISystem::guiInstance::GetInstance();
}
