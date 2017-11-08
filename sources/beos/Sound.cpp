/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Sound.h"
#include "SoundPlayer.h"

using namespace dwarf;

Sound::SoundServer *Sound::m_server = 0;

/*!
	\brief initializes the sound to a silence lasting 0 seconds
	\todo The future planned SoundFile class, that plays a sound from disk, should be
	platform dependent and know about the SoundServer's internals; to optimize it.
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
{
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

void Sound::SoundServer::RemoveRef()
{
	m_ref_count--;
	if (m_ref_count == 0)
	{
		m_player.Stop();
		delete this;
	}
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
		acquire_sem(ch.semaphore);
		release_sem(ch.semaphore);
	}
}

void Sound::SoundServer::Stop(int32 index, int32 id, bool nice)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		// if we are nice, tell the sound that we are removing it's instance
		if (nice) ch.sound->StopInstance(ch.instance_id);
	
		ch.id = -1;
		ch.instance_id = 0;
		ch.sound = 0;
		release_sem(ch.semaphore);

		m_channels.RemoveItemAt(index);
	}
}

float Sound::SoundServer::Time(int32 index, int32 id)
{
	PlayingSound &ch = m_channels.ItemAt(index);
	if (ch.id == id)
	{
		if (ch.start_time == -1) return 0.0f; // the special case, before the sound has begun playing
		return (float)(m_player.CurrentTime() - m_player.Latency() - ch.start_time) / 1000000.0f;
	}
	return 0.0;
}

void Sound::SoundServer::RemoveSound(Sound *sound, bool nice)
{
	acquire_sem(m_play_sem);

	// remove all playing channels playing the given sound
	for (ListArray<PlayingSound>::Iterator i = m_channels.Begin(); i.IsValid(); i++)
	{
		PlayingSound &ch = *i;
		if (ch.sound == sound)
			Stop(i.Index(), ch.id, nice);
	}

	release_sem(m_play_sem);
}

static media_raw_audio_format audio_format =
{
	44100,	// sample rate
	2,		// number of channels
	media_raw_audio_format::B_AUDIO_SHORT,	// sample size (int16)
	B_MEDIA_HOST_ENDIAN,					// endian of samples
	512		// buffer size
};

Sound::SoundServer::SoundServer() throw(Sound::NoSound):
	m_player(&audio_format,"dwarf soundserver", buffer_player, 0, this),
	m_channels(),
	m_free_id(0),
	m_ref_count(0),
	m_play_sem(create_sem(1, "sound syncronization semaphore"))
{
	status_t stat = m_player.Start();
	if (stat != B_OK)
		throw NoSound(TEXT("Couldn't open sound stream"), TEXT("Sound::SoundServer::SoundServer()"));
}

int32 Sound::SoundServer::AddSoundPlayer(Sound *player, int32 &id)
{
	PlayingSound ch;
	
	acquire_sem(ch.semaphore);
	ch.id = m_free_id;
	ch.instance_id = player->StartInstance();
	ch.volume = 1.0f;
	ch.pan = 0.0f;
	ch.sound = player;
	ch.start_time = -1;

	int32 index = m_channels.AddItem(ch);

	id = m_free_id;
	
	m_free_id++;
	
	return index;
}

void Sound::SoundServer::buffer_player(void *param, void *buffer, size_t size, const media_raw_audio_format &format)
{
	SoundServer *server = (SoundServer*)param;

	// clear the buffer before any Sounds adds its data
	memset(buffer, 0, size);

	acquire_sem(server->m_play_sem);

	// let all sounds play to the buffer
	for (ListArray<PlayingSound>::Iterator i = server->m_channels.Begin(); i.IsValid(); i++)
	{
		PlayingSound &ch = *i;

		if (ch.start_time < 0) ch.start_time = server->m_player.CurrentTime() - server->m_player.Latency();

		// if the sound is finished, remove it
		if (!ch.sound->AddToBuffer(ch.l_volume, ch.r_volume, (int16*)buffer, size/sizeof(int16), ch.instance_id)) server->Stop(i.Index(), ch.id);
	}

	release_sem(server->m_play_sem);
}
