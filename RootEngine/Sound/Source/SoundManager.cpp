#include <RootEngine/Sound/Include/SoundManager.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <iostream>

namespace RootEngine
{
	namespace Sound
	{
		SubsystemSharedContext g_context;

		SoundManager::SoundManager()
		{

		}

		SoundManager::~SoundManager()
		{

		}
		void SoundManager::Startup()
		{
		
		}

		void SoundManager::Shutdown()
		{
					
		}
	}
}

RootEngine::Sound::SoundInterface* CreateSoundInterface(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::Sound::g_context = p_context;

	return new RootEngine::Sound::SoundManager;
}