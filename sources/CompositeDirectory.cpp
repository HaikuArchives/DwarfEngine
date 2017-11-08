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
#include "CompositeDirectory.h"
#include "ListArray.h"

using namespace dwarf;

int32 CompositeDirectory::AddFileSystem(FileSystem *directory)
{
	return m_file_system.AddItem(directory);
}

void CompositeDirectory::RemoveFileSystem(int32 index)
{
	m_file_system.RemoveItemAt(index);
}

CompositeDirectory::CompositeDirectory() throw(FileSystem::IoError):
	m_file_system(3)
{
}

/*!
	\brief virtual destructor
*/
CompositeDirectory::~CompositeDirectory()
{
}

/*!
	\brief create a new filesystem (directory)
	\param name the name of the sub-filesystem to create
	\returns a pointer to the newly created sub-filesystem
	\warning You cannot create directories with this class (because it represents multiple directories)
*/
void CompositeDirectory::CreateFileSystem(const String &name) throw(FileSystem::IoError)
{
	throw FileSystem::IoError(TEXT("You cannot create new directories within a CompositeDirectory, because it represents multiple directories."), TEXT("CompositeDirectory::CreateFileSystem()"));
}

/*!
	\brief open a filesystem (directory) relative to this one
	\param name the sub-filesystem to open
	\returns a pointer to the newly opened sub-filesystem
	\warning you have the responsibility to delete the returned filesystem
	\todo make this method throw exception upon fail
*/
void CompositeDirectory::OpenFileSystem(const String &name) throw(IoError)
{
	for (ListArray<FileSystem *>::Iterator i = m_file_system.Begin(); i.IsValid(); i++)
		(*i)->OpenFileSystem(name);
}

/*!
	\brief open a file in this directory
	\param name the name of the file to open
	\return a pointer to the newly opened file
	\warning you have the responsibility to delete the returned file
*/
FileObject *CompositeDirectory::NewFile(const String &path, File::OpenMode mode, uint32 flags) throw(IoError)
{
	for (ListArray<FileSystem *>::Iterator i = m_file_system.Begin(); i.IsValid(); i++)
	{
		try { return (*i)->NewFile(path, mode, flags); }
		catch (IoError e) {}
	}
	String message;
	message.Format(TEXT("File not found \"%s\""), *path);

	throw IoError(message, TEXT("CompositeDirectory::NewFile()"));
}

void CompositeDirectory::GetContent(ListArray<String> &file_list) const
{
//	logger(Log::LOW, TEXT("CompositeDirectory::GetContent()\n"));
	ListArray<String> list;
	ListArray<String> temp_list;

	for (ListArray<FileSystem *>::Iterator i = m_file_system.Begin(); i.IsValid(); i++)
	{
		temp_list.Empty();
		(*i)->GetContent(temp_list);
		list.Union(temp_list);
	}

	file_list.AddList(list);
}

