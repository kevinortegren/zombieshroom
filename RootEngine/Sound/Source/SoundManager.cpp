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
			g_context.m_logger->LogText(LogTag::SOUND, LogLevel::INIT_PRINT, "SoundManager initialized!");
		}

		SoundManager::~SoundManager()
		{
			m_backGroundStream->release();
			m_system->release();
		}
		void SoundManager::Startup()
		{
			m_BackGroundChannel = nullptr;

			m_result = FMOD::System_Create(&m_system);
			ErrorCheck(m_result);

			m_result = m_system->getVersion(&m_version);
			ErrorCheck(m_result);

			if(m_version < FMOD_VERSION)
				printf("Error you are using a old version of FMOD %08x. This program requires %08x\n", m_version, FMOD_VERSION);
			m_result = m_system->getNumDrivers(&m_numDrivers);
			ErrorCheck(m_result);

			if( m_numDrivers == 0)
			{
				m_result = m_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
				ErrorCheck(m_result);
			}
			else
			{
				m_result = m_system->getDriverCaps(0,&m_caps,0,&m_speakerMode);
				ErrorCheck(m_result);
			}

			m_result =  m_system->setSpeakerMode(m_speakerMode);
			ErrorCheck(m_result);
			m_result = m_system->setDSPBufferSize(1024,2);
			if(m_caps & FMOD_CAPS_HARDWARE_EMULATED)
			{
				/*
				The user has the 'Acceleration' slider set to off! This is really bad
				for latency! You might want to warn the user about this.
				*/
				m_result = m_system->setDSPBufferSize(1024,10);
				ErrorCheck(m_result);
			}
			m_result = m_system->getDriverInfo(0, m_name, 256, 0);
			ErrorCheck(m_result);

			if( strstr(m_name, "SigmaTel"))
			{
				/*
				"Sigmatel" sound devices crackle for some reason if the format is PCM 16bit.
				PCM floating point output seems to solve it.
				*/
				m_result = m_system->setSoftwareFormat(4800, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
				ErrorCheck(m_result);
			}
			m_result = m_system->init(1024, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
			if(m_result == FMOD_ERR_OUTPUT_CREATEBUFFER)
			{
				/*
				Ok, the speaker mode selected isn't supported by this soundcard. Switch it
				back to stereo...
				*/
				m_result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
				ErrorCheck(m_result);

				/*
				... and re-init.
				*/
				m_result = m_system->init(1024, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
			}
			ErrorCheck(m_result);

			FMOD_RESULT result;
			unsigned int blocksize;
			int numblocks;
			float ms;
			int frequency;
			result = m_system->getDSPBufferSize(&blocksize, &numblocks);
			result = m_system->getSoftwareFormat(&frequency, 0, 0, 0, 0, 0);
			ms = (float)blocksize * 1000.0f / (float)frequency;
			g_context.m_logger->LogText(LogTag::SOUND, LogLevel::DEBUG_PRINT, "Mixer blocksize        = %.02f ms", ms);
			g_context.m_logger->LogText(LogTag::SOUND, LogLevel::DEBUG_PRINT, "Mixer Total buffersize = %.02f ms", ms * numblocks);
			g_context.m_logger->LogText(LogTag::SOUND, LogLevel::DEBUG_PRINT, "Mixer Average Latency  = %.02f ms", ms * ((float)numblocks - 1.5f));
		}

		void SoundManager::Shutdown()
		{
					
		}

		void SoundManager::ErrorCheck( FMOD_RESULT p_result )
		{
			if(p_result !=  FMOD_OK)
			{
				perror("Error initializing SoundHandler" + p_result);
				g_context.m_logger->LogText(LogTag::SOUND, LogLevel::FATAL_ERROR, "Error initializing SoundManager");
			}
		}

		void SoundManager::Update()
		{
			m_system->update();
		}

		void SoundManager::PlayBackgroundSound(std::string p_name)
		{
			std::string soundFile = m_workingDir + "Assets\\Audio\\" + p_name;
			if(m_BackGroundChannel != nullptr)
				m_BackGroundChannel->stop();
			const char* l_temp = p_name.c_str();
			m_system->createStream(l_temp,FMOD_DEFAULT, 0, &m_backGroundStream);
			m_system->playSound(FMOD_CHANNEL_FREE, m_backGroundStream, false, &m_BackGroundChannel);
			m_BackGroundChannel->setMode(FMOD_LOOP_NORMAL);
			m_BackGroundChannel->setLoopCount(-1);
			m_BackGroundChannel->setVolume(0.1f);
			if(m_BackGroundChannel == nullptr)	
				g_context.m_logger->LogText(LogTag::SOUND, LogLevel::NON_FATAL_ERROR, "Background music not started!");
			else
				g_context.m_logger->LogText(LogTag::SOUND, LogLevel::DEBUG_PRINT, "Started background music!");
		}

		void SoundManager::SetListenerAttributes( glm::vec3 p_position, glm::vec3 p_up, glm::vec3 p_forward )
		{
			FMOD_VECTOR position	= {p_position.x, p_position.y, p_position.z};
			FMOD_VECTOR up			= {p_up.x, p_up.y, p_up.z};
			FMOD_VECTOR forward		= {p_forward.x, p_forward.y, p_forward.z};
			m_system->set3DListenerAttributes(0, &position, 0, &forward, &up );
		}

		void SoundManager::SetMasterVolume( float p_masterVolume )
		{
			FMOD::ChannelGroup* chgrp = nullptr;
			m_system->getMasterChannelGroup(&chgrp);
			chgrp->setVolume(p_masterVolume);
		}

		SoundAudioInterface* SoundManager::CreateSoundAudio()
		{
			return new SoundAudio(m_system); //Let resourcemanager delete this
		}

		SoundChannelInterface* SoundManager::CreateSoundChannel()
		{
			return new SoundChannel(m_system); //Let component delete this
		}

	}
}

RootEngine::Sound::SoundInterface* CreateSoundInterface(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::Sound::g_context = p_context;

	return new RootEngine::Sound::SoundManager;
}