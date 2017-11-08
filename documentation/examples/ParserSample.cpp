#include "Dwarf.h"

using namespace dwarf;

int32 DwarfMain(Array<String> &commandline)
{
// set up window and directories
	Directory dir(TEXT("../data/fonts"));

	Window win(Rect(20, 20, 320, 500), TEXT("Output")/*, Window::UNRESIZABLE*/);

// create views
	ClearView clear(win.Bounds(), Color(0, 0, 0));

	Font font(&dir, TEXT("Swis721 BT, Bold Italic.font"));
	TextView textview(win.Bounds(), font, View::FOLLOW_ALL_SIDES);

// add views
	win.AddChild(&clear);
	win.AddChild(&textview);

// create a buffer and read the file into it
//	PakFile app_dir(TEXT("pakfile.zip"));
	Directory app_dir;
	File file(&app_dir, TEXT("dwarf.log"));

	TextBuffer text;
	file.Read(text);

// add the text to the text view

	for (int32 i = 0; i < 40; i++)
	{
		textview.AddLine(text.ReadString(), TextView::LEFT, Color(1.0f, 1.0f, 1.0f), 15.0f);
	}

	win.Show();
	win.Run();
	return 0;

}
