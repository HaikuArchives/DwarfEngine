/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "File.h"
#include "FileObject.h"
#include "FileSystem.h"
#include "String.h"
#include "Buffer.h"

using namespace dwarf;

/*!
	\brief copy contructor
*/
File::File(const File &file):
	m_file(file.m_file->Duplicate())
{
}

/*!
	\brief file constructor
	\param directory a pointer to a FileSystem object (e.g. Directory) where the file
	you want to open is
	\param path the path and filename of the file you want to open.
	\param mode the mode in which you want to open the file. Available choises are:
	\li \e File::READ opens the file for read only
	\li \e File::WRITE opens the file for write only
	\li \e File::READ_WRITE opens the file in both read and write mode
	\param flags the flags that apply to this file, can be any combination of
	\li \e File::CREATE creates a new file if the given does not exist
	\li \e File::TEXT identifies the file as a textfile
	\li \e File::APPEND all write operations to the file are appended to the end
	\exception may throw File::IoError() on error
*/
File::File(FileSystem *directory, const String &path, OpenMode mode, uint32 flags) throw(IoError):
	m_file(directory->NewFile(path, mode, flags))
{
}

/*!
	\brief virtual destructor
*/
File::~File()
{
	delete m_file;
}

/*!
	\brief copy operator
*/
const File &File::operator=(const File &file)
{
	D_ASSERT_MESSAGE(m_file != 0, TEXT("The file you tried to assign with a new file is not initialized."));

	delete m_file;
	m_file = file.m_file->Duplicate();
	return *this;
}

/*!
	\brief writes the buffer to the file
	\param buffer the buffer containing the data to write
	\param size the size of the buffer given in bytes
*/
int32 File::Write(const void *buffer, int32 size)
{
	return m_file->Write(buffer, size);
}

/*!
	Any pending writes for this file will be executed and finished when the method returns.
	\brief flushes the disk cache for this file.
*/
void File::Flush()
{
	m_file->Flush();
}

/*!
	\brief reads the given number of bytes from the file, returns the number of bytes read.
	\param buffer a buffer to be filled with data from the file. The buffer
	will be cleared from all contents before it's filled with the file data.
	\param size the number of bytes you want to read from the file, if you don't give any size (or a size
	less than zero) the entire file will be read.
	\return The return value is always as much as you wanted to read (the given buffersize), unless the file ends
	\warning The buffer will be cleared from all contents before it's filled with the file data.
*/
int32 File::Read(Buffer &buffer, int32 size)
{
	buffer.Empty();
	if (size < 0) size = Size();

	int32 ret = m_file->Read(buffer.SupplyData(size), size);
	buffer.DataWritten(ret);
	buffer.Rewind();
	return ret;
}

/*!
	\brief writes the given number of bytes from the given buffer to the file
	\param buffer a buffer containing the data to write to the file
	\param size the number of bytes to read, if you set this parameter to -1
	the entire buffer will be written to the file (which is default).
	\return the actual number of bytes written to the file
*/
int32 File::Write(const Buffer &buffer, int32 size)
{
	if (size < 0 || size > buffer.Size()) size = buffer.Size();
	return m_file->Write(buffer.Data(), size);
}

TextOutStream &File::operator<<(const dchar *string)
{
	int32 len = 0;
	while (string[len]) len++;
#ifdef D_UNICODE
	char* buf = new char[len];

	for (int i = 0; i < len; i++)
		buf[i] = (char)string[i];

	m_file->Write(buf, len * sizeof(char));

	delete [] buf;

#else
	m_file->Write(string, len * sizeof(dchar));
#endif

	return *this;
}
