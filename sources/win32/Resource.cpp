/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "../Defs.h"


#include "../Log.h"
#include "../File.h"
#include "../FileSystem.h"
#include "../Exception.h"
#include "../String.h"

#include "Resource.h"

using namespace dwarf;

/*!
	A normal file as described by the lunatic of arabia
*/
Resource::Resource() :
	m_name(),
	m_handle(GetModuleHandle(0))
{
}

Resource::Resource(const String &name) :
	m_name(name),
	m_handle(GetModuleHandle(*name))
{
	if (m_name.RightString(1) != TEXT("/")) m_name += TEXT("/");

}

Resource::~Resource()
{
}

void Resource::GetContent(Array<String> &file_list)
{
	//TODO:Implement (along with the rest of this bloody class)
}


void Resource::CreateFileSystem(const String &name) throw(IoError)
{
	String message;
	message.Format(TEXT("You cannot create directories within resources, you tried to create \"%s\"."), *(m_name + name));
	throw FileSystem::IoError(message, TEXT("Resource::CreateFileSystem()"));
}

void Resource::OpenFileSystem(const String &name) throw(IoError)
{
	m_name += name;
}

FileObject* Resource::NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError)
{
	return new ResFile(m_handle, m_name + path, mode, flags);
}

Resource::ResFile::ResFile(HMODULE module, const String &path, File::OpenMode mode, uint32 flags) throw(IoError):
	m_filename(path),
	m_data()
{
	bool always_true = true;
	
	if (always_true)
	{
		String message;
		message.Format(TEXT("Failed to open file \"%s\"."), *m_filename);
		throw FileSystem::IoError(message, TEXT("Resource::ResFile::ResFile"));
	}

}

Resource::ResFile::~ResFile()
{
}

int32 Resource::ResFile::Read(void *buffer, int32 size)
{
	if (m_data.Position() + size > m_data.Size()) size = m_data.Size() - m_data.Position();
	memcpy(buffer, static_cast<uint8*>(m_data.Data()) + m_data.Position(), size);
	m_data.Seek(size);
	return size;
}

int32 Resource::ResFile::Write(const void *buffer, int32 size)
{
	return 0;
}

void Resource::ResFile::Seek(int32 position)
{
	m_data.Seek(position);
}

int32 Resource::ResFile::Size() const
{
	return m_data.Size();
}

int32 Resource::ResFile::Position() const
{
	return m_data.Position();
}
	
bool Resource::ResFile::IsReadable() const
{
	return true;
}

bool Resource::ResFile::IsWritable() const
{
	return false;
}

const String& Resource::ResFile::Filename() const
{
	return m_filename;
}

void Resource::ResFile::Flush()
{
}

//    End of Resource.cpp

