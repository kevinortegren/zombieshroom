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
		namespace LuaLibraryInit
		{
			enum LuaLibraryInit
			{
				INIT_BASE = 1,
				INIT_IO = 2,
				INIT_OS = 4,
				INIT_STRING = 8,
				INIT_MATH = 16,
				INIT_DEBUG = 32,
				INIT_BIT32 = 64,
				INIT_COROUTINE = 128, 
				INIT_ALL = INIT_BASE | INIT_BASE | INIT_IO | INIT_OS | INIT_STRING | INIT_MATH | INIT_DEBUG | INIT_BIT32 | INIT_COROUTINE
			};
		}

		class ScriptInterface : public RootEngine::SubsystemInterface
		{
		public:
			virtual void SetWorkingDir(std::string p_path) = 0;

			virtual void ExecuteWholeScript(std::string p_scriptPath, int p_flags) = 0;
			virtual void ExecuteScriptWithFunction(std::string p_scriptPath, int p_flags, std::string p_functionName) = 0;

		};
	}
}

extern "C"
{
	typedef RootEngine::Script::ScriptInterface* (*CREATESCRIPTINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context);
}