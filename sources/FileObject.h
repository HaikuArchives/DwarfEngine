#ifndef D_FILEOBJECT_H_
#define D_FILEOBJECT_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */




namespace dwarf
{

		class String;
		
		/*!
			Unless you want to implement your own file media type you
			are not interested in this class.
			\brief internal representation of a file. It can be implemented 
			to support many different media types.
		*/
		class FileObject
		{
			public:
			
			virtual ~FileObject() {}

			/*!
				In your implementation this method is supposed to read from the file
				and fill the given buffer with data. It should read 'size' number of
				bytes and return the actual number of bytes read.
				\param buffer a pointer to the buffer your implementation should fill
				with data from the file.
				\param size the number of bytes to read from the file
				\return the number of bytes actually read from the file (may differ from
				size in case the file ends before all bytes are read).
			*/
			virtual int32 Read(void *buffer, int32 size) = 0;
			virtual int32 Write(const void *buffer, int32 size) = 0;
			
			/*!
				Your implementation of this method should set the current position, of the file, to
				the given offset (given in bytes).
			*/
			virtual void Seek(int32 position) = 0;
			
			/*!
				Your implementation of this method should return the size of the file, in bytes.
				\return the size of the file, in bytes
			*/
			virtual int32 Size() const = 0;
			virtual bool IsReadable() const = 0;
			virtual bool IsWritable() const = 0;
			virtual int32 Position() const = 0;
			virtual const String &Filename() const = 0;
			virtual void Flush() = 0;
			
			/*!
				Your implementation of this method should return a new instance of the file. That is,
				it should return an exact copy of this file, but not this file itself. The returned copy needs
				to be totally independent of this file. (like the POSIX dup() function). They should be
				two separate file objects refering to the same file.
				\return a copy of this instance
			*/
			virtual FileObject *Duplicate() const = 0;
		};

} // dwarf

#endif // D_FILEOBJECT_H_
