#include <RootEngine/Sound/Include/SoundAudio.h>
#include <iostream>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootEngine
{
	namespace Sound
	{
		extern SubsystemSharedContext g_context;

		SoundAudio::SoundAudio( FMOD::System* p_system )
		{
			m_system = p_system;
			m_sound = nullptr;
		}

		SoundAudio::~SoundAudio()
		{
			m_sound->release();
		}

		bool SoundAudio::LoadSound( std::string p_name )
		{
			FMOD_RESULT result;
			if(m_sound == nullptr)
			{
				result = m_system->createSound(p_name.c_str(), SOUND_SOFTWARE, 0, &m_sound);

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

		void SoundAudio::PlayOnce( float p_volume, unsigned p_flags, glm::vec3 p_position /*= glm::vec3(0.0f)*/, float p_minRange /*= 0.0f*/, float p_maxRange /*= 0.0f*/ )
		{
			FMOD::Channel* channel;
			m_system->playSound(FMOD_CHANNEL_FREE, m_sound, true, &channel);
			channel->setMode(p_flags);
			channel->setVolume(p_volume);
			FMOD_VECTOR position = {p_position.x, p_position.y, p_position.z};
			if((p_flags & SOUND_3D) == SOUND_3D)
			{
				channel->set3DAttributes(&position, 0);
				channel->set3DDopplerLevel(0.0f);
				channel->set3DMinMaxDistance(p_minRange, p_maxRange);
			}
			channel->setPaused(false);
		}
	}
}
