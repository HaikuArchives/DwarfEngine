#include "Dwarf.h"

using namespace dwarf;

class MyView : public View
{
public:
	MyView(const Rect &frame):
		View(frame, View::KEEP_PROPORTIONS, View::SPACE_2D_ABSOLUTE)
	{
		Directory dir(TEXT("../data/fonts"));
		m_font.Load(&dir, TEXT("Verdana, Regular.font"));

		m_font.DrawStates().SetColor(Color(1, 0, .6));
	}

	virtual void Draw(RenderingDevice &device) const
	{
		m_font.DrawString(device, Point(40, 70), TEXT("Testing to draw a string\nwith linefeeds"), 20);
	}

private:
	
	Font m_font;
//	Spline<float> m_spline;
};


int32 DwarfMain(Array<String> &commandline)
{
	Directory dir;
	Window win(Rect(20, 20, 640, 480), TEXT("Window title"));
	
	// clear the window with the color black
	ClearView clear(win.Bounds(), Color(0, 0, 0));
	MyView view(win.Bounds());
	
	win.AddChild(&clear);
	win.AddChild(&view);
	
	win.Show();
	win.Run();
	return 0;
}
