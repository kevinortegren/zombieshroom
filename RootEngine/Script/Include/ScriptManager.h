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

			virtual void ExecuteWholeScript(std::string p_scriptPath, LuaLibraryInit::LuaLibraryInit p_luaLib) = 0;
			virtual void ExecuteScript(std::string p_scriptPath, LuaLibraryInit::LuaLibraryInit p_luaLib) = 0;

		};

		class ScriptManager : public ScriptInterface
		{
		public:
			void Startup();
			void Shutdown();
			void Initialize();
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }

			void ExecuteWholeScript(std::string p_scriptPath, LuaLibraryInit::LuaLibraryInit p_luaLib);
			void ExecuteScript(std::string p_scriptPath, LuaLibraryInit::LuaLibraryInit p_luaLib);

			static ScriptManager* GetInstance();

		private:
			static ScriptManager* s_scriptManager;

			std::string m_workingDir;

			void SetupLibrariesForLua(LuaLibraryInit::LuaLibraryInit p_luaLib);

			void LuaLibBase(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			void LuaLibIO(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			void LuaLibOS(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			void LuaLibString(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			void LuaLibMath(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			void LuaLibDebug(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			void LuaLibBit32(LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			void LuaLibCoroutine (LuaLibraryInit::LuaLibraryInit p_luaLib, const luaL_Reg* p_luaLibrary);
			// I'm here
			/*
				I must move manager away from interface.

				Next step is to make all the loadLibs work out as intended.
				Move all of them into the same function.
				Or include lua in .h file. Which I dont wanna do. (This can be solved by putting another .h file up that specifies the private functions and thus the include aswell.
				Tomorrows work

			*/
		};
	}
}

extern "C"
{
	typedef RootEngine::Script::ScriptInterface* (*CREATESCRIPTINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Script::ScriptInterface* CreateScriptInterface(RootEngine::SubsystemSharedContext p_context);
}