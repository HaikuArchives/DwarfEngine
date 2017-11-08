#ifndef D_PAKFILE_H_
#define D_PAKFILE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "FileSystem.h"
#include "String.h"
#include "FileObject.h"
#include "File.h"

#include "minizip/unzip.h"


namespace dwarf
{

		/*!
			The PakFile class is an implementation of FileSystem that represents the filesystem within a zip file.
			This class uses the open sourced zlib and some additional source, Copyright (C) 1998 Gilles Vollant,
			making it possible to read zip files (zlib can only read gzip files). For more information see "source/minizip/unzip.h".
			\brief represents a filesystem within a zip file.
			\warning please note that this interface is read-only. You cannot create zip archives using this class. That means that
			CreateFileSystem() won't work.
			Another limitation of this filesystem is that it's really slow when Seek():ing. This is because the zip-file has to be extracted in order
			to get to the seek'ed position. Try to avoid seeking whne using a PakFile.
		 */
		class PakFile : public FileSystem
		{
		public:

			PakFile(const String &path) throw(FileSystem::IoError);
			virtual ~PakFile();

			virtual void OpenFileSystem(const String &name) throw(FileSystem::IoError);
			virtual void CreateFileSystem(const String &name) throw(FileSystem::IoError);

			virtual void GetContent(ListArray<String> &file_list) const;

		private:

			virtual FileObject *NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(FileSystem::IoError);

			class PFile : public FileObject
			{
			public:

				PFile(const String &pakfile_path, const String &file_path, uint32 openMode, bool textfile) throw(FileSystem::IoError);
				virtual ~PFile();

				virtual int32 Read(void *buffer, int32 size);
				virtual int32 Write(const void *buffer, int32 size);

				virtual void Seek(int32 position);

				virtual int32 Size() const;
				virtual int32 Position() const;

				virtual bool IsReadable() const;
				virtual bool IsWritable() const;

				virtual const String &Filename() const;
				virtual void Flush();
				virtual FileObject *Duplicate() const;

			private:

				// the Seek() method, only move the virtual file pointer to optimize performance
				// the the user tries to read at the current virtual file pointer, this method is used to
				// actually move the (real) file pointer
				void seek_real(int32 position);

				PFile(const PFile &file);

				String m_filename;
				String m_path; // the path to the zipfile, enables duplication
				unzFile m_file;
				unz_file_info m_info;
				int32 m_virtual_position;

			};

			PakFile(const String &pakfile_path, const String &path) throw(IoError);

			String m_pakfile_path;	// the real path to the zip file
			String m_path;			// the current path within the zipfile

		}; 

} // dwarf

#endif // D_PAKFILE_H_
