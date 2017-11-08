/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"
#include "Math.h"

#include "File.h"
#include "FileSystem.h"
#include "PakFile.h"

using namespace dwarf;

PakFile::PakFile(const String &pakfile_path, const String &path) throw(IoError):
	m_pakfile_path(pakfile_path),
	m_path(path)
{
	if (m_path.RightString(1) != TEXT("/")) m_path += TEXT("/");
}

PakFile::PakFile(const String &path) throw(IoError):
	m_pakfile_path(FileSystem::BasePath()+path),
	m_path()
{
	char buffer[1024];
	m_pakfile_path.ToChar(buffer, 1024);

	unzFile file = unzOpen(buffer);
	
	if (file == 0)
	{
		unzClose(file);
		String message;
		message.Format(TEXT("Couldn't open packfile \"%s\"."), *m_pakfile_path);
		throw IoError(message, TEXT("PakFile::PakFile()"));
	}
	
	unzClose(file);
}

/*!
	\brief virtual destructor
*/
PakFile::~PakFile()
{
}

/*!
	\brief create a new filesystem (directory)
	\param name the name of the sub-filesystem to create
	\returns a pointer to the newly created sub-filesystem
	\warning you have the responsibility to delete the returned filesystem
*/
void PakFile::CreateFileSystem(const String &name) throw(FileSystem::IoError)
{
	throw FileSystem::IoError(TEXT("You cannot create new directories within a PakFile."), TEXT("PakFile::CreateFileSystem()"));
}

/*!
	\brief open a filesystem (directory) relative to this one
	\param name the sub-filesystem to open
	\returns a pointer to the newly opened sub-filesystem
	\warning you have the responsibility to delete the returned filesystem
	\todo make this method throw exception upon fail
*/
void PakFile::OpenFileSystem(const String &name) throw(IoError)
{
	m_path += name;
	if (m_path.RightString(1) != TEXT("/")) m_path += TEXT("/");
}

/*!
	\brief open a file in this directory
	\param name the name of the file to open
	\return a pointer to the newly opened file
	\warning you have the responsibility to delete the returned file
*/
FileObject *PakFile::NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError)
{
	return new PFile(m_pakfile_path, m_path + path, mode, (flags & File::TEXT) != 0);
}

void PakFile::GetContent(ListArray<String> &file_list) const
{
	char buffer[1024];
	m_pakfile_path.ToChar(buffer, 1024);
	String filename;

	unzFile file = unzOpen(buffer);
	if (file == 0) return;
	
	if (unzGoToFirstFile(file) != UNZ_OK) return;
	
	do
	{

		unzGetCurrentFileInfo(file, 0, buffer, 1024, 0, 0, 0, 0);
		filename = buffer;
		if (filename.StartsWith(m_path)) file_list.AddItem(filename);

	} while(unzGoToNextFile(file) == UNZ_OK);
}

// ************* File **************

PakFile::PFile::PFile(const String &pakfile_path, const String &path, uint32 openMode, bool textfile) throw(IoError):
	m_path(pakfile_path),
	m_virtual_position(0)
{
	char buffer[1024];
	pakfile_path.ToChar(buffer, 1024);

	m_file = unzOpen(buffer);

	if (m_file == 0)
	{
		String message;
		message.Format(TEXT("Couldn't open packfile \"%s\""), *pakfile_path);
		throw File::IoError(message, TEXT("PakFile::PFile::PFile()"));
	}

	path.ToChar(buffer, 1024);

	if (unzLocateFile(m_file, buffer, 1) != UNZ_OK)
	{
		String message;
		message.Format(TEXT("File not found \"%s\""), *path);
		throw File::IoError(message, TEXT("PakFile::PFile::PFile()"));
	}

	if (unzOpenCurrentFile(m_file) != UNZ_OK)
	{
		String message;
		message.Format(TEXT("File couldn't be opened \"%s\""), *path);
		throw File::IoError(message, TEXT("PakFile::PFile::PFile()"));
	}
	
	unzGetCurrentFileInfo(m_file, &m_info, buffer, 1024, 0, 0, 0, 0);
	
	m_filename = buffer;
}
	
PakFile::PFile::~PFile()
{
	unzCloseCurrentFile(m_file);
	unzClose(m_file);
}

int32 PakFile::PFile::Read(void *buffer, int32 size)
{
	if (m_virtual_position == Size()) return 0;
	seek_real(m_virtual_position);
	int32 read_bytes = unzReadCurrentFile(m_file, buffer, size);
	m_virtual_position += read_bytes;
	return read_bytes;
}

int32 PakFile::PFile::Write(const void *buffer, int32 size)
{
	return 0;
}

void PakFile::PFile::Seek(int32 position)
{
	if (position > Size()) position = Size();
	m_virtual_position = position;
}

void PakFile::PFile::seek_real(int32 position)
{
	int32 pos = unztell(m_file);
	if (pos == position) return;
	if (pos > position)
	{
		unzCloseCurrentFile(m_file);
		unzOpenCurrentFile(m_file);
		pos = 0;
	}
	
	char buffer[1024];
	while(pos < position)
	{
		int32 r = unzReadCurrentFile(m_file, buffer, min((position - pos), (int32)1024));
		pos += r;
	}
}
	
int32 PakFile::PFile::Size() const
{
	return m_info.uncompressed_size;
}

int32 PakFile::PFile::Position() const
{
	return m_virtual_position;
}

bool PakFile::PFile::IsReadable() const
{
	return true;
}

bool PakFile::PFile::IsWritable() const
{
	return false;
}

const String &PakFile::PFile::Filename() const
{
	return m_filename;
}

void PakFile::PFile::Flush()
{
}

FileObject *PakFile::PFile::Duplicate() const
{
	PFile *new_file = new PFile(m_path, m_filename, File::READ, false);
	//! \todo make the file pointer correct too, by reading from the file
	return new_file;
}
