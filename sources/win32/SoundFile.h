#ifndef D_SOUNDFILE_H_
#define D_SOUNDFILE_H_

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
#include "Sound.h"
#include "File.h"

#include "Directory.h"


namespace dwarf
{


		class SoundPlayer;

		/*!
			It has been implemented in some forms (to play different sounds)
			\brief This is the beseclass of a sound clip
		*/
		class SoundFile : public Sound
		{
		friend class Sound::SoundServer; // to enable some optimization using fmod.dll
		friend class SoundPlayer;
		public:

			enum flags
			{
				CACHE_IN_MEMORY = 1
			};

			SoundFile(FileSystem *dir, const String &filename, uint32 flags = 0);
			virtual ~SoundFile();

		protected:

			virtual bool AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id);
			virtual int32 StartInstance();
			virtual void StopInstance(int32 instance_id);

		private:

			static signed char stop_instance(FSOUND_STREAM *stream, void *, int, int param);

			Buffer m_buffer;
			File *m_file;
			const bool m_use_buffer; // using buffer or file

			struct Instance
			{
				FSOUND_STREAM *stream;
				bool end_reached;
			};

			ListArray<Instance> m_instances;
		};

} // dwarf

#endif // D_SOUNDFILE_H_
