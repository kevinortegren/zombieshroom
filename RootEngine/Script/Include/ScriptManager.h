#pragma once

#include "RootScript.h"
#include <Lua/lua.hpp>
#include <../../Utility/ECS/Include/World.h>

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
			void SetScriptWithFunction(std::string p_scriptPath, std::string p_functionName);
			void ExecuteScript();
			
			void AddParameterString(std::string p_string);
			void AddParameterNumber(double p_double);
			void AddParameterBoolean(bool p_bool);

			static ScriptManager* GetInstance();

		private:
			static ScriptManager* s_scriptManager;

			std::string m_workingDir;
			lua_State* m_luaState;

			int m_parameterCount;
			
		};
	}
}
