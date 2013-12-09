#include <RootEngine/Script/Include/ScriptManager.h>
#include <RootEngine/Script/Include/ScriptAPI.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <iostream>

namespace RootEngine
{
	namespace Script
	{
		ScriptManager* ScriptManager::s_scriptManager = nullptr;

		void ScriptManager::Startup()
		{
			m_luaState = luaL_newstate();
			luaL_openlibs(m_luaState);
			RegisterFunctions();
			m_parameterCount = 0;
			g_context.m_logger->LogText(LogTag::SCRIPT, LogLevel::INIT_PRINT, "Succesfull startup of the scripting system");
		}

		void ScriptManager::Shutdown()
		{
			lua_close(m_luaState);
			//delete s_scriptManager;			
		}

		void ScriptManager::Initialize( ECS::World* p_world )
		{
			g_world = p_world;
		}

		
		ScriptManager* ScriptManager::GetInstance()
		{
			if(!s_scriptManager)
				s_scriptManager = new ScriptManager();

			return s_scriptManager;
		}

		void ScriptManager::ExecuteWholeScript(std::string p_scriptPath)
		{
			// Execute the script
			luaL_dofile(m_luaState, (m_workingDir + "Assets/Scripts/" + p_scriptPath).c_str());
		}

		void ScriptManager::SetFunction(std::string p_functionName)
		{
			// Execute specific function
			lua_getglobal(m_luaState, p_functionName.c_str());
		}

		void ScriptManager::ExecuteScript()
		{
			lua_call(m_luaState, m_parameterCount, 0);
			m_parameterCount = 0;
		}

		void ScriptManager::AddParameterString(std::string p_string)
		{
			lua_pushlstring(m_luaState, p_string.c_str(), p_string.length());
			m_parameterCount++;
		}

		void ScriptManager::AddParameterNumber(double p_double)
		{
			lua_pushinteger(m_luaState, (int)p_double);
			m_parameterCount++;
		}

		void ScriptManager::AddParameterBoolean(bool p_bool)
		{
			lua_pushboolean(m_luaState, p_bool);
			m_parameterCount++;
		}

		void ScriptManager::RegisterFunctions()
		{
			lua_register(m_luaState, "CreateThing", CreateThing);
		}

		void ScriptManager::LoadScript( std::string p_scriptPath )
		{
			// Execute the script
			luaL_dofile(m_luaState, (m_workingDir + "Assets/Scripts/" + p_scriptPath).c_str());
		}

	}
}

RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::Script::g_context = p_context;

	return RootEngine::Script::ScriptManager::GetInstance();
}