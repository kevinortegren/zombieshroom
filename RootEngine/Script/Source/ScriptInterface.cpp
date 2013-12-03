#include "LUAInterface.h"

namespace RootEngine
{
	namespace LUA
	{
		SubsystemSharedContext g_context;
		LUAManager* LUAManager::s_LUAManager = nullptr;

		void LUAManager::Startup()
		{
			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Succesfull startup of the ");
		}

		void LUAManager::Shutdown()
		{
			delete s_LUAManager;
		}

		void LUAManager::Initialize()
		{
			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "LUA engine succesfully initialized");
		}

		LUAManager* LUAManager::GetInstance()
		{
			if(!s_LUAManager)
				s_LUAManager = new LUAManager();

			return s_LUAManager;
		}

	}
}

RootEngine::LUA::LUAInterface* CreateLUASystem(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::LUA::g_context = p_context;

	return RootEngine::LUA::LUAManager::GetInstance();
}
