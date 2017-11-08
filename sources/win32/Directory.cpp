/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <direct.h>
#include <io.h>

#include "../Defs.h"

#include "../Log.h"
#include "../File.h"
#include "../FileSystem.h"
#include "../Exception.h"
#include "Directory.h"
#include "../String.h"

using namespace dwarf;

/*!
	A normal file as described by the lunatic of arabia
*/
Directory::Directory() :
	m_name(),
	m_base_path(FileSystem::BasePath())
{
}

Directory::Directory(const String &name) :
	m_name(name),
	m_base_path(FileSystem::BasePath())
{
	m_name.ReplaceAll(CHAR('/'), CHAR('\\'));

	if (m_name.RightString(1) != TEXT("\\")) m_name += TEXT("\\");

	String path = m_base_path + m_name;

	dchar buffer[260];

	GetCurrentDirectory(260, buffer);
	if (SetCurrentDirectory(path) == FALSE)
	{
		SetCurrentDirectory(buffer);
		String message;
		message.Format(TEXT("Failed to open directory \"%s\""), *path);
		throw FileSystem::IoError(message, TEXT("Directory::CreateFileSystem()"));
	}
	SetCurrentDirectory(buffer);
}	

Directory::~Directory()
{
}

/*!
	\brief writes the contents of the directory to the array
*/
void Directory::GetContent(ListArray<String> &file_list) const
{
	WIN32_FIND_DATA data;
	ZeroMemory(&data, sizeof(data));
	HANDLE hFile;
//	int32 file_cnt = 0;
	int32 current = 0;

	String path = m_base_path + m_name + TEXT("*");
	
	path.ReplaceAll(CHAR('/'), CHAR('\\'));

	hFile = FindFirstFile(*path, &data);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(String(data.cFileName) != TEXT(".") && String(data.cFileName) != TEXT(".."))
			{
				file_list.AddItem(String(data.cFileName));
				current++;
			}
		} while(FindNextFile(hFile, &data));
	}
	FindClose(hFile);
}

void Directory::CreateFileSystem(const String &name) throw(IoError)
{
	String path = m_base_path + m_name + name;

	path.ReplaceAll(CHAR('/'), CHAR('\\'));

	if (!CreateDirectory(*path, 0))
	{
		String message;
		message.Format(TEXT("Failed to create directory \"%s\""), *path);
		throw IoError(TEXT("Failed to create directory."), TEXT("Directory::CreateFileSystem()"));
	}
}

void Directory::OpenFileSystem(const String &name) throw(IoError)
{
	m_name += name;
	m_name.ReplaceAll(CHAR('/'), CHAR('\\'));
	if (m_name.RightString(1) != TEXT("\\")) m_name += TEXT("\\");

	String path = m_base_path + m_name;

	path.ReplaceAll(CHAR('/'), CHAR('\\'));

	dchar buffer[1024];
	GetCurrentDirectory(1024, buffer);
	if (SetCurrentDirectory(path) == FALSE)
	{
		SetCurrentDirectory(buffer);
		String message;
		message.Format(TEXT("Failed to open directory \"%s\""), *path);
		throw IoError(message, TEXT("Directory::CreateFileSystem()"));
	}
	SetCurrentDirectory(buffer);
}

FileObject* Directory::NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError)
{
	String tmp = path;
	tmp.ReplaceAll(CHAR('/'), CHAR('\\'));
	tmp = m_base_path + m_name + tmp;

	return new DirFile(tmp, mode, flags);
}

Directory::DirFile::DirFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError):
	m_file(0),
	m_full_path(path),
	m_mode(mode),
	m_flags(flags)
{
	String mode_string;
	switch (m_mode)
	{
		case File::READ: mode_string = TEXT("r"); break;
		case File::WRITE: mode_string = TEXT("w"); break;
		case File::READ_WRITE: mode_string = TEXT("rw"); break;
	}

	if (flags & File::CREATE) mode_string += TEXT("+");
	if (!(flags & File::TEXT)) mode_string += TEXT("b");

	char buffer[1024];
	m_full_path.ToChar(buffer, 1024);
	
	char mode_buf[10];
	mode_string.ToChar(mode_buf, 10);
	
	m_file = fopen(buffer, mode_buf);
	if(m_file == NULL)
	{
		String message;
		message.Format(TEXT("Failed to open file \"%s\""), *path);
		throw File::IoError(message, TEXT("Directory::DirFile::DirFile"));
	}

	fseek(m_file, 0, SEEK_SET);
}

Directory::DirFile::~DirFile()
{
	if(m_file)
	{
		fclose(m_file);
		m_file = 0;
	}
}

int32 Directory::DirFile::Read(void *buffer, int32 size)
{
	D_ASSERT(buffer);

	int32 read = fread(buffer, 1, size, m_file);
	return read;
}

int32 Directory::DirFile::Write(const void *buffer, int32 size)
{
	D_ASSERT(buffer);

	int32 r = fwrite(buffer, size, 1, m_file);
	fflush(m_file);
	return r;
}

void Directory::DirFile::Seek(int32 position)
{
	fseek(m_file, position, SEEK_SET);
}

int32 Directory::DirFile::Size() const
{
	int32 position = ftell(m_file);
	fseek(m_file, 0, SEEK_END);
	int32 size = ftell(m_file);
	fseek(m_file, position, SEEK_SET);
	return size;
}

int32 Directory::DirFile::Position() const
{
	return ftell(m_file);
}
	
bool Directory::DirFile::IsReadable() const
{
	return (m_flags & File::READ) || (m_flags & File::READ_WRITE);
}

bool Directory::DirFile::IsWritable() const
{
	return (m_flags & File::WRITE) || (m_flags & File::READ_WRITE);
}

const String& Directory::DirFile::Filename() const
{
	return m_full_path;
}

void Directory::DirFile::Flush()
{
	fflush(m_file);
}

FileObject *Directory::DirFile::Duplicate() const
{
	DirFile *ret = new DirFile(m_full_path, m_mode, m_flags);
	fseek(ret->m_file, ftell(m_file), SEEK_SET);
	return ret;
}
