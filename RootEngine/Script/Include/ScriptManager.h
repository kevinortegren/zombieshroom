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
			void Initialize(ECS::World* p_world);
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }

			void ExecuteWholeScript(std::string p_scriptPath);
			void SetFunction(std::string p_functionName);
			void ExecuteScript();
			void LoadScript(std::string p_scriptPath);
			void AddParameterString(std::string p_string);
			void AddParameterNumber(double p_double);
			void AddParameterBoolean(bool p_bool);

			static ScriptManager* GetInstance();

		private:
			void RegisterFunctions();
			

			std::string m_workingDir;
			lua_State* m_luaState;
			int m_parameterCount;
			static ScriptManager* s_scriptManager;
		};
	}
}
