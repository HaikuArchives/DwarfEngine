#include "Dwarf.h"

#include "ConsoleView.h"

using namespace dwarf;
/*
int32 DwarfMain(Array<String> &commandline)
{
	Directory dir(TEXT("../data"));
	File info(&dir, TEXT("head_info"), File::READ);
	Window win(Rect(20, 20, 640, 480), TEXT("Window title"), Window::UNRESIZABLE || Window::FULLSCREEN);

	ClearView clear(win.Bounds(), Color(1, 1, 1));
	DrawImageView fader(win.Bounds(), HTexture(&dir,TEXT("head.jpg")));

	Buffer buf;
	info.Read(buf);
	
	while (!buf.EndOfBuffer())
	{
		Rect tex;
		Rect pos;
		((InStream&)buf) >> (Serializable&)tex >> pos;
		fader.AddPatch(tex, pos);
	}
	
	win.AddChild(&clear);
	win.AddChild(&fader);

	win.Show();
	win.Run();

	return 0;
}
*/

int32 DwarfMain(Array<String> &commandline)
{
	Directory dir;
	dir.OpenFileSystem(TEXT("../data/fonts"));
	File font(&dir, TEXT("Verdana, Bold Italic.font"), File::READ);
	Window win(Rect(20, 20, 320, 240), TEXT("Window title"));

	ClearView clear(win.Bounds(), Color(1, 1, 1));
	ConsoleView console(win.Bounds(), 15.0f, Color(1,0,.5), 256, View::KEEP_PROPORTIONS);

	win.AddChild(&clear);
	win.AddChild(&console);

	console << TEXT("hej hej.\n");
	console << TEXT("blä.\näéáñ\n");

	win.Show();
	win.Run();

	return 0;
}
