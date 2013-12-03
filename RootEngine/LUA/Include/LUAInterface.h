#pragma once

#include <RootEngine/Include/SubsystemSharedContext.h>

#if defined(_WINDLL)
#define SOUND_DLL_EXPORT __declspec(dllexport)
#else
#define SOUND_DLL_EXPORT __declspec(dllimport)
#endif

namespace RootEngine
{
	namespace LUA
	{
		class LUAInterface : public RootEngine::SubsystemInterface
		{
		public:
			virtual void DoScript() = 0;

		};


		class LUAManager : public LUAInterface
		{
		public:
			void Startup();
			void Shutdown();
			void Initialize();

			void DoScript()	{ }

			static LUAManager* GetInstance();

		private:
			static LUAManager* s_LUAManager;
		};
	}

}

extern "C"
{
	typedef RootEngine::LUA::LUAInterface* (*GETSOUNDINTERFACE) (RootEngine::SubsystemSharedContext);
	SOUND_DLL_EXPORT RootEngine::LUA::LUAInterface* CreateSoundSystem(RootEngine::SubsystemSharedContext p_context);
}