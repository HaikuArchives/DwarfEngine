#ifndef D_SOUND_H_
#define D_SOUND_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Exception.h"
#include "ListArray.h"
#include "Semaphore.h"
#include "Timer.h"
//verboten..
//#include <dsound.h>
//#include <windows.h>
//#include "fmod.h"

struct FSOUND_STREAM;

namespace dwarf
{


		class SoundPlayer;

		/*!
			It has been implemented in some forms (to play different sounds)
			\brief This is the beseclass of a sound clip
		*/
		class Sound
		{
		friend class SoundPlayer;
		public:
		
			/*!
				\brief a class that's thrown when the sound-system cannot be initialized
			*/
			class NoSound : public Exception
			{
				public:
				NoSound(const String &message, const String &source) : Exception(message, source, TEXT("Sound::NoSound"))
				{}
			};

			Sound();
			virtual ~Sound();

			/*!
				\brief plays the sound
				\param volume the volume in wich the sound should be playing
				\param pan the left-right balance of the sound
				\param start_time the time, in seconds, where the sound should start play
				\warning as long as the sound is playing, this object must not be removed
			*/
			SoundPlayer Play(float volume = 1.0, float pan = 0.0, float start_time = 0.0) throw(NoSound);

		protected:
			/*!
				This is the method that the subclasses has to implement.
				It is supposed to add it's sound to the given float buffer, containing num_samples of samples
				\param left_volume the volume of your sound (multiply this with all of your left-channel-samples). This value is in the range [0, 1]
				\param right_volume the volume of your sound (multiply this with all of your right-channel-samples). This value is in the range [0, 1]
				\param buffer the sound buffer that this sound should add it's sound to.
				\param num_samples the number of samples in the buffer (one sample == one float). This parameter is guaranteed
				to be an even number (because there are two interleaved channels)
				\param frame_position the position in the sound (wherefrom in the sound we are playing) given in number of samples.
				\return true if the entire buffer was filled, returns false if not the entire buffer was filled, and the
				sound has reached it's end.
				\warning The buffer may contain audio data, be careful that you don't overwrites it, you should ADD your data to it (that's
				done using the plus operator +). Your audio stream is supposed to be in the range [-1, 1].
				\todo explain the interlaced stereo system. 
			*/
			virtual bool AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id) = 0;

			virtual int32 StartInstance() = 0;

			virtual void StopInstance(int32 instance_id) = 0;

			/*!
				\brief this method must be called in you destructor!
				A call to this method will make sure that all currently
				playing instances are stopped. The reason why this couldn't
				be done in the basclass' destructor is that by that time
				your part of the object has already been removed, and no pure
				virtual method calls can occur (to Stopinstance() for example)
			*/
			void Close() { m_server->RemoveSound(this); }
		private:

			class SoundServer
			{
			friend class Sound;
			public:
				int32 AddSoundPlayer(Sound *player, int32 &id);
				
				float Volume(int32 index, int32 id);
				void SetVolume(int32 index, int32 id, float volume);
				
				float Pan(int32 index, int32 id);
				void SetPan(int32 index, int32 id, float pan);
				
				bool IsPlaying(int32 index, int32 id) { return m_channels.ItemAt(index).id == id; }
				void WaitForSound(int32 index, int32 id);
				void Stop(int32 index, int32 id, bool nice = true);
				float Time(int32 index, int32 id);
				
				void RemoveSound(Sound *sound, bool nice = true);
				
				void AddRef() {m_ref_count++;}
				void RemoveRef();
			private:
				SoundServer() throw(NoSound);
				~SoundServer();
				static signed char buffer_player(FSOUND_STREAM *str, void *buffer, int length, int user);

				struct PlayingSound
				{
					PlayingSound() :
						sound(0),
						instance_id(-1),
						pan(0.0f),
						volume(1.0f),
						l_volume(1.0f),
						r_volume(1.0f),
						id(-1),
						time(),
						semaphore()
					{}
					PlayingSound(const PlayingSound &s):
						sound(s.sound),
						instance_id(s.instance_id),
						pan(s.pan),
						volume(s.volume),
						l_volume(s.l_volume),
						r_volume(s.r_volume),
						id(s.id),
						time(s.time),
						semaphore()
					{}
					const PlayingSound &operator=(const PlayingSound &s)
					{
						sound = s.sound;
						instance_id = s.instance_id;
						pan = s.pan;
						volume = s.volume;
						l_volume = s.l_volume;
						r_volume = s.r_volume;
						id = s.id;
						time = s.time;
						return *this;
					}
	
					Sound *sound; // the sound that's playing
					int32 instance_id; // the instance id of the playing sound, as returnened from StartInstance()
					float pan; // the pan
					float volume; // the volume of the plsying sond
					float l_volume; // precalced left_volume
					float r_volume; // precalced right_volume
					int32 id; // the number to identify a particular sound
					int64 time; // the time for this sound, tells you when the sound started playing
					Semaphore semaphore; // the semaphore will be acquired as long as the sound still is playing (is used by WaitForSound())
				};

				// the list that contain all currently playing channels
				ListArray<PlayingSound> m_channels;

				int32 m_free_id;
				FSOUND_STREAM *m_str; // the sound stream
				
				int32 m_ref_count; // the reference counter (the number of sounds that still exists)
				Semaphore m_play_sem; // this semaphore is acquired by the thread that makes critical changes to the channels list
				static SoundServer *m_this; // static this pointer, used from play_buffer
			};

			friend class Sound::SoundServer;
	
			static SoundServer *m_server;
		};


} // dwarf

#endif // D_SOUND_H_
