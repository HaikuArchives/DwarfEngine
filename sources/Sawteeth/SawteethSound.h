#ifndef D_SAWTEETH_H
#define D_SAWTEETH_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

/*
	NOTE that sawteeth is written and (c) by Jonas Norberg jn@earthling.net
	see www.bebits.com/
*/


#include "Sound.h"
#include "File.h"
#include "../Buffer.h"

// sawteeth files
#include "txt.h"
#include "song.h"

using namespace dwarf;

namespace dwarf
{

	/*!
		\todo use a semaphore to be sure we don't remove an instance at the same time we are playing it (write a platform dependent semaphore class)
	*/
	class SawteethSound : public Sound
	{
	public:
		SawteethSound(File &file);
		~SawteethSound() { Close(); }

	protected:

		bool AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id);
		int32 StartInstance();
		void StopInstance(int32 instance_id);

	private:

		struct Instance
		{
			float *buffer;	// the buffer we let sawteeth write to
			int32 samples_left;	// the number of unplayed frames in the current buffer (when this is 0, let sawteeth generate some more)
			Song *song;		// the song we are playing
		};

		Buffer m_buffer;

		ListArray<Instance> m_instance;

	};

} // dwarf

#endif // D_SAWTEETH_H
