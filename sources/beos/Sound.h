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
			SoundPlayer Play(float volume = .3f, float pan = 0.0f, float start_time = 0.0f) throw(NoSound);

		protected:
			/*!
				This is the method that the subclasses has to implement.
				It is supposed to add it's sound to the given float buffer, containing num_samples of samples
				\param left_volume the volume of your sound (multiply this with all of your left-channel-samples). This value is in the range [0, 1]
				\param right_volume the volume of your sound (multiply this with all of your right-channel-samples). This value is in the range [0, 1]
				\param buffer the sound buffer that this sound should add it's sound to.
				\param num_samples the number of samples in the buffer (one sample == one float). This parameter is guaranteed
				to be an even number (because there are two interleaved channels)
				\param instance_id the instance id that you returned from StartInstance(). This tells you e.g. where in the sound you should play.
				\return true if the entire buffer was filled, returns false if not the entire buffer was filled, and the
				sound has reached it's end. If you return false the current instance will be removed from the sound server, and AddToBuffer() will
				not be called with this instance id anymore (unless StartInstance() is called and it returns this id again, but then it will refer to a new instance).
				\warning The buffer may contain audio data, be careful that you don't overwrites it, you should ADD your data to it (that's
				done using the plus operator +). Your audio stream is supposed to be in the range [-1, 1].
				\todo explain the interlaced stereo system. 
				\sa StartInstance()
			*/
			virtual bool AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id) = 0;
			
			/*!
				The sound object has to maintain a list of all playing instances, where every entry
				maybe should contain the current position of that instance. Remember that the same sound
				can be played more than once, at one given moment. Every time someone calls
				Play() on your sound, a new instance is started. An instance is ended when you return false
				from AddToBuffer (the instance that is ended is the one AddToBuffer() was supposed to play).
				ListArray is a class that is very suitable to maintain this kind of list.
				\brief is called by the sound server every time the sound is to be started.
				\return you should return a uniqe instance id number, that is used when AddToBuffer() is called.
				\sa AddToBuffer()
			*/
			virtual int32 StartInstance() = 0;
			
			/*!
				\breif This method is called every time an instance of the sound stops playing
				No matter why it stops playing, it may have died when AddToBuffer() returned false, it may
				have been forced to stop when somebody called Stop() on it's SoundPlayer object.
				Every call to StartInstance() has it's corresponding call to StopInstance(). StopInstance() is
				called exactly once per instance.
				If you're using a list to keep track you your instances, this is the perfect
				place to remove the instance from the list.
			*/
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
				static void buffer_player(void *, void *buffer, size_t size, const media_raw_audio_format &format);

				struct PlayingSound
				{
					PlayingSound() :
						sound(0),
						instance_id(0),
						pan(0.0f),
						volume(1.0f),
						l_volume(1.0f),
						r_volume(1.0f),
						id(-1),
						start_time(0),
						semaphore(create_sem(1, "sound semaphore"))
					{};

					Sound *sound; // the sound that's playing
					int32 instance_id; // the current position in the sound, in number of frames
					float pan; // the pan
					float volume; // the volume of the plsying sond
					float l_volume; // precalced left_volume
					float r_volume; // precalced right_volume
					int32 id; // the number to identify a particular sound
					bigtime_t start_time; // the time when the sound started
					sem_id semaphore; // the semaphore will be acquired as long as the sound still is playing (is used by WaitForSound())
				};
				BSoundPlayer m_player;
				
				// the list that contain all currently playing channels
				ListArray<PlayingSound> m_channels;

				int32 m_free_id;
				
				int32 m_ref_count; // the reference counter (the number of sounds that still exists)
				sem_id m_play_sem; // this semaphore is acquired by the thread that makes critical changes to the channels list
			};
			friend class Sound::SoundServer;

			static SoundServer *m_server;
		};

} // dwarf

#endif // D_SOUND_H_
