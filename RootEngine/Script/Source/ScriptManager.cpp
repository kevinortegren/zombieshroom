#include <RootEngine/Script/Include/ScriptManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <iostream>

namespace RootEngine
{
	namespace Script
	{
		SubsystemSharedContext g_context;
		ScriptManager* ScriptManager::s_scriptManager = nullptr;

		void ScriptManager::Startup()
		{
			m_luaState = luaL_newstate();
			luaL_openlibs(m_luaState);
			m_parameterCount = 0;
			g_context.m_logger->LogText(LogTag::SCRIPT, LogLevel::INIT_PRINT, "Succesfull startup of the scripting system");
		}

		void ScriptManager::Shutdown()
		{
			lua_close(m_luaState);
			delete s_scriptManager;			
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

		bool ScriptManager::IsFunctionDefined(const std::string& p_scriptName, const std::string& p_functionName)
		{
			lua_getglobal(m_luaState, p_scriptName.c_str());
			lua_getfield(m_luaState, -1, p_functionName.c_str());
			bool result = lua_isfunction(m_luaState, -1);
			lua_pop(m_luaState, 2);

			return result;
		}

		void ScriptManager::SetFunction(const std::string& p_abilityName, const std::string& p_functionName)
		{
			// Execute specific function
			if(p_abilityName.compare("MatchState") == 0 && p_functionName.compare("OnDestroy") == 0)
				int i = 0;
			lua_getglobal(m_luaState, p_abilityName.c_str());
			lua_getfield(m_luaState, -1, p_functionName.c_str());
		}

		void ScriptManager::ExecuteScript()
		{
			int error = 0;
			if(error = lua_pcall(m_luaState, m_parameterCount, 0, 0) != 0)
			{
				printf("Error: %s\n", lua_tostring(m_luaState, -1));
			}

			m_parameterCount = 0;
		}

		void ScriptManager::AddParameterUserData(void* p_data, size_t p_size, const std::string& p_metaTable)
		{
			void** p = (void**) lua_newuserdata(m_luaState, p_size);
			*p = p_data;
			luaL_setmetatable(m_luaState, p_metaTable.c_str());
			m_parameterCount++;
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

		void ScriptManager::SetGlobalString(const std::string& p_variableName, const std::string& p_string, const std::string& p_globalName)
		{
			lua_getglobal(m_luaState, p_globalName.c_str());
			lua_pushstring(m_luaState, p_string.c_str());
			lua_setfield(m_luaState, -2, p_variableName.c_str());
			lua_pop(m_luaState, 1);
		}

		void ScriptManager::SetGlobalNumber(const std::string& p_variableName, double p_double, const std::string& p_globalName)
		{	
			lua_getglobal(m_luaState, p_globalName.c_str());
			lua_pushnumber(m_luaState, p_double);
			lua_setfield(m_luaState, -2, p_variableName.c_str());
			lua_pop(m_luaState, 1);
		}

		void ScriptManager::SetGlobalBoolean(const std::string& p_variableName, bool p_bool, const std::string& p_globalName)
		{
			lua_getglobal(m_luaState, p_globalName.c_str());
			lua_pushboolean(m_luaState, p_bool);
			lua_setfield(m_luaState, -2, p_variableName.c_str());
			lua_pop(m_luaState, 1);
		}


		std::string ScriptManager::GetGlobalString(const std::string& p_variableName, const std::string& p_globalName)
		{
			lua_getglobal(m_luaState, p_globalName.c_str());
			lua_getfield(m_luaState, -1, p_variableName.c_str());
			std::string variable(lua_tostring(m_luaState, -1));
			lua_pop(m_luaState, 2);

			return variable;
		}

		double ScriptManager::GetGlobalNumber(const std::string& p_variableName, const std::string& p_globalName)
		{
			lua_getglobal(m_luaState, p_globalName.c_str());
			lua_getfield(m_luaState, -1, p_variableName.c_str());
			double variable = (double) lua_tonumber(m_luaState, -1);
			lua_pop(m_luaState, 2);

			return variable;
		}

		bool ScriptManager::GetGlobalBoolean(const std::string& p_variableName, const std::string& p_globalName)
		{
			lua_getglobal(m_luaState, p_globalName.c_str());
			lua_getfield(m_luaState, -1, p_variableName.c_str());
			bool variable = lua_toboolean(m_luaState, -1) != 0;
			lua_pop(m_luaState, 2);

			return variable;
		}


		void ScriptManager::RegisterFunction(std::string p_funcName, lua_CFunction p_func)
		{
			lua_register(m_luaState, p_funcName.c_str(), p_func);
		}

		int ScriptManager::LoadScript( std::string p_scriptPath )
		{
			// Execute the script

			if (luaL_dofile(m_luaState, (m_workingDir + "Assets/Scripts/" + p_scriptPath).c_str()))
			{
				printf("%s\n", lua_tostring(m_luaState, -1));
				return 1;
			}
			return 0;
		}

		lua_State* ScriptManager::GetLuaState()
		{
			return m_luaState;
		}

	}
}

RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context)
{ 
	RootEngine::Script::g_context = p_context;

	return RootEngine::Script::ScriptManager::GetInstance();
}