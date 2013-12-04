#include "ScriptManager.h"
#include <Lua/lua.hpp>

#include <iostream>

namespace RootEngine
{
	namespace Script
	{
		SubsystemSharedContext g_context;
		ScriptManager* ScriptManager::s_scriptManager = nullptr;

		void ScriptManager::Startup()
		{
			g_context.m_logger->LogText(LogTag::GENERAL, LogLevel::INIT_PRINT, "Succesfull startup of the scripting system");
		}

		void ScriptManager::Shutdown()
		{
			delete s_scriptManager;
		}

		void ScriptManager::Initialize()
		{
			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "LUA engine succesfully initialized");
		}

		ScriptManager* ScriptManager::GetInstance()
		{
			if(!s_scriptManager)
				s_scriptManager = new ScriptManager();

			return s_scriptManager;
		}

		void ScriptManager::ExecuteScript()
		{
			lua_State* lua_state;
			lua_state = luaL_newstate();
			// load Lua libraries
			static const luaL_Reg lualibs[] =
			{
				{ "base", luaopen_base },
				{"io", luaopen_io},
				{ NULL, NULL }
			};

			const luaL_Reg* lib = lualibs;
			for(; lib->func != NULL; lib++)
			{
				luaL_requiref(lua_state, lib->name, lib->func, 1); // http://www.lua.org/manual/5.2/manual.html#luaL_requiref
				lua_settop(lua_state, 0);	// The lua_settop() sentence just ensures that we discard any variables that may be populated into the Lua stack.
				// http://www.lua.org/manual/5.2/manual.html#lua_settop
			}

			luaL_dofile(lua_state, "callLuaFunction.lua");


			int a, b = 0;
			std::string message;

			lua_getglobal(lua_state, "lel");
			lua_pushnumber(lua_state, 5);
			lua_pushnumber(lua_state, 10);

			lua_call(lua_state, 2, 1);

			a = (int)lua_tointeger(lua_state, -1);
			lua_pop(lua_state, 1);
			b = (int)lua_tointeger(lua_state, -1);
			//message = lua_tostring(lua_state, -1);
			lua_pop(lua_state, 1);

			using namespace std;

			cout << "[C++] " << a << endl;
			cout << "[C++] " << b << endl;
			//cout << "[C++] " << message << endl;

		}
	}
}

RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::Script::g_context = p_context;

	return RootEngine::Script::ScriptManager::GetInstance();
}
