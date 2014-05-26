#include <RootEngine/Sound/Include/SoundChannel.h>
#include <iostream>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
namespace RootEngine
{
	namespace Sound
	{
		extern SubsystemSharedContext g_context;

		SoundChannel::SoundChannel( FMOD::System* p_system ): m_is3D(false)
		{
			m_system = p_system;
		}

		SoundChannel::~SoundChannel()
		{
			m_channel->stop();
		}

		void SoundChannel::PlaySound( Sound::SoundAudioInterface* p_audio, float p_volume, unsigned p_flags )
		{
			HandleError(m_system->playSound(FMOD_CHANNEL_FREE, p_audio->GetFmodSound(), true, &m_channel));
			HandleError(m_channel->setVolume(p_volume));
			HandleError(m_channel->setMode(p_flags));
			if((p_flags & SOUND_3D) == SOUND_3D)
			{
				HandleError(m_channel->set3DDopplerLevel(0.0f)); //No doppler level
				m_is3D = true;
			}
			
			HandleError(m_channel->setPaused(false)); //Set attributes before starting sound
		}

		void SoundChannel::SetVolume( float p_volume )
		{
			if(IsPlaying())
				 HandleError(m_channel->setVolume(p_volume));
		}

		void SoundChannel::SetPaused( bool p_paused )
		{
			if(IsPlaying())
				 HandleError(m_channel->setPaused(p_paused));
		}

		void SoundChannel::SetMinMaxDistance( float p_min, float p_max )
		{
			if(IsPlaying() && m_is3D)
				 HandleError(m_channel->set3DMinMaxDistance(p_min, p_max));
		}

		bool SoundChannel::IsPlaying()
		{
			bool playing;
			FMOD_RESULT result = m_channel->isPlaying(&playing);
			if(result != FMOD_OK)
			{
				return false;
			}
			return playing;
		}

		void SoundChannel::SetPosition( glm::vec3 p_position )
		{
			if(IsPlaying() && m_is3D)
			{
				FMOD_VECTOR position = {p_position.x, p_position.y, p_position.z};
				m_channel->set3DAttributes(&position, 0);
			}
				//FMOD uses a left handed co-ordinate system by default.
				//To use a right handed co-ordinate system specify FMOD_INIT_3D_RIGHTHANDED from FMOD_INITFLAGS in System::init
		}

		void SoundChannel::HandleError( FMOD_RESULT p_result )
		{
			if(p_result != FMOD_OK)
			{
				g_context.m_logger->LogText(LogTag::SOUND, LogLevel::FATAL_ERROR, "Fmod error: %d", (int)p_result);
			}
		}
	}
}
