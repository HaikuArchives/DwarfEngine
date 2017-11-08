/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <typeinfo.h>
#include "fmod.h"

#include "Defs.h"

#include "SoundFile.h"
#include "SoundPlayer.h"

using namespace dwarf;

// callback functions used for FMOD file operations
static ListArray<FileObject *> m_files;

// I uses the name to pass FileObject pointer to the
// file I should open.
unsigned int OpenCallback(const char *name)
{
	FileObject *file = (FileObject*)name;
	return m_files.AddItem(file->Duplicate()) + 1;
}

void CloseCallback(unsigned int handle)
{
	handle--;
	delete m_files[handle];
	m_files.RemoveItemAt(handle);
}

int ReadCallback(void *buffer, int size, unsigned int handle)
{
	handle--;
	return m_files[handle]->Read(buffer, size);
}

int SeekCallback(unsigned int handle, int pos, signed char mode)
{
	handle--;

	if (mode == SEEK_END)
		pos += m_files[handle]->Size();
	else if (mode == SEEK_CUR)
		pos += m_files[handle]->Position();

	m_files[handle]->Seek(pos);
	return m_files[handle]->Position();	
}

int TellCallback(unsigned int handle)
{
	handle--;
	return m_files[handle]->Position();
}





// -------------------------------------------------------------------------





SoundFile::SoundFile(FileSystem *dir, const String &filename, uint32 flags):
	Sound(),
	m_buffer(0),
	m_file(0),
	m_use_buffer(flags & CACHE_IN_MEMORY)
{
	if (m_use_buffer)
	{
		File file(dir, filename);
		file.Read(m_buffer);
	}
	else
	{
		m_file = new File(dir, filename);
		FSOUND_File_SetCallbacks(
			OpenCallback,
			CloseCallback,
			ReadCallback,
			SeekCallback,
			TellCallback);
	}
}

bool SoundFile::AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id)
{
	return !m_instances[instance_id].end_reached;
}

int32 SoundFile::StartInstance()
{
	Instance inst;
	inst.end_reached = false;

	if (m_use_buffer) inst.stream = FSOUND_Stream_OpenFile((char *)m_buffer.Data(), FSOUND_LOADMEMORY, m_buffer.Size());
	else inst.stream = FSOUND_Stream_OpenFile((char *)m_file->m_file, 0, 0);

	int32 instance_id = m_instances.AddItem(inst);

	FSOUND_Stream_SetEndCallback(inst.stream, stop_instance, (int)&m_instances[instance_id]);
	FSOUND_Stream_Play(FSOUND_FREE, inst.stream);

	return instance_id;
}

void SoundFile::StopInstance(int32 instance_id)
{
	Instance &inst = m_instances[instance_id];

	if (!inst.end_reached) FSOUND_Stream_Close(inst.stream);
	m_instances.RemoveItemAt(instance_id);
}

SoundFile::~SoundFile()
{
	// this makes the soundserver remove all playing instances of this sound
	// (and of course call StopInstance() on them). It's very important
	// to call this method.
	Close();

	// if we streamed the file, close it.
	if (!m_use_buffer) delete m_file;
}

signed char SoundFile::stop_instance(FSOUND_STREAM *, void *, int, int param)
{
	Instance *inst = (Instance*)param;
	inst->end_reached = true;
	return 0;
}
