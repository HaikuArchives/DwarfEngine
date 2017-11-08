#ifndef D_DIRECTORY_H_
#define D_DIRECTORY_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <Directory.h>
#include <File.h>

#include "Defs.h"

#include "FileSystem.h"
#include "String.h"
#include "FileObject.h"
#include "File.h"

namespace dwarf
{

		/*!
			The Directory class is an implementation of FileSystem that represents the directory structure of the harddrive.
			\brief represents a directory on the hard drive
		 */
		class Directory : public FileSystem
		{
			public:
	
			Directory();
			Directory(const String &path);
			virtual ~Directory();

			virtual void GetContent(ListArray<String> &file_list) const;
			virtual void OpenFileSystem(const String &name);
			virtual void CreateFileSystem(const String &name);

			protected:
	
			virtual FileObject *NewFile(const String &path, File::OpenMode mode, uint32 flags);
	
			private:
	
			Directory(const BDirectory *dir, const String &path);
	
	
	
			class DirFile : public FileObject
			{
			public:
	
				DirFile(const BDirectory *dir, const String &path, uint32 open_mode);
				virtual ~DirFile();
	
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
				
				DirFile(const DirFile &file);
	
				String m_filename;
				BFile m_file;
			};
			
			mutable BDirectory m_directory;
		};
	

} // dwarf 

#endif // D_DIRECTORY_H_
