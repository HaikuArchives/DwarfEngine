#include "Dwarf.h"

using namespace dwarf;

int32 DwarfMain(Array<String> &commandline)
{
	Directory dir(TEXT("../data"));

	Window win(Rect(20, 20, 320, 240), TEXT("Window title"));

	ClearView clear(win.Bounds(), Color(0, 0, 0));
//	BitmapView bitmap(win.Bounds(), HTexture(&dir, TEXT("Bear.png")), View::KEEP_PROPORTIONS);
	NoiseView noise(win.Bounds(), View::FOLLOW_ALL_SIDES);

	win.AddChild(&clear);
//	win.AddChild(&bitmap);
	win.AddChild(&noise);

	win.Show();
	win.Run();
	return 0;
}
