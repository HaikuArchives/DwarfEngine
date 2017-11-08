#ifndef D_RESOURCE_H_
#define D_RESOURCE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "../Defs.h"

#include "../Buffer.h"
#include "../String.h"
#include "../FileSystem.h"
#include "../File.h"

/*
	Resource
*/

namespace dwarf
{

		/*!
			\brief Used to load resources contained in the executable itself.
			\todo Get it to work in windows..
		*/
		class Resource : public FileSystem
		{
			public:
	
			Resource();
	
			public:
	
			virtual ~Resource();
	
			virtual void OpenFileSystem(const String &name) throw(IoError);
			virtual void CreateFileSystem(const String &name) throw(IoError);

			virtual void GetContent(Array<String> &file_list);

			protected:
	
			Resource(const String &name);
			virtual FileObject *NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError);
	
			private:
			String m_name;		// name of the directory
			HMODULE m_handle;	// handle to the resource container
		
			class ResFile : public FileObject
			{
				public:
	
				ResFile(HMODULE module, const String &path, File::OpenMode mode, uint32 flags) throw(IoError);
				virtual ~ResFile();
	
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
				
				ResFile(const ResFile &file);
				
				HGLOBAL m_resource;

				String m_filename;
				Buffer m_data;

			};
	
		};

} // dwarf

#endif // D_RESOURCE_H_
