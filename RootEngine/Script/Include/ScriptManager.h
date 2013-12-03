#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>
//#include 

#if defined(_WINDLL)
#define LUA_DLL_EXPORT __declspec(dllexport)
#else
#define LUA_DLL_EXPORT __declspec(dllimport)
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
	typedef RootEngine::Script::ScriptInterface* (*GETSCRIPTINTERFACE) (RootEngine::SubsystemSharedContext);
	LUA_DLL_EXPORT RootEngine::Script::ScriptInterface* CreateScriptSystem(RootEngine::SubsystemSharedContext p_context);
}