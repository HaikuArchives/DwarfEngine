/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <MemoryIO.h>

#include "Defs.h"

#include "SoundFile.h"
#include "SoundPlayer.h"

using namespace dwarf;

/*!
	\param dir a pointer to the FileSystem where the sound file is located.
	\param filename the sound file's name.
	\param flags Sets some extra attributes for this sound. It can be any
	combination of the following constants.
	\li \e SoundFile::CACHE_IN_MEMORY This flag makes the SoundFile load the
	file into memory, and keep it there as long as the SoundFIle object exists (until it's
	deleted). This should be used for small files that are played often.
*/
SoundFile::SoundFile(FileSystem *dir, const String &filename, uint32 flags):
	Sound(),
	m_buffer(0),
	m_use_buffer(flags & CACHE_IN_MEMORY),
	m_file(0)
{
	if (m_use_buffer)
	{
		File file(dir, filename);
		file.Read(m_buffer);
	}
	else
	{
		m_file = new File(dir, filename);
	}
}

/*!
	\brief closed the file
	All playing instances of this sound are stopped.
*/
SoundFile::~SoundFile()
{
	Close();

	if (!m_use_buffer) delete m_file;
}

int32 SoundFile::StartInstance()
{
	Instance inst;
	
	if (m_use_buffer)
		inst.data_stream = new BMemoryIO(m_buffer.Data(), m_buffer.Size());
	else
		inst.data_stream = new FilePositionIO(m_file->m_file);

	inst.media_file = new BMediaFile(inst.data_stream);

	status_t e = inst.media_file->InitCheck();

	if (e != B_OK && e != B_MISSING_SYMBOL) throw NoSound(String(strerror(e)), TEXT("SoundFile::StartInstance()"));

// ***********************************
//   Search for a track containing sound
// ***********************************
	media_format format;
	bool sound_found = false;
	int32 i = 0;
	int32 num_tracks = inst.media_file->CountTracks();
	do
	{
		// if we have looked on all tracks
		if (i >= num_tracks) throw NoSound(TEXT("unrecognized file format"), TEXT("SoundFile::StartInstance()"));

		// get the track at position i
		inst.track = inst.media_file->TrackAt(i);
		
		// ask for it's media format
		if (inst.track->EncodedFormat(&format) == B_NO_ERROR)
		{
			// and check if it's a sound track
			switch (format.type)
			{
			case B_MEDIA_RAW_AUDIO:
			case B_MEDIA_ENCODED_AUDIO:
				sound_found = true;
			default:
				break;
			}
		}
		i++;
	} while (!sound_found);
	


	inst.buffer_pos = 0;
	inst.frame_count = inst.track->CountFrames();
	inst.frames_left = 0;

	status_t err = inst.track->DecodedFormat(&format);
	if (err) throw NoSound(String(strerror(err)), TEXT("SoundFile::SoundFile"));

	inst.buffer = new int8[format.u.raw_audio.buffer_size];
	inst.format = format.u.raw_audio;

	switch (inst.format.format)
	{
	case media_raw_audio_format::B_AUDIO_UCHAR :
		inst.default_data = 0x80;
		inst.frame_size = 1;
		break;
	case media_raw_audio_format::B_AUDIO_SHORT :
		inst.default_data = 0;
		inst.frame_size = 2;
		break;
	case media_raw_audio_format::B_AUDIO_INT :
	case media_raw_audio_format::B_AUDIO_FLOAT :
		inst.default_data = 0;
		inst.frame_size = 4;
		break;
	}
	inst.frame_size *= inst.format.channel_count;
	
	return m_instance.AddItem(inst);
}

void SoundFile::StopInstance(int32 instance_id)
{
	Instance &inst = m_instance[instance_id];

	inst.media_file->ReleaseAllTracks();
	delete inst.media_file;
	delete inst.data_stream;
	
	delete []inst.buffer;
}

bool SoundFile::AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id)
{
	Instance &inst = m_instance[instance_id];

	int32 j = inst.buffer_pos; // index within the media files buffer
	int32 i = 0; // index within the systems sound buffer


	while (i < num_samples)
	{
		if (inst.frames_left == 0)
		{
			inst.track->ReadFrames(inst.buffer, &inst.frames_left, 0);
			j = 0;
			if (inst.frames_left == 0) return false;
		}

		switch (inst.format.format)
		{
		case media_raw_audio_format::B_AUDIO_SHORT : // ******************** SHORT ************
			if (inst.format.channel_count == 2)		// ** two channels
				while (inst.frames_left > 0 && i < num_samples)
				{
					buffer[i] += ((int16*)inst.buffer)[j] * left_volume;
					buffer[i+1] += ((int16*)inst.buffer)[j+1] * right_volume;
					j += 2;
	
					inst.frames_left--;
					i+=2;
				}
			else 									// ** one channel
				while (inst.frames_left > 0 && i < num_samples)
				{
					buffer[i] += ((int16*)inst.buffer)[j] * left_volume;
					buffer[i+1] += ((int16*)inst.buffer)[j] * right_volume;
					j++;
	
					inst.frames_left--;
					i+=2;
				}

			break;
		case media_raw_audio_format::B_AUDIO_INT : // ******************** INT ************

			while (inst.frames_left > 0 && i < num_samples)
			{
				
				buffer[i] += ((float)((int32*)inst.buffer)[j]) / (float)(1<<31) * left_volume;

				if (inst.format.channel_count == 2)
					buffer[i+1] += ((float)((int32*)inst.buffer)[j+1]) / (float)(1<<15) * right_volume;
				else
					buffer[i+1] += ((float)((int32*)inst.buffer)[j]) / (float)(1<<15) * right_volume;

				j += inst.format.channel_count;

				inst.frames_left--;
				i+=2;
			}
			break;
		case media_raw_audio_format::B_AUDIO_FLOAT : // ******************** FLOAT ************
			while (inst.frames_left > 0 && i < num_samples)
			{
				
				buffer[i] += ((float*)inst.buffer)[j] * 32767.0f * left_volume;

				if (inst.format.channel_count == 2)
					buffer[i+1] += ((float*)inst.buffer)[j+1] * 32767.0f * right_volume;
				else
					buffer[i+1] += ((float*)inst.buffer)[j] * 32767.0f * right_volume;

				j += inst.format.channel_count;

				inst.frames_left--;
				i+=2;
			}
			break;
		}
	}
	inst.buffer_pos = j;
	return true;
}
