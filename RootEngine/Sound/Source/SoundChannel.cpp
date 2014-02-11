#include <RootEngine/Sound/Include/SoundChannel.h>
#include <iostream>

namespace RootEngine
{
	namespace Sound
	{
		extern SubsystemSharedContext g_context;

		SoundChannel::SoundChannel( FMOD::System* p_system )
		{
			m_system = p_system;
		}

		SoundChannel::~SoundChannel()
		{
			
		}

		void SoundChannel::PlaySound( Sound::SoundAudio* p_audio, float p_volume )
		{
			m_system->playSound(FMOD_CHANNEL_FREE, p_audio->GetFmodSound(), true, &m_channel);
			m_channel->setVolume(p_volume);
			m_channel->set3DDopplerLevel(0.0f); //No doppler level
			m_channel->setPaused(false); //Set attributes before starting sound
		}

		void SoundChannel::SetVolume( float p_volume )
		{
			if(IsPlaying())
				m_channel->setVolume(p_volume);
		}

		void SoundChannel::SetPaused( bool p_paused )
		{
			if(IsPlaying())
				m_channel->setPaused(p_paused);
		}

		void SoundChannel::SetMinMaxDistance( float p_min, float p_max )
		{
			if(IsPlaying())
				m_channel->set3DMinMaxDistance(p_min, p_max);
		}

		bool SoundChannel::IsPlaying()
		{
			bool playing;
			FMOD_RESULT result = m_channel->isPlaying(&playing);
			if(result != FMOD_OK)
			{
				g_context.m_logger->LogText(LogTag::SOUND, LogLevel::WARNING, "Channel IsPlaying is not FMOD_OK");
				return false;
			}
			return playing;
		}

		void SoundChannel::SetPosition( glm::vec3 p_position )
		{
			if(IsPlaying())
			{
				FMOD_VECTOR position = {p_position.x, p_position.y, p_position.z};
				m_channel->set3DAttributes(&position, 0);
			}
				//FMOD uses a left handed co-ordinate system by default.
				//To use a right handed co-ordinate system specify FMOD_INIT_3D_RIGHTHANDED from FMOD_INITFLAGS in System::init
		}

	}
}
