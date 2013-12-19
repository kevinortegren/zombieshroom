#pragma once
#include "RootScript.h"
#include <Lua/lua.hpp>



namespace RootEngine
{
	namespace Script
	{
		class ScriptManager : public ScriptInterface
		{
		public:
			void Startup();
			void Shutdown();
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }

			void ExecuteWholeScript(std::string p_scriptPath);
			void SetFunction(std::string p_abilityName, std::string p_functionName);
			void ExecuteScript();
			int LoadScript(std::string p_scriptPath);
			void AddParameterString(std::string p_string);
			void AddParameterNumber(double p_double);
			void AddParameterBoolean(bool p_bool);
			void RegisterFunction(std::string p_funcName, lua_CFunction p_func);
			lua_State* GetLuaState();
			static ScriptManager* GetInstance();

		private:
			
			

			std::string m_workingDir;
			lua_State* m_luaState;
			int m_parameterCount;
			static ScriptManager* s_scriptManager;
		};
	}
}
