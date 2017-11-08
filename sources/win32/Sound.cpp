/*
 *	    The Dwarf Engine 3D Library
 *	  copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Sound.h"
#include "SoundFile.h"
#include "SoundPlayer.h"

#include "fmod.h"

using namespace dwarf;

Sound::SoundServer *Sound::m_server = 0;

/*!
	\brief initializes the sound to a silence lasting 0 seconds
*/
Sound::Sound()
{
	if (m_server == 0) m_server = new SoundServer();
	m_server->AddRef();
}

/*!
	\brief just the destructor
*/
Sound::~Sound()
{	// if the derived class didn't clean up it's instances, this call will crash the application
	m_server->RemoveSound(this, false);
	m_server->RemoveRef();
}


/*!
	\brief starts playing the sound
	\param start_value If you want you can supply a start value, the timer will then start at the
	specified time (given in seconds).
	\sa Current()
*/
SoundPlayer Sound::Play(float volume, float pan, float start_time) throw(NoSound)
{
	int32 id;
	int32 index = m_server->AddSoundPlayer(this, id);

	m_server->SetPan(index, id, pan);

	m_server->SetVolume(index, id, volume);

	return SoundPlayer(index, id);
}







// ***********************************************
//
//	SoundServer
//
// ***********************************************


Sound::SoundServer *Sound::SoundServer::m_this = 0;


void Sound::SoundServer::RemoveRef()
{
	m_ref_count--;
	if (m_ref_count == 0) delete this;
}

float Sound::SoundServer::Volume(int32 index, int32 id)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		return m_channels.ItemAt(index).volume;
	}
	return 0.0f;
}

float Sound::SoundServer::Pan(int32 index, int32 id)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		return m_channels.ItemAt(index).pan;
	}
	return 0.0f;
}

void Sound::SoundServer::SetPan(int32 index, int32 id, float pan)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		ch.pan = pan;
		ch.l_volume = ch.volume;
		ch.r_volume = ch.volume;
		if (pan < 0.0f) ch.r_volume *= 1 + pan;
		else if (pan > 0.0f) ch.l_volume *= 1 - pan;
	}
}

void Sound::SoundServer::SetVolume(int32 index, int32 id, float volume)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		ch.volume = volume;
		ch.l_volume = ch.volume;
		ch.r_volume = ch.volume;
		if (ch.pan < 0.0f) ch.r_volume *= 1 + ch.pan;
		else if (ch.pan > 0.0f) ch.l_volume *= 1 - ch.pan;
	}
}

void Sound::SoundServer::WaitForSound(int32 index, int32 id)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		// wait for the sound to finish
		ch.semaphore.Lock();
		ch.semaphore.Unlock();
	}
}

void Sound::SoundServer::Stop(int32 index, int32 id, bool nice)
{
	m_play_sem.Lock();

	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		// tell the sound that the instance is removed
		// (if we are nice that is)
		if (nice) ch.sound->StopInstance(ch.instance_id);

		// remove the instance
		ch.id = -1;
		ch.instance_id = -1;
		ch.sound = 0;
		ch.semaphore.Unlock();

		m_channels.RemoveItemAt(index);
	}

	m_play_sem.Unlock();
}

float Sound::SoundServer::Time(int32 index, int32 id)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
		// WHY DOESN'T THE FOLLOWING LINE COMPILE?!
//		if (typeid(ch.sound) == typeid(SoundFile))
			return (FSOUND_Stream_GetTime(((SoundFile*)ch.sound)->m_instances[ch.instance_id].stream)) / 1000.0;
//		else
//			return (FSOUND_Stream_GetTime(m_str) - ch.time) / 1000.0;
	else
		return 0.0;
}

void Sound::SoundServer::RemoveSound(Sound *sound, bool nice)
{
	// remove all playing channels playing the given sound
	for (ListArray<PlayingSound>::Iterator i = m_channels.Begin(); i.IsValid(); i++)
	{
		PlayingSound &ch = *i;
		if (ch.sound == sound)
			Stop(i.Index(), ch.id, nice);
	}
}


signed char Sound::SoundServer::buffer_player(FSOUND_STREAM *stream, void *buffer, int length, int param)
{
//	SoundServer *server = (SoundServer*)param;
	SoundServer *server = m_this;

	server->m_play_sem.Lock();

	memset(buffer, 0, length);

	// let all sounds play to the buffer
	for (ListArray<PlayingSound>::Iterator i = server->m_channels.Begin(); i.IsValid(); i++)
	{
		PlayingSound &ch = *i;

		// if the sound is finished, remove it
		if (!ch.sound->AddToBuffer(ch.l_volume, ch.r_volume, (int16*)buffer, length / 2, ch.instance_id))
			server->Stop(i.Index(), ch.id);
	}

	server->m_play_sem.Unlock();
	return TRUE;
}

Sound::SoundServer::SoundServer() throw(Sound::NoSound):
	m_channels(),
	m_free_id(0),
	m_ref_count(0),
	m_play_sem(true) // allow nesting with this semaphore
{
	m_this = this;

	FSOUND_Init(44100, 5, 0);

	m_str = FSOUND_Stream_Create(
		buffer_player,	// callback
		1024,			// buffersize
		FSOUND_16BITS | FSOUND_SIGNED | FSOUND_STEREO | FSOUND_LOOP_OFF,	// mode
		44100,			// framerate
		reinterpret_cast<int>(this));	// user data
	if (m_str == 0) throw NoSound(TEXT("Can't initialize device"), TEXT("Sound::SoundServer::SoundServer"));

	FSOUND_Stream_Play(0, m_str);
/* add code here */
}


Sound::SoundServer::~SoundServer()
{
	FSOUND_Stream_Close(m_str);
	FSOUND_Close();
}

int32 Sound::SoundServer::AddSoundPlayer(Sound *player, int32 &id)
{
	PlayingSound ch;
	
	ch.id = m_free_id;
	ch.instance_id = player->StartInstance();
	ch.volume = 1.0f;
	ch.pan = 0.0f;
	ch.sound = player;
	ch.time = FSOUND_Stream_GetTime(m_str);

	int32 index = m_channels.AddItem(ch);

	m_channels[index].semaphore.Lock();
	id = m_free_id;
	m_free_id++;
	return index;
}
