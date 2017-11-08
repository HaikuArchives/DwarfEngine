/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <File.h>
#include <Roster.h>
#include <Application.h>

#include "Defs.h"

#include "Log.h"
#include "File.h"
#include "FileSystem.h"
#include "Exception.h"
#include "Resource.h"
#include "String.h"

using namespace dwarf;

/*!
	A normal file as described by the lunatic of arabia
*/
Resource::Resource() :
	m_name(),
	m_resource()
{
	app_info info;
	be_app->GetAppInfo(&info);
	BFile file(&info.ref, (long unsigned int)B_READ_ONLY);
	m_resource.SetTo(&file);
}

Resource::Resource(const String &name) :
	m_name(name),
	m_resource()
{
	if (m_name.RightString(1) != TEXT("/")) m_name += TEXT("/");

	app_info info;
	be_app->GetAppInfo(&info);
	BFile file(&info.ref, B_READ_ONLY);
	m_resource.SetTo(&file);
}

Resource::~Resource()
{
}

void Resource::CreateFileSystem(const String &name) throw(IoError)
{
	String path = m_name + name;
	String message;
	message.Format(TEXT("You cannot create directories within resources, you tried to create \"%s\"."), *path);
	throw FileSystem::IoError(message, TEXT("Resource::CreateFileSystem()"));
}

/*!
	\todo make this method throw exception upon fail
*/
void Resource::OpenFileSystem(const String &name) throw(IoError)
{
	m_name += name;
	if (m_name.RightString(1) != TEXT("/")) m_name += TEXT("/");
}

FileObject* Resource::NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError)
{
	return new ResFile(m_resource, m_name + path, mode, flags);
}

Resource::ResFile::ResFile(BResources &res, const String &path, File::OpenMode mode, uint32 flags) throw(IoError):
	m_filename(path),
	m_data()
{
	char buffer[1024];
	path.ToChar(buffer, 1024);

	size_t len;
	const void *data = res.LoadResource(B_RAW_TYPE, buffer, &len);

	if (data == NULL)
	{
		String message;
		message.Format(TEXT("Failed to open file \"%s\"."), *m_filename);
		throw FileSystem::IoError(message, TEXT("Resource::ResFile::ResFile"));
	}

	m_data.SetTo(data, len);
}

Resource::ResFile::ResFile(const ResFile &file):
	m_filename(file.m_filename),
	m_data(file.m_data)
{
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

FileObject *Resource::ResFile::Duplicate() const
{
	return new ResFile(*this);
}

