#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>

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
			virtual void ExecuteScript() = 0;

		};

		class ScriptManager : public ScriptInterface
		{
		public:
			void Startup();
			void Shutdown();
			void Initialize();

			void ExecuteScript();

			static ScriptManager* GetInstance();

		private:
			static ScriptManager* s_scriptManager;
		};
	}
}

extern "C"
{
	typedef RootEngine::Script::ScriptInterface* (*CREATESCRIPTINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context);
}