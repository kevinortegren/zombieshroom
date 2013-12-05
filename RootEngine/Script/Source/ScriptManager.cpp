#include "ScriptManager.h"
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

		void ScriptManager::ExecuteWholeScript(std::string p_scriptPath, int p_flags)
		{
			// Create LUA state
			lua_State* l_luaState;
			l_luaState = luaL_newstate();

			// Setup needed libraries
			SetupLibrariesForLua(l_luaState, p_flags);

			// Execute the script
			luaL_dofile(l_luaState, (m_workingDir + "Assets/Scripts/" + p_scriptPath).c_str());

			// Close LUA
			lua_close(l_luaState);
		}

		void ScriptManager::ExecuteScriptWithFunction(std::string p_scriptPath, int p_flags, std::string p_functionName)
		{
			// Create LUA state
			lua_State* l_luaState;
			l_luaState = luaL_newstate();

			// Setup needed libraries
			SetupLibrariesForLua(l_luaState, p_flags);

			// Execute the script
			luaL_dofile(l_luaState, (m_workingDir + "Assets/Scripts/" + p_scriptPath).c_str());

			// Execute specific function
			lua_getglobal(l_luaState, p_functionName.c_str());			
			lua_call(l_luaState, 0, 0);

			lua_close(l_luaState);
		}

		void ScriptManager::SetupLibrariesForLua(lua_State* p_luaState, int p_flags)
		{
			const luaL_Reg* l_luaLibrary = g_luaLibrary;

			if((p_flags & LuaLibraryInit::INIT_BASE) == LuaLibraryInit::INIT_BASE)
				LuaLoadLib(p_luaState, l_luaLibrary);
			l_luaLibrary++;

			if((p_flags & LuaLibraryInit::INIT_OS) == LuaLibraryInit::INIT_OS)
					LuaLoadLib(p_luaState, l_luaLibrary);
			l_luaLibrary++;

			if((p_flags & LuaLibraryInit::INIT_STRING) == LuaLibraryInit::INIT_STRING)
				LuaLoadLib(p_luaState, l_luaLibrary);
			l_luaLibrary++;

			if((p_flags & LuaLibraryInit::INIT_MATH) == LuaLibraryInit::INIT_MATH)
				LuaLoadLib(p_luaState, l_luaLibrary);
			l_luaLibrary++;

			if((p_flags & LuaLibraryInit::INIT_DEBUG) == LuaLibraryInit::INIT_DEBUG)
				LuaLoadLib(p_luaState, l_luaLibrary);
			l_luaLibrary++;

			if((p_flags & LuaLibraryInit::INIT_BIT32) == LuaLibraryInit::INIT_BIT32)
				LuaLoadLib(p_luaState, l_luaLibrary);
			l_luaLibrary++;

			if((p_flags & LuaLibraryInit::INIT_COROUTINE) == LuaLibraryInit::INIT_COROUTINE)
				LuaLoadLib(p_luaState, l_luaLibrary);
			l_luaLibrary++;			
		}

		void ScriptManager::LuaLoadLib(lua_State* p_luaState, const luaL_Reg* p_luaLibrary)
		{
			p_luaLibrary->func(p_luaState);
			//luaL_requiref(p_luaState, p_luaLibrary->name, p_luaLibrary->func, 1);,
			lua_settop(p_luaState, 0);
		}
	}
}

RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::Script::g_context = p_context;

	return RootEngine::Script::ScriptManager::GetInstance();
}