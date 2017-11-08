/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "File.h"
#include "FileSystem.h"
#include "Directory.h"
#include "ListArray.h"

using namespace dwarf;

/*!
	\brief default constructor. Creates a directory initialized to the app dir.
*/
Directory::Directory():
	m_directory()
{
	char buffer[512];
	BasePath().ToChar(buffer, 512);
	m_directory.SetTo(buffer);
}

Directory::Directory(const String &path):
	m_directory()
{
	char buffer[1024];
	(BasePath() + path).ToChar(buffer, 1024);
	m_directory.SetTo(buffer);
}

Directory::Directory(const BDirectory *dir, const String &path):
	m_directory()
{
	char buffer[512];
	path.ToChar(buffer, 512);
	status_t stat = m_directory.SetTo(dir, buffer);

	if (stat != B_OK)
	{
		String message;
		message.Format(TEXT("Couldn't create filesystem \"%s\""), *path);
		switch(stat)
		{
			case B_BAD_VALUE:
				message += TEXT(", bad parameter");
			break;
			case B_BUSY:
				message += TEXT(", the node is busy and cannot be accessed");
			break;
			case B_ENTRY_NOT_FOUND:
				message += TEXT(", the path doesn't exist");
			break;
			case B_FILE_ERROR:
				message += TEXT(", a file system error prevented the operation");
			break;
			case B_LINK_LIMIT:
				message += TEXT(", recursive links in the filesystem");
			break;
			case B_NAME_TOO_LONG:
				message += TEXT(", the name is too long");
			break;
			case B_NO_MEMORY:
				message += TEXT(", not enough memory to complete operation");
			break;
			case B_NO_MORE_FDS:
				message += TEXT(", no more file descriptors (too many files open)");
			break;
			case B_NOT_A_DIRECTORY:
				message += TEXT(", some parts of the path is not a directory");
			break;
			case B_NOT_ALLOWED:
				message += TEXT(", the media is read-only");
			break;
			case B_PERMISSION_DENIED:
				message += TEXT(", permission denied");
			break;
			default:
			break;
		}
		throw IoError(message, TEXT("Directory::Directory()"));
		return;
	}
}

/*!
	\brief virtual destructor
*/
Directory::~Directory()
{
}

void Directory::GetContent(ListArray<String> &file_list) const
{
	m_directory.Rewind();

	BEntry entry;
	while (m_directory.GetNextEntry(&entry) != B_ENTRY_NOT_FOUND)
	{
		char name[B_FILE_NAME_LENGTH];
		entry.GetName(name);
		file_list.AddItem(String(name));
	}
}

/*!
	\brief create a new filesystem (directory)
	\param name the name of the sub-filesystem to create
	\returns a pointer to the newly created sub-filesystem
	\warning you have the responsibility to delete the returned filesystem
*/
void Directory::CreateFileSystem(const String &name)
{
	char buffer[512];
	name.ToChar(buffer, 512);
	BDirectory dir;
	status_t stat = m_directory.CreateDirectory(buffer, &dir);

	if (stat != B_OK)
	{
		String message;
		message.Format(TEXT("Couldn't open directory \"%s\""), *name);
		switch(stat)
		{
			case B_BAD_VALUE:
				message += TEXT(", bad parameter");
			break;
			case B_BUSY:
				message += TEXT(", the node is busy and cannot be accessed");
			break;
			case B_ENTRY_NOT_FOUND:
				message += TEXT(", the path doesn't exist");
			break;
			case B_FILE_ERROR:
				message += TEXT(", a file system error prevented the operation");
			break;
			case B_LINK_LIMIT:
				message += TEXT(", recursive links in the filesystem");
			break;
			case B_NAME_TOO_LONG:
				message += TEXT(", the name is too long");
			break;
			case B_NO_MEMORY:
				message += TEXT(", not enough memory to complete operation");
			break;
			case B_NO_MORE_FDS:
				message += TEXT(", no more file descriptors (too many files open)");
			break;
			case B_NOT_A_DIRECTORY:
				message += TEXT(", some parts of the path is not a directory");
			break;
			case B_NOT_ALLOWED:
				message += TEXT(", the media is read-only");
			break;
			case B_PERMISSION_DENIED:
				message += TEXT(", permission denied");
			break;
			default:
			break;
		}
		throw IoError(message, TEXT("Directory::CreateFileSystem()"));
		return;
	}
}

/*!
	\brief open a filesystem (directory) relative to this one
	\param name the sub-filesystem to open
	\returns a pointer to the newly opened sub-filesystem
	\warning you have the responsibility to delete the returned filesystem
*/
void Directory::OpenFileSystem(const String &name)
{
	char buffer[512];
	name.ToChar(buffer, 512);
	status_t stat = m_directory.SetTo(&m_directory, buffer);

	if (stat != B_OK)
	{
		String message;
		message.Format(TEXT("Couldn't open directory \"%s\""), *name);
		switch(stat)
		{
			case B_BAD_VALUE:
				message += TEXT(", bad parameter");
			break;
			case B_BUSY:
				message += TEXT(", the node is busy and cannot be accessed");
			break;
			case B_ENTRY_NOT_FOUND:
				message += TEXT(", the path doesn't exist");
			break;
			case B_FILE_ERROR:
				message += TEXT(", a file system error prevented the operation");
			break;
			case B_LINK_LIMIT:
				message += TEXT(", recursive links in the filesystem");
			break;
			case B_NAME_TOO_LONG:
				message += TEXT(", the name is too long");
			break;
			case B_NO_MEMORY:
				message += TEXT(", not enough memory to complete operation");
			break;
			case B_NO_MORE_FDS:
				message += TEXT(", no more file descriptors (too many files open)");
			break;
			case B_NOT_A_DIRECTORY:
				message += TEXT(", some parts of the path is not a directory");
			break;
			case B_NOT_ALLOWED:
				message += TEXT(", the media is read-only");
			break;
			case B_PERMISSION_DENIED:
				message += TEXT(", permission denied");
			break;
			default:
			break;
		}
		throw IoError(message, TEXT("Directory::OpenFileSystem()"));
	}
}


/*!
	\brief open a file in this directory
	\param name the name of the file to open
	\return a pointer to the newly opened file
	\warning you have the responsibility to delete the returned file
*/
FileObject *Directory::NewFile(const String &path, File::OpenMode mode, uint32 flags)
{
	uint32 b_mode = B_READ_ONLY;
	switch (mode)
	{
		case File::READ: b_mode = B_READ_ONLY; break;
		case File::WRITE: b_mode = B_WRITE_ONLY; break;
		case File::READ_WRITE: b_mode = B_READ_WRITE; break;
	}

	if (flags & File::CREATE) b_mode |= B_CREATE_FILE;
	return new DirFile(&m_directory, path, b_mode);
}

// ******************************************
// *     DirFile
// ******************************************

Directory::DirFile::DirFile(const BDirectory *dir, const String &path, uint32 open_mode):
	m_filename(path),
	m_file()
{
	char buffer[512];
	path.ToChar(buffer, 512);
	status_t stat = m_file.SetTo(dir, buffer, open_mode);

//	if the file wasn't initilazed correctly, throw IoError()

	if (stat != B_OK)
	{
		String message;
		message.Format(TEXT("Couldn't open file \"%s\""), *path);
		switch(stat)
		{
			case B_FILE_EXISTS:
				message += TEXT(", file already exists");
			break;
			case B_BAD_VALUE:
				message += TEXT(", bad path");
			break;
			case B_ENTRY_NOT_FOUND:
				message += TEXT(", file not found");
			break;
			case B_NO_MEMORY:
				message += TEXT(", out of memory");
			break;
			case B_PERMISSION_DENIED:
				message += TEXT(", permission denied");
			break;
			default:
			break;
		}
		throw File::IoError(message, TEXT("DirFile::DirFile()"));
	}
}

Directory::DirFile::DirFile(const DirFile &file):
	m_filename(file.m_filename),
	m_file(file.m_file)
{
}

Directory::DirFile::~DirFile()
{
	logger(TEXT("DirFile::~DirFile()"));
}

int32 Directory::DirFile::Read(void *buffer, int32 size)
{
	return m_file.Read(buffer, size);
}

int32 Directory::DirFile::Write(const void *buffer, int32 size)
{
	return m_file.Write(buffer, size);
}

void Directory::DirFile::Seek(int32 position)
{
	m_file.Seek(position, SEEK_SET);
}

int32 Directory::DirFile::Size() const
{
	off_t size;
	m_file.GetSize(&size);
	return size;
}

int32 Directory::DirFile::Position() const
{
	return m_file.Position();
}

bool Directory::DirFile::IsReadable() const
{
	return m_file.IsReadable();
}

bool Directory::DirFile::IsWritable() const
{
	return m_file.IsWritable();
}

const String &Directory::DirFile::Filename() const
{
	return m_filename;
}

void Directory::DirFile::Flush()
{
	m_file.Sync();
}

FileObject *Directory::DirFile::Duplicate() const
{
	return new DirFile(*this);
}
