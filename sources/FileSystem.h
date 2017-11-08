#ifndef D_FILESYSTEM_H_
#define D_FILESYSTEM_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Exception.h"
#include "File.h"
#include "ListArray.h"
#include "Texture.h"


namespace dwarf
{

		class String;
		class FileObject;
		class FileSystem;
		class String;
	
	
		/*!
			This can be a packfile, a directory, an URL, whatever...
			\brief An abstract filesystem class (most commonly, a directory)
		 */
		class FileSystem
		{
			friend class File;
			// I really don't know why this is needed, but it seems to be.
			friend class CompositeDirectory;
			public:

			virtual ~FileSystem()
			{
				if (Texture::GetFileSystem() == this)
				{
					Texture::SetFileSystem(0);
				}
			}

			/*!
				\brief when this class is thrown when some I/O request failed
			*/
			class IoError: public Exception
			{
			public:
				IoError(const String &message, const String &source) : Exception(message, source, TEXT("FileSystem::IoError"))
				{}
			};

			/*!
				\brief Gives you a list of all files & directories in this directory
				\param file_list An array the will be filled with the filenames of all the files within this directory
			*/
			virtual void GetContent(ListArray<String> &file_list) const = 0;

			/*!
				This FileSystem object will point to the given sub-filesystem (or throw FileSystem::IoError exception).
				\brief opens a sub-file system
				\param name the name of the file system to open
				\exception FileSystem::IoError is thrown if the given filesystem doesn't exist
			*/
			virtual void OpenFileSystem(const String &name) throw(IoError) = 0;
	
			/*!
				The sub-filesystem is created (or FileSystem::IoError is thrown). If you want to "open" the newly created
				directory, you have to call OpenFileSystem() with the directory name.
				\brief creates a new file system (directory)
				\param name the new file system's name
				\exception FileSystem::IoError is thrown if it for some reason was impossible to create the file system (directory)
				\sa OpenFileSystem()
			*/
			virtual void CreateFileSystem(const String &name) throw(IoError) = 0;
	
			protected:
		
			/*!
				only used by the File class, you only need to know about this method if you're about to
				implement your own filesystem (and derive from this class). If you want to implement your own
				filesystem you must also implement a FileObject (preferably inside your filesystem class).
				\brief opens a file that lives in this filesystem
				\param path the the path to the file
				\param mode the mode in which the file should be opened (see File::OpenMode)
				\param flags the file flags (see File::Flags)
				\return a pointer to the given fileobject
				\exception FileSystem::IoError is thrown if the given file doesn't exist in this filesystem (or if it's not a file)
			*/

			virtual FileObject *NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError) = 0;

			public:
			/*!
				Implementation is OS Specific.
				\brief Returns the path to the application
				\return the path to the application (without the application's name)
			*/
			static String BasePath();


#ifdef WIN32
			public:
			/*!
				This is here strictly for efficiency reasons in windows.
			*/
			static void SetBasePath();

			static String m_Win32BasePath;
#endif

		};


} // dwarf

#endif // D_FILESYSTEM_H_
