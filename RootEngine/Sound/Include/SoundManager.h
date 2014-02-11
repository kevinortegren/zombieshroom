#pragma once
#include <Fmod/fmod.h>
#include <RootEngine/Include/SubsystemSharedContext.h>

#if defined(_WINDLL)
#define SCRIPT_DLL_EXPORT __declspec(dllexport)
#else
#define SCRIPT_DLL_EXPORT __declspec(dllimport)
#endif

namespace RootEngine
{
	namespace Sound
	{
		class SoundInterface : public RootEngine::SubsystemInterface
		{
		public:
			virtual void SetWorkingDir(std::string p_path) = 0;
		};

		class SoundManager : public SoundInterface
		{
		public:
			SoundManager();
			~SoundManager();
			void Startup();
			void Shutdown();
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }

		private:
			std::string m_workingDir;
		};
	}
}

extern "C"
{
	typedef RootEngine::Sound::SoundInterface* (*CREATESOUNDINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Sound::SoundInterface* CreateSoundInterface(RootEngine::SubsystemSharedContext p_context);
}