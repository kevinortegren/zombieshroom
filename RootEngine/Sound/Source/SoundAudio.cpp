#include <RootEngine/Sound/Include/SoundAudio.h>
#include <iostream>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>


namespace RootEngine
{
	namespace Sound
	{
		extern SubsystemSharedContext g_context;

		SoundAudio::SoundAudio( FMOD::System* p_system ): m_is3D(false)
		{
			m_system = p_system;
			m_sound = nullptr;
		}

		SoundAudio::~SoundAudio()
		{
			m_sound->release();
		}

		bool SoundAudio::LoadSound( std::string p_name, unsigned p_flags )
		{
			FMOD_RESULT result;
			if(m_sound == nullptr)
			{
				if((p_flags & SOUND_3D) == SOUND_3D)
					m_is3D = true;
				else
					m_is3D = false;

				result = m_system->createSound(p_name.c_str(), p_flags, 0, &m_sound);

				if(result != FMOD_OK)
				{
					if(result == FMOD_ERR_FILE_NOTFOUND)
						g_context.m_logger->LogText(LogTag::SOUND, LogLevel::FATAL_ERROR, "Audio file not found: %s", p_name.c_str());
					else
						g_context.m_logger->LogText(LogTag::SOUND, LogLevel::WARNING, "FMOD Results: %d", (int)result);
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

		bool SoundAudio::Is3D()
		{
			return m_is3D;
		}

	}
}
