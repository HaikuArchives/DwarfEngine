#ifndef D_FILE_H_
#define D_FILE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "TextOutStream.h"
#include "String.h"
#include "FileObject.h"
#include "Exception.h"

namespace dwarf
{

		class FileSystem;
	
		/*!
			This class represents a file, that file can live anywhere. All implementations
			of this class is private in the implementations of FileSystem. The most useful
			implementations of this class is the one that reads normal files (from the harddrive)
			and the one that reads files from a PAK file.
			\brief This is an abstract class that represents a file
		*/
		class File : public TextOutStream
		{
		friend class SoundFile;
		public:

			class IoError: public Exception
			{
			public:
				IoError(const String &message, const String &source) : Exception(message, source, TEXT("File::IoError"))
				{}
			};
	
			/*!
				\brief open modes to the File constructor
			*/
			enum OpenMode
			{
				READ = 0,	//!< opens the file in read-only mode, you can't write to the file
				WRITE,		//!< opend the file in write-only mode, you can't read from the file
				READ_WRITE	//!< opens the file in read and write mode, you can both read and write to/from the file
			};
	
			/*!
				\brief flags to the File constructor
			*/
			enum flags
			{
				CREATE	= 0x01,	//!< if the file doesn't exists, it will be created, if it exists, that file will be opened
				TEXT	= 0x02,	//!< the file will be opened as a textfile
				APPEND	= 0x04	//!< All writes are appended
			};
		
			File(const File &file);
			File(FileSystem *directory, const String &path, OpenMode mode = READ, uint32 flags = 0) throw(IoError);
			virtual ~File();

			int32 Size() const;
			void Flush();

			TextOutStream &operator<<(const dchar *string);

			const File &operator=(const File &file);
	
			int32 Read(Buffer &buffer, int32 size = -1);
			int32 Write(const Buffer &buffer, int32 size = -1);

			void Seek(int32 position);

			bool IsReadable() const;
			bool IsWritable() const;
			int32 Position() const;
			
			const String &Filename() const;

			virtual int32 Write(const void *buffer, int32 size);

		private:
	
			FileObject *m_file;
		};



		/*!
			\brief Sets teh current position in the file
		*/
		inline void File::Seek(int32 position)
		{
			m_file->Seek(position);
		}
		
		/*!
			\brief tells you the size of the file
			\return the size of the file, in bytes
		*/
		inline int32 File::Size() const
		{
			return m_file->Size();
		}
		
		/*!
			\brief tells you wether or not the file was opened in READ mode
			\return true if it is possible to read from the file
			(if it is not possible, this is probably because the file was initialized write-only)
		*/
		inline bool File::IsReadable() const
		{
			return m_file->IsReadable();
		}
		
		/*!
			\brief tells you waether you can write to the file or not
			\return true if it is possible to write to the file
			(if it is not possible, this is can be because the file was initialized
			read-only or it lives on a read-only media)
		*/
		inline bool File::IsWritable() const
		{
			return m_file->IsWritable();
		}
		
		/*!
			\brief tells you where the current file position is
			\return the current file position, measured in bytes from the beginning of the file.
		*/
		inline int32 File::Position() const
		{
			return m_file->Position();
		}

		/*!
			\brief tells you the file's name
			without the path to the file
			\return the file's name
		*/
		inline const String &File::Filename() const
		{
			return m_file->Filename();
		}

} // dwarf

#endif // D_FILE_H_
