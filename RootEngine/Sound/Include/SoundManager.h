#pragma once
#include <Fmod/fmod.hpp>
#include <Fmod/fmod_errors.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <string>

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
			virtual void PlaySound() = 0;
			virtual void Update() = 0;
			virtual void PlayBackgroundSound() = 0;
		};

		class SoundManager : public SoundInterface
		{
		public:
			SoundManager();
			~SoundManager();
			void Startup();
			void Shutdown();
			void SetWorkingDir(std::string p_path) { m_workingDir = p_path; }
			void PlaySound();
			void Update();
			void PlayBackgroundSound();

		private:
			void ErrorCheck(FMOD_RESULT p_result);

			std::string m_workingDir;
			FMOD::System*		m_system;
			FMOD_RESULT			m_result;
			FMOD_SPEAKERMODE	m_speakerMode;
			FMOD_CAPS			m_caps;
			FMOD::Sound*		m_backGroundStream;
			FMOD::Channel*		m_BackGroundChannel;

			unsigned int		m_version;
			int					m_numDrivers;
			char				m_name[256];
		};
	}
}

extern "C"
{
	typedef RootEngine::Sound::SoundInterface* (*CREATESOUNDINTERFACE) (RootEngine::SubsystemSharedContext);
	SCRIPT_DLL_EXPORT RootEngine::Sound::SoundInterface* CreateSoundInterface(RootEngine::SubsystemSharedContext p_context);
}