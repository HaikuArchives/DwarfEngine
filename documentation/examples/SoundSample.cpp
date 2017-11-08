#include "Dwarf.h"


using namespace dwarf;

/*
class MySound : public Sound
{
public:
	// this has to be here!
	~MySound() { Close(); }

protected:

	bool AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id)
	{
		// look up the position for this instance
		float &position = m_instance[instance_id];

		String string;
		string.Format("AddToBuffer: %f\n", position);
		OutputDebugString(*string);

		// fill the buffer
		for (int32 i = 0; i < num_samples; i+=2)
		{
			// left channel
			buffer[i] = 0;//3000.0f * sin((float)(i + position) * .05f) * left_volume; // multiply with left channel volume
			
			// right channel
			buffer[i+1] = 30000.0f * sin((float)(i + position) * .00005f);// * right_volume; // multiply with right channel volume
		}

		m_instance[instance_id] += (float)num_samples;

//		position += num_samples;
		// we can't play forever
		if (position > 10000)	return false;
		return true;
	}


	int32 StartInstance()
	{
		return m_instance.AddItem(0);
	}


	void StopInstance(int32 instance_id)
	{
		m_instance.RemoveItemAt(instance_id);	
	}
private:

	ListArray<float> m_instance;

};
*/

int32 DwarfMain(Array<String> &commandline)
{
	Directory dir(TEXT("../data"));
//	PakFile dir(TEXT("../data/pakfile.zip"));

//	MySound snd;
	SoundFile snd(&dir, TEXT("music.mp3"));
//	SoundFile snd(&dir, TEXT("tada.wav"), SoundFile::CACHE_IN_MEMORY);
	
//	File f(&dir, TEXT("../data/test.st"));
//	SawteethSound snd(f);

	SoundPlayer player1 = snd.Play();

	// wait for the sound to finish
	player1.WaitForSound();

	return 0;
}
