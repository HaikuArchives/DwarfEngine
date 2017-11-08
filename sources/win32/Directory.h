#ifndef D_DIRECTORY_H_
#define D_DIRECTORY_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "../Buffer.h"
#include "../String.h"
#include "../FileSystem.h"
#include "../File.h"

namespace dwarf
{

		/*!
			\brief represents a directory in the filesystem
		*/
		class Directory : public FileSystem
		{
			public:
	
			Directory();
			Directory(const String &name);
	
			public:
	
			virtual ~Directory();
	
	
			virtual void GetContent(ListArray<String> &file_list) const;
			virtual void OpenFileSystem(const String &name) throw(IoError);
			virtual void CreateFileSystem(const String &name) throw(IoError);
	
		private:
	
			virtual FileObject *NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError);
	
			String m_name;		// name of the directory
			String m_base_path;	// the base path, to the application
		
			class DirFile : public FileObject
			{
			public:
	
				DirFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError);
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
				FILE *m_file;

				String m_full_path;
				File::OpenMode m_mode;
				uint32 m_flags;
			};
	
		};

} // dwarf

#endif // D_DIRECTORY_H_
