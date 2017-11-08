
#include "Dwarf.h"

using namespace dwarf;

// derive from TextView and make it draw a dark blue background
class MyView : public TextView
{
public:
	MyView(const Rect &frame, const Font &font, uint32 resize_mode):
		TextView(frame, font, 256, resize_mode)
	{}

	virtual void PreDraw(RenderingDevice &device) const
	{
		device.DisableState(RenderingDevice::BLEND);
		device.DisableState(RenderingDevice::TEXTURE_2D);
		device.SetColor(Color(0, 0, .5));

		device.Begin(RenderingDevice::QUADS);

		device.AddVertex(Point(0, 0));
		device.AddVertex(Point(0, Bounds().Height()));
		device.AddVertex(Point(Bounds().Width(), Bounds().Height()));
		device.AddVertex(Point(Bounds().Width(), 0));

		device.End();

		TextView::PreDraw(device);
	}
};


int32 DwarfMain(Array<String> &commandline)
{
	// create a window
	Window win(Rect(20, 20, 640, 480));

	// load a font
	Directory dir(TEXT("../data/fonts/"));
	File file(&dir, TEXT("Arial, Bold Italic.font"));
	Font font(file);

	// create the views
	ClearView clear(win.Bounds(), Color(0, 0, 0));





	// center view
	MyView v1(Rect(0,0,180,40), font, View::CENTER);
	v1.AddLine(TEXT("CENTER"));
	v1.AddLine(TEXT("keeps centered"));

	// streach view
	MyView v2(Rect(200,380,400,80), font, View::KEEP_PROPORTIONS);
	v2.AddLine(TEXT("KEEP_PROPORTIONS"));
	v2.AddLine(TEXT("is rescaled"));

	// follow left right view
	MyView v3(Rect(10,10,win.Width() - 20,40), font, View::FOLLOW_LEFT + View::FOLLOW_RIGHT);
	v3.AddLine(TEXT("FOLLOW_LEFT + FOLLOW_RIGHT"));
	v3.AddLine(TEXT("follows both left & right sides"));

	// follow top bottom view
	MyView v4(Rect(10,60,110,win.Height() - 70), font, View::FOLLOW_TOP + View::FOLLOW_BOTTOM);
	v4.AddLine(TEXT("FOLLOW_TOP +"));
	v4.AddLine(TEXT("FOLLOW_BOTTOM"));

	// add thw views
	win.AddChild(&clear);
	win.AddChild(&v1);
	win.AddChild(&v2);
	win.AddChild(&v3);
	win.AddChild(&v4);

	// show and run the window
	win.Show();
	win.Run();
	return 0;
}

