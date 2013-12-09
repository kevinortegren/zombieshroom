#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>
#include <../../Utility/ECS/Include/World.h>

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

			virtual void ExecuteWholeScript(std::string p_scriptPath) = 0;
			virtual void SetScriptWithFunction(std::string p_scriptPath, std::string p_functionName) = 0;
			virtual void ExecuteScript() = 0;

			virtual void AddParameterString(std::string p_string) = 0;
			virtual void AddParameterNumber(double p_double) = 0;
			virtual void AddParameterBoolean(bool p_bool) = 0;
		};
	}
}

extern "C"
{
	typedef RootEngine::Script::ScriptInterface* (*CREATESCRIPTINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context);
}