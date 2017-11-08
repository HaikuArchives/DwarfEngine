#ifndef D_COMPOSITE_DIRECTORY_H_
#define D_COMPOSITE_DIRECTORY_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "FileSystem.h"
#include "File.h"


namespace dwarf
{

		class String;

		class CompositeDirectory : public FileSystem
		{
		public:

			CompositeDirectory() throw(FileSystem::IoError);
			virtual ~CompositeDirectory();

			int32 AddFileSystem(FileSystem *directory);
			void RemoveFileSystem(int32 index);

			virtual void OpenFileSystem(const String &name) throw(FileSystem::IoError);
			virtual void CreateFileSystem(const String &name) throw(FileSystem::IoError);

			virtual void GetContent(ListArray<String> &file_list) const;

		private:

			virtual FileObject *NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(FileSystem::IoError);

			// needs to be mutable until const iterators are implemented
			mutable ListArray<FileSystem *> m_file_system;

		};

} // dwarf

#endif // D_COMPOSITE_DIRECTORY_H_
