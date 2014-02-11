#pragma once
#include <Fmod/fmod.hpp>
#include <Fmod/fmod_errors.h>
#include <string>
#include <RootEngine/Include/SubsystemSharedContext.h>

namespace SoundFlags
{
	enum SoundFlags
	{
		SOUND_DEFAULT,                   
		SOUND_LOOP_OFF,                  
		SOUND_LOOP_NORMAL,               
		SOUND_LOOP_BIDI,                 
		SOUND_2D,                        
		SOUND_3D,                        
		SOUND_HARDWARE,                  
		SOUND_SOFTWARE,                  
		SOUND_CREATESTREAM,              
		SOUND_CREATESAMPLE,              
		SOUND_CREATECOMPRESSEDSAMPLE,    
		SOUND_OPENUSER,                  
		SOUND_OPENMEMORY,                
		SOUND_OPENMEMORY_POINT,          
		SOUND_OPENRAW,                   
		SOUND_OPENONLY,                  
		SOUND_ACCURATETIME,              
		SOUND_MPEGSEARCH,                
		SOUND_NONBLOCKING,               
		SOUND_UNIQUE,                    
		SOUND_3D_HEADRELATIVE,           
		SOUND_3D_WORLDRELATIVE,          
		SOUND_3D_INVERSEROLLOFF,         
		SOUND_3D_LINEARROLLOFF,          
		SOUND_3D_LINEARSQUAREROLLOFF,    
		SOUND_3D_CUSTOMROLLOFF,          
		SOUND_3D_IGNOREGEOMETRY,         
		SOUND_UNICODE,                   
		SOUND_IGNORETAGS,                
		SOUND_LOWMEM,                    
		SOUND_LOADSECONDARYRAM,          
		SOUND_VIRTUAL_PLAYFROMSTART,     
	};
}

namespace RootEngine
{
	namespace Sound
	{
		class SoundAudioInterface
		{
		public:
			virtual bool LoadSound(std::string p_name, SoundFlags::SoundFlags p_flags) = 0;
			virtual FMOD::Sound* GetFmodSound() = 0;
		};

		class SoundAudio : public SoundAudioInterface
		{
		public:
			SoundAudio(FMOD::System* p_system);
			~SoundAudio();
			bool LoadSound(std::string p_name, SoundFlags::SoundFlags p_flags);
			FMOD::Sound* GetFmodSound();
		
		private:
			FMOD::System*		m_system;
			FMOD::Sound*		m_sound;
		};
	}
}
