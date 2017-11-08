#ifndef D_SOUNDPLAYER_H_
#define D_SOUNDPLAYER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

namespace dwarf
{

	class Timer;

		/*!
			Should be implemented as some kind of handle. Because it can be copied.
			\brief Represents a playing sound, a sound channel
		*/
		class SoundPlayer
		{
		friend class Sound;
		public:
			SoundPlayer(): m_sound_index(-1), m_id(-1) {}
			SoundPlayer(const SoundPlayer &player):
				m_sound_index(player.m_sound_index),
				m_id(player.m_id)
			{}
			~SoundPlayer() {}

			/*!
				\brief tells you the volume of the playing sound
				\return the volume of the playing sound, if the sound isn't playing anymore, it returns 0.0
			*/
			float Volume()
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				return Sound::m_server->Volume(m_sound_index, m_id);
			}
			void SetVolume(float volume)
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				Sound::m_server->SetVolume(m_sound_index, m_id, volume);
			}
			
			/*!
				\brief tells you the balance of the playing sound
				\return the left-right balance of the playing sound, if the sound isn't playing anymore, it returns 0.0
			*/
			float Pan()
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				return Sound::m_server->Pan(m_sound_index, m_id);
			}
			void SetPan(float pan)
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				Sound::m_server->SetPan(m_sound_index, m_id, pan);
			}

			/*!
				This can be used to sync graphics with the music.
				\brief returns the amount of time (in seconds) that has been played.
				\return the time since the sound started.
			*/
			float Time()
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				return Sound::m_server->Time(m_sound_index, m_id);
			}
			
			const SoundPlayer &operator=(const SoundPlayer &player)
			{
				m_sound_index = player.m_sound_index;
				m_id = player.m_id;
				return *this;
			};
			
			/*!
				\brief let's you know if the sound is finished playing, or not
				\return true if the entire sound has been played, or false if
				the sound is still playing. 
			*/
			bool IsPlaying()
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				return Sound::m_server->IsPlaying(m_sound_index, m_id);
			}
			
			/*!
				\brief blocks the calling thread until the sound is finished
			*/
			void WaitForSound()
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				Sound::m_server->WaitForSound(m_sound_index, m_id);
			}
			
			/*!
				After a call to this method, this instance of SoundPlayer will no longer be useful, because
				it refers to a playing sound, that's not playing anymore.
				\brief stops the sound from playing, and removes it from the list of playing sounds.
			*/
			void Stop()
			{
				D_ASSERT_MESSAGE(m_sound_index != -1 && m_id != -1, TEXT("You are trying to use an uninitialized SoundPlayer object."));
				Sound::m_server->Stop(m_sound_index, m_id);
			}
		private:
			SoundPlayer(int32 index, int32 id) : m_sound_index(index), m_id(id) {};
		
			// an index to the sound. The information about the playing sound
			// is stored in the Sound::SoundServer class. It's accessed through
			// a static pointer Sound::m_server. It is a private pointer within
			// the Sound class, but this class is a friend.
			int32 m_sound_index;
			
			// an id number that identifies this particular instance, this is used
			// when we want to check weather the sound is finished or not.
			// we can't just check the index because the index may be used by
			// a new sound.
			int32 m_id;
		};


} // dwarf

#endif // D_SOUNDPLAYER_H_
