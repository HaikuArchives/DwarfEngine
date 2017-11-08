#include "Dwarf.h"
#include "CompositeDirectory.h"

using namespace dwarf;

int32 DwarfMain(Array<String> &commandline)
{
	// create two directories 
	Directory dir;
	Directory shader_dir(TEXT("../data/Shaders"));

	// create a composite directory
	CompositeDirectory sys;

	// and add the two "real" directories
	sys.AddFileSystem(&dir);
	sys.AddFileSystem(&shader_dir);

	// create a list and get the directory content
	ListArray<String> file_list;
	sys.GetContent(file_list);

	// print the list content to the log
	for (ListArray<String>::Iterator i = file_list.Begin(); i.IsValid(); i++)
		logger(Log::LOW, *i);

	return 0;
}
