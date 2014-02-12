#pragma once
#include <Fmod/fmod.hpp>
#include <Fmod/fmod_errors.h>
#include <string>
#include <RootEngine/Include/SubsystemSharedContext.h>

typedef unsigned int SOUND_FLAGS;

#define SOUND_DEFAULT                   0x00000000  /* Default for all modes listed below. FMOD_LOOP_OFF, FMOD_2D, FMOD_HARDWARE */
#define SOUND_LOOP_OFF                  0x00000001  /* For non looping sounds. (DEFAULT).  Overrides FMOD_LOOP_NORMAL / FMOD_LOOP_BIDI. */
#define SOUND_LOOP_NORMAL               0x00000002  /* For forward looping sounds. */
#define SOUND_LOOP_BIDI                 0x00000004  /* For bidirectional looping sounds. (only works on software mixed static sounds). */
#define SOUND_2D                        0x00000008  /* Ignores any 3d processing. (DEFAULT). */
#define SOUND_3D                        0x00000010  /* Makes the sound positionable in 3D.  Overrides FMOD_2D. */
#define SOUND_HARDWARE                  0x00000020  /* Attempts to make sounds use hardware acceleration. (DEFAULT).  Note on platforms that don't support FMOD_HARDWARE (only 3DS, PS Vita, PSP, Wii and Wii U support FMOD_HARDWARE), this will be internally treated as FMOD_SOFTWARE. */
#define SOUND_SOFTWARE                  0x00000040  /* Makes the sound be mixed by the FMOD CPU based software mixer.  Overrides FMOD_HARDWARE.  Use this for FFT, DSP, compressed sample support, 2D multi-speaker support and other software related features. */
#define SOUND_CREATESTREAM              0x00000080  /* Decompress at runtime, streaming from the source provided (ie from disk).  Overrides FMOD_CREATESAMPLE and FMOD_CREATECOMPRESSEDSAMPLE.  Note a stream can only be played once at a time due to a stream only having 1 stream buffer and file handle.  Open multiple streams to have them play concurrently. */
#define SOUND_CREATESAMPLE              0x00000100  /* Decompress at loadtime, decompressing or decoding whole file into memory as the target sample format (ie PCM).  Fastest for FMOD_SOFTWARE based playback and most flexible.  */
#define SOUND_CREATECOMPRESSEDSAMPLE    0x00000200  /* Load MP2/MP3/IMAADPCM/CELT/Vorbis/AT9 or XMA into memory and leave it compressed.  CELT/Vorbis/AT9 encoding only supported in the FSB file format.  During playback the FMOD software mixer will decode it in realtime as a 'compressed sample'.  Can only be used in combination with FMOD_SOFTWARE.  Overrides FMOD_CREATESAMPLE.  If the sound data is not one of the supported formats, it will behave as if it was created with FMOD_CREATESAMPLE and decode the sound into PCM. */
#define SOUND_OPENUSER                  0x00000400  /* Opens a user created static sample or stream. Use FMOD_CREATESOUNDEXINFO to specify format and/or read callbacks.  If a user created 'sample' is created with no read callback, the sample will be empty.  Use Sound::lock and Sound::unlock to place sound data into the sound if this is the case. */
#define SOUND_OPENMEMORY                0x00000800  /* "name_or_data" will be interpreted as a pointer to memory instead of filename for creating sounds.  Use FMOD_CREATESOUNDEXINFO to specify length.  If used with FMOD_CREATESAMPLE or FMOD_CREATECOMPRESSEDSAMPLE, FMOD duplicates the memory into its own buffers.  Your own buffer can be freed after open.  If used with FMOD_CREATESTREAM, FMOD will stream out of the buffer whose pointer you passed in.  In this case, your own buffer should not be freed until you have finished with and released the stream.*/
#define SOUND_OPENMEMORY_POINT          0x10000000  /* "name_or_data" will be interpreted as a pointer to memory instead of filename for creating sounds.  Use FMOD_CREATESOUNDEXINFO to specify length.  This differs to FMOD_OPENMEMORY in that it uses the memory as is, without duplicating the memory into its own buffers.  For Wii/PSP FMOD_HARDWARE supports this flag for the GCADPCM/VAG formats.  On other platforms FMOD_SOFTWARE must be used, as sound hardware on the other platforms (ie PC) cannot access main ram.  Cannot be freed after open, only after Sound::release.   Will not work if the data is compressed and FMOD_CREATECOMPRESSEDSAMPLE is not used. */
#define SOUND_OPENRAW                   0x00001000  /* Will ignore file format and treat as raw pcm.  Use FMOD_CREATESOUNDEXINFO to specify format.  Requires at least defaultfrequency, numchannels and format to be specified before it will open.  Must be little endian data. */
#define SOUND_OPENONLY                  0x00002000  /* Just open the file, dont prebuffer or read.  Good for fast opens for info, or when sound::readData is to be used. */
#define SOUND_ACCURATETIME              0x00004000  /* For System::createSound - for accurate Sound::getLength/Channel::setPosition on VBR MP3, and MOD/S3M/XM/IT/MIDI files.  Scans file first, so takes longer to open. FMOD_OPENONLY does not affect this. */
#define SOUND_MPEGSEARCH                0x00008000  /* For corrupted / bad MP3 files.  This will search all the way through the file until it hits a valid MPEG header.  Normally only searches for 4k. */
#define SOUND_NONBLOCKING               0x00010000  /* For opening sounds and getting streamed subsounds (seeking) asyncronously.  Use Sound::getOpenState to poll the state of the sound as it opens or retrieves the subsound in the background. */
#define SOUND_UNIQUE                    0x00020000  /* Unique sound, can only be played one at a time */
#define SOUND_3D_HEADRELATIVE           0x00040000  /* Make the sound's position, velocity and orientation relative to the listener. */
#define SOUND_3D_WORLDRELATIVE          0x00080000  /* Make the sound's position, velocity and orientation absolute (relative to the world). (DEFAULT) */
#define SOUND_3D_INVERSEROLLOFF         0x00100000  /* This sound will follow the inverse rolloff model where mindistance = full volume, maxdistance = where sound stops attenuating, and rolloff is fixed according to the global rolloff factor.  (DEFAULT) */
#define SOUND_3D_LINEARROLLOFF          0x00200000  /* This sound will follow a linear rolloff model where mindistance = full volume, maxdistance = silence.  Rolloffscale is ignored. */
#define SOUND_3D_LINEARSQUAREROLLOFF    0x00400000  /* This sound will follow a linear-square rolloff model where mindistance = full volume, maxdistance = silence.  Rolloffscale is ignored. */
#define SOUND_3D_CUSTOMROLLOFF          0x04000000  /* This sound will follow a rolloff model defined by Sound::set3DCustomRolloff / Channel::set3DCustomRolloff.  */
#define SOUND_3D_IGNOREGEOMETRY         0x40000000  /* Is not affect by geometry occlusion.  If not specified in Sound::setMode, or Channel::setMode, the flag is cleared and it is affected by geometry again. */
#define SOUND_UNICODE                   0x01000000  /* Filename is double-byte unicode. */
#define SOUND_IGNORETAGS                0x02000000  /* Skips id3v2/asf/etc tag checks when opening a sound, to reduce seek/read overhead when opening files (helps with CD performance). */
#define SOUND_LOWMEM                    0x08000000  /* Removes some features from samples to give a lower memory overhead, like Sound::getName.  See remarks. */
#define SOUND_LOADSECONDARYRAM          0x20000000  /* Load sound into the secondary RAM of supported platform. On PS3, sounds will be loaded into RSX/VRAM. */
#define SOUND_VIRTUAL_PLAYFROMSTART     0x80000000

namespace RootEngine
{
	namespace Sound
	{
		class SoundAudioInterface
		{
		public:
			virtual bool LoadSound(std::string p_name, unsigned p_flags) = 0;
			virtual FMOD::Sound* GetFmodSound() = 0;
			virtual bool Is3D() = 0;
		};

		class SoundAudio : public SoundAudioInterface
		{
		public:
			SoundAudio(FMOD::System* p_system);
			~SoundAudio();
			bool LoadSound(std::string p_name, unsigned p_flags);
			FMOD::Sound* GetFmodSound();
			bool Is3D();
		private:
			FMOD::System*		m_system;
			FMOD::Sound*		m_sound;
			bool m_is3D;
		};
	}
}
