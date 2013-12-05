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
			void Initialize();
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }

			void ExecuteWholeScript(std::string p_scriptPath, int p_flags);
			void ExecuteScriptWithFunction(std::string p_scriptPath, int p_flags);

			static ScriptManager* GetInstance();

		private:
			static ScriptManager* s_scriptManager;

			std::string m_workingDir;

			void SetupLibrariesForLua(lua_State* p_luaState, int p_flags);
			void LuaLoadLib(lua_State* p_luaState, const luaL_Reg* p_luaLibrary);
			
		};
	}
}

/*
extern "C"
{
	typedef RootEngine::Script::ScriptInterface* (*CREATESCRIPTINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context);
}
*/