#include "ScriptManager.h"
#include <Lua/lua.hpp>

#include <iostream>

namespace RootEngine
{
	namespace Script
	{
		static const luaL_Reg g_luaLibrary[] =
		{
			{ "base", luaopen_base },		
			{ "io", luaopen_io },
			{ "os", luaopen_os },
			{ "string", luaopen_string },
			{ "math", luaopen_math },
			{ "debug", luaopen_debug },
			{ "base", luaopen_package },
			{ "bit32", luaopen_bit32 },
			{ "coroutine", luaopen_coroutine },
			{ NULL, NULL}
		};

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
			g_context.m_logger->LogText(LogTag::SCRIPT, LogLevel::DEBUG_PRINT, "LUA engine succesfully initialized");
		}

		ScriptManager* ScriptManager::GetInstance()
		{
			if(!s_scriptManager)
				s_scriptManager = new ScriptManager();

			return s_scriptManager;
		}

		void ScriptManager::ExecuteWholeScript(std::string p_scriptPath, LuaLibraryInit::LuaLibraryInit p_flags)
		{
			lua_State* lua_state;
			lua_state = luaL_newstate();

			// load Lua libraries
			const luaL_Reg* l_luaLibrary = g_luaLibrary;
			



		// 	const LuaLibraryInit::LuaLibraryInit* test = &LuaLibraryInit::INIT_BASE;
			
				/*INIT_BASE = 1,
				INIT_IO = 2,
				INIT_OS = 4,
				INIT_STRING = 8,
				INIT_MATH = 16,
				INIT_DEBUG = 32,
				INIT_BIT32 = 64,
				INIT_COROUTINE = 128, 

				if((p_flags & SubsystemInit::INIT_RENDER) == SubsystemInit::INIT_RENDER)*/

			/*
			const luaL_Reg* lib = lualibs;
			for(; lib->func != NULL; lib++)
			{
				 // http://www.lua.org/manual/5.2/manual.html#luaL_requiref
				lua_settop(lua_state, 0);	// The lua_settop() sentence just ensures that we discard any variables that may be populated into the Lua stack.
				// http://www.lua.org/manual/5.2/manual.html#lua_settop
			}

			luaL_dofile(lua_state, (m_workingDir + "Assets/Scripts/" + p_scriptPath).c_str());


			int a, b = 0;
			std::string message;

			lua_getglobal(lua_state, "lel");
			//lua_pushnumber(lua_state, 5);
			//lua_pushnumber(lua_state, 10);

			lua_call(lua_state, 0, 0);

			/*a = (int)lua_tointeger(lua_state, -1);
			lua_pop(lua_state, 1);
			b = (int)lua_tointeger(lua_state, -1);
			//message = lua_tostring(lua_state, -1);
			lua_pop(lua_state, 1);

			using namespace std;

			cout << "[C++] " << a << endl;
			cout << "[C++] " << b << endl;*/
			//cout << "[C++] " << message << endl;*/

		}
		void ScriptManager::ExecuteScript(std::string p_scriptPath, LuaLibraryInit::LuaLibraryInit p_luaLib)
		{

		}

		void ScriptManager::SetupLibrariesForLua(LuaLibraryInit::LuaLibraryInit p_luaLib)
		{
			const luaL_Reg* l_luaLibrary = g_luaLibrary;
			
		}

		void ScriptManager::LuaLibBase(LuaLibraryInit::LuaLibraryInit p_luaLib, int p_flags const luaL_Reg* p_luaLibrary)
		{
			if( (p_flags & LuaLibraryInit::INIT_BASE) == LuaLibraryInit::INIT_BASE)
			{
				luaL_requiref(lua_state, p_luaLibrary->name, l_luaLibrary->func, 1);
			}
		}
		
		void ScriptManager::LuaLibIO(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary)
		{

		}

		void ScriptManager::LuaLibOS(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary)
		{
			
		}

		void ScriptManager::LuaLibString(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary)
		{
			
		}

		void ScriptManager::LuaLibMath(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary)
		{

		}
			
		void ScriptManager::LuaLibDebug(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary)
		{
			
		}
		
		void ScriptManager::LuaLibBit32(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary)
		{
				
		}
		
		void ScriptManager::LuaLibCoroutine (LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary)
		{
				
		}
	}
}

RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::Script::g_context = p_context;

	return RootEngine::Script::ScriptManager::GetInstance();
}
