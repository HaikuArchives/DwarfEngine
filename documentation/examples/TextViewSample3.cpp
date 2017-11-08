/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Dwarf.h"

using namespace dwarf;

class CreditScroller : public TextView
{
public:
	CreditScroller(Rect frame, const Font &font, const Font &font2) :
		TextView(frame, font, 24, View::KEEP_PROPORTIONS),
		m_time(0.0f)
	{
		AddFont(font2);

		AddLine(TEXT("Example"), TextView::CENTER, Color(1,1,1), 40, 1);
		AddLine(TEXT("An example of how to use"), TextView::LEFT, Color(1,1,1), 15);
		AddLine(TEXT("The TextView class as a"), TextView::LEFT, Color(1,1,1), 15);
		AddLine(TEXT("credit scroller."), TextView::LEFT, Color(1,1,1), 15);
		AddLine(TEXT("You can of course align"), TextView::LEFT, Color(1,1,1), 15);
		AddLine(TEXT("the text differently."), TextView::LEFT, Color(1,1,1), 15);
		AddLine(TEXT(""));
		AddLine(TEXT("Left"), TextView::LEFT);
		AddLine(TEXT("Right"), TextView::RIGHT);
		AddLine(TEXT("Center"), TextView::CENTER);
		AddLine(TEXT(""));
		AddLine(TEXT("You can also have different"), TextView::CENTER);
		AddLine(TEXT("colors of the text."), TextView::CENTER);
		AddLine(TEXT(""));
		AddLine(TEXT("Red"), TextView::CENTER, Color(1.0f, .3f, .3f));
		AddLine(TEXT("Green"), TextView::CENTER, Color(.3f, 1.0f, .3f));
		AddLine(TEXT("Blue"), TextView::CENTER, Color(.3f, .3f, 1.0f));
	}

	virtual void Tick(float frame_time)
	{
//		ScrollBy(frame_time * 50);
		ScrollTo(m_time * 50);
		m_time += frame_time;
		TextView::Tick(frame_time);
	}
private:
	float m_time;
};

int32 DwarfMain(Array<String> &commandline)
{
	Directory dir(TEXT("../data/fonts"));
	Window win(Rect(20, 20, 320, 240), TEXT("dwarf")/*, Window::UNRESIZABLE*/);
	InputServer input(&win);

	ClearView clear(win.Bounds(), Color(0, 0, 0));

	File file(&dir, TEXT("Verdana, Bold Italic.font"));
	Font font(file);

	File file2(&dir, TEXT("CommercialScript, Regular.font"));
	Font font2(file2);

	CreditScroller text(win.Bounds(), font, font2);

	win.AddChild(&clear);
	win.AddChild(&text);

	win.Show();
	win.Run();
	return 0;
}
 
/*
#include "Dwarf.h"
#include "DrawFPS.h"
#include "NoiseView.h"
#include "TunnelView.h"

using namespace dwarf;


class KeyHandler : public InputReceiver
{
public:
	KeyHandler(dwarf::Window &win):
		m_win(win)
	{
	}
	~KeyHandler()
	{
	}

	void KeyDown(dchar, uint32 key, uint32)
	{
		switch (key)
		{
		case D_ESCAPE:
			m_win.Close();
			break;

		default:
			break;
		}
	}

private:

	Window &m_win;
};

int32 DwarfMain(StringArray &commandline)
{
	using namespace dwarf;

	Directory directory;
	directory.OpenFileSystem(TEXT("data"));
//	PakFile directory(TEXT("data.zip"));

	Window win(Rect(30, 40, 400, 300), TEXT("dwarf"));
	InputServer m_input;
	win.SetInputServer(&m_input);

	// the KeyHandler will be removed before the InputServer, this is a potential bug!
	// make the InputReceiver disconnect itself when destroyed,
	KeyHandler kb(win);
	m_input.Add(&kb);

	Texture outer(Bitmap(&directory, TEXT("tex1.jpg")), win);
	Texture inner(Bitmap(&directory, TEXT("tex3.jpg")), win);
	Texture font(Bitmap(&directory, TEXT("text2.tga")), win);

	ClearView clr(win.Bounds(), Color(0,0,0));

	TunnelView tunnel(
		win.Bounds(),
		outer,
		inner,
		View::KEEP_PROPORTIONS);

	DrawFPS fps(
		Rect(win.Width() - 20,5,20,20),
		font,
		Point(9, 16),
		View::FOLLOW_RIGHT + View::FOLLOW_BOTTOM);

	fps.SetColor(Color(1,0,0));

	win.AddChild(&clr);
	win.AddChild(&tunnel);
	win.AddChild(&fps);

	win.Show();
	win.Run();

//	m_input.Remove(&kb);

	return 0;
}
*/