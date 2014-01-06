#pragma once
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <Lua/lua.hpp>

#if defined(_WINDLL)
#define SCRIPT_DLL_EXPORT __declspec(dllexport)
#else
#define SCRIPT_DLL_EXPORT __declspec(dllimport)
#endif

namespace RootEngine
{
	namespace Script
	{

		class ScriptInterface : public RootEngine::SubsystemInterface
		{
		public:
			virtual void SetWorkingDir(std::string p_path) = 0;


			/* This function runs a script from top to bottom and executes all code that is not contained in functions. */
			virtual void ExecuteWholeScript(std::string p_scriptPath) = 0;


			/* This sets a specific script that is to be used when doing a specific function call with added parameters. */
			virtual void SetFunction(std::string p_abilityName, std::string p_functionName) = 0;
			
			virtual int LoadScript(std::string p_scriptPath) = 0;

			/* This executes the script that has been set. */
			virtual void ExecuteScript() = 0;


			virtual void AddParameterString(std::string p_string) = 0;
			virtual void AddParameterNumber(double p_double) = 0;
			virtual void AddParameterBoolean(bool p_bool) = 0;
			virtual void RegisterFunction(std::string p_funcName, lua_CFunction p_func) = 0;

			virtual lua_State* GetLuaState() = 0;
		};
	}
}

extern "C"
{
	typedef RootEngine::Script::ScriptInterface* (*CREATESCRIPTINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context);
}