/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"
#include "SawteethSound.h"

using namespace dwarf;



SawteethSound::SawteethSound(File &file)
{
	file.Read(m_buffer);
}

bool SawteethSound::AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id)
{
	Instance &inst = m_instance[instance_id];

	int32 j = 2048 - inst.samples_left; // the position in the buffer
	int32 i = 0; // index within the systems sound buffer


	while (i < num_samples)
	{
		if (inst.samples_left == 0)
		{
			inst.song->Play(inst.buffer, 2048 / 2);
			inst.samples_left = 2048;
			j = 0;
//			if (inst.song->Looped()) return false;
		}

		while (inst.samples_left > 0 && i < num_samples)
		{
			buffer[i] += inst.buffer[j] * 32767.0f * left_volume;
			buffer[i+1] += inst.buffer[j+1] * 32767.0f * right_volume;

			inst.samples_left -= 2;
			i+=2;
			j+=2;
		}

	}
	return true;
}


int32 SawteethSound::StartInstance()
{
//	printf("startinstance\n");
	Instance inst;
	
	inst.buffer = new float[2048];
	inst.samples_left = 0;
	
	txt f((char*)m_buffer.Data(), (int)m_buffer.Size());
	inst.song = new Song(f);
	
	return m_instance.AddItem(inst);
}

void SawteethSound::StopInstance(int32 instance_id)
{
//	printf("stopinstance\n");
	Instance &inst = m_instance[instance_id];

	delete []inst.buffer;
	delete inst.song;

	m_instance.RemoveItemAt(instance_id);
}
