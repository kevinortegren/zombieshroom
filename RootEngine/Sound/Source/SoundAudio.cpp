#include <RootEngine/Sound/Include/SoundAudio.h>
#include <iostream>


namespace RootEngine
{
	namespace Sound
	{
		extern SubsystemSharedContext g_context;

		SoundAudio::SoundAudio( FMOD::System* p_system )
		{
			m_system = p_system;
		}

		SoundAudio::~SoundAudio()
		{
			m_sound->release();
		}

		bool SoundAudio::LoadSound( std::string p_name, SoundFlags::SoundFlags p_flags )
		{
			if(m_sound == nullptr)
			{
				m_system->createSound(p_name.c_str(), p_flags, 0, &m_sound );
				if (m_sound == nullptr)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			return false;
		}

		FMOD::Sound* SoundAudio::GetFmodSound()
		{
			return m_sound;
		}
	}
}
