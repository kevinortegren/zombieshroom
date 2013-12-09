#pragma once

#include <Lua/lua.hpp>
#include <../../Utility/ECS/Include/World.h>
#include <RootEngine/Include/GameSharedContext.h>

namespace RootEngine
{
	namespace Script
	{
		ECS::World* g_world;
		SubsystemSharedContext g_context;

		// TODO lua to c++ LOGGER api (möjlighet att kunna logga grejor ifrån LUA)

		static int CreateThing(lua_State* p_luaState)
		{
			//g_world->GetEntityManager()->
			//g_world->GetEntityManager()->
			//g_context.m_logger->LogText(LogTag::SCRIPT, LogLevel::DEBUG_PRINT, "Trying to create");
			return 0;
		}

		static int LogTextLUA()
		{
		
			return 0;
		}


	}
}
