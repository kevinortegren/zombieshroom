#pragma once
#include <Fmod/fmod.hpp>
#include <Fmod/fmod_errors.h>
#include <string>
#include <RootEngine/Sound/Include/SoundAudio.h>
#include <RootEngine/Include/SubsystemSharedContext.h>
#include <glm/glm.hpp>

namespace RootEngine
{
	namespace Sound
	{
		class SoundChannelInterface
		{
		public:
			virtual void PlaySound(Sound::SoundAudio* p_audio, float p_volume) = 0;
			virtual void SetVolume(float p_volume) = 0;
			virtual void SetPaused(bool p_paused) = 0;
			virtual void SetMinMaxDistance(float p_min, float p_max) = 0;
			virtual void SetPosition(glm::vec3 p_position) = 0;
		};

		class SoundChannel : public SoundChannelInterface
		{
		public:
			SoundChannel(FMOD::System* p_system);
			~SoundChannel();
			void PlaySound(Sound::SoundAudio* p_audio, float p_volume);
			void SetVolume(float p_volume);
			void SetPaused(bool p_paused);
			void SetMinMaxDistance(float p_min, float p_max);
			void SetPosition(glm::vec3 p_position);
		
		private:
			bool IsPlaying();
			FMOD::System*		m_system;
			FMOD::Channel*		m_channel;
		};
	}
}
