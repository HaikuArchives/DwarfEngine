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
#include "FileSystem.h"
#include "String.h"

#ifdef _DEBUG
#include "nommgr.h"
#endif

namespace dwarf
{


		class SoundPlayer;

		/*!
			\brief represents a sound that is stored on disk.
			In some sense it's a wrapper for File that has the ability
			to play the file's content.
		*/
		class SoundFile : public Sound
		{
		friend class SoundPlayer;
		public:

			//! the flags for the SOundFile constructor
			enum flags
			{
				CACHE_IN_MEMORY = 1 //!< The entire file is loaded into memory and played from there, instead of playing it from disk
			};

			SoundFile(FileSystem *dir, const String &filename, uint32 flags = 0);
			virtual ~SoundFile();

		protected:

			virtual bool AddToBuffer(float left_volume, float right_volume, int16 *buffer, int32 num_samples, int32 instance_id);
			virtual int32 StartInstance();
			virtual void StopInstance(int32 instance_id);

		private:

			class FilePositionIO : public BPositionIO
			{
			public:
				FilePositionIO(FileObject *file): BPositionIO(), m_file(file) {}
				virtual FilePositionIO::~FilePositionIO() {}

				virtual off_t Seek(off_t position, uint32 mode)
				{
					if (mode == SEEK_CUR) position += m_file->Position();
					else if (mode == SEEK_END) position += m_file->Size();

					m_file->Seek(position);
					return m_file->Position();
				}
				virtual off_t Position() const { return m_file->Position(); }
				
				virtual ssize_t Read(void *buffer, size_t size) { return m_file->Read(buffer, size); }
				virtual ssize_t ReadAt(off_t position, void *buffer, size_t size) { off_t pos = Position(); Seek(position, SEEK_SET); int32 ret = m_file->Read(buffer, size); Seek(pos, SEEK_SET); return ret; }
			// not implemented
				virtual ssize_t Write(const void *buffer, size_t size) { return 0; }
				virtual ssize_t WriteAt(off_t position, const void *buffer, size_t size) { return 0; }

				void *operator new(size_t size) { return malloc(size); }
				void operator delete(void *ptr) { free(ptr); }

			private:
				FileObject *m_file;
			};
			
			Buffer m_buffer;
			const bool m_use_buffer;
			File *m_file;
			
			struct Instance
			{
				BPositionIO *data_stream;
				BMediaFile *media_file;
				BMediaTrack *track;

				int8 *buffer;
				int32 buffer_pos;
				int32 frame_count;
				int64 frames_left;
				
				media_raw_audio_format format;

				int8 default_data;
				int32 frame_size;
			};

			ListArray<Instance> m_instance;
		};

} // dwarf

#endif // D_SOUNDFILE_H_
