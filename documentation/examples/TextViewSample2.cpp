
#include "Dwarf.h"

using namespace dwarf;

class MainView : public TextView
{
public:
	MainView(Window *win, Rect frame, Font &font, uint32 resize_mode = 0) :
		TextView(frame, font, 100, resize_mode),
		m_time(0.0f), m_win(win), m_curLine(TEXT(""))
	{
		TextView::Align al = LEFT;
		if (m_win->HasInputServer())
			m_win->GetInputServer()->Add(this);
		AddLine(TEXT("DwarfEngine"));
		AddLine(TEXT("Command console:"));
		AddLine(TEXT("> "), al, Color(0.5,1,0.5));
		AddLine(TEXT(""), al, Color(0.3,0.8,0.3));
	}

	virtual ~MainView()
	{
		if (m_win->HasInputServer())
			m_win->GetInputServer()->Remove(this);
	}

	virtual void Tick(float frame_time)
	{
		if (m_time > 0)
		{
			m_time -= frame_time * 5;
			if (m_time < 0)
			{
				SetLine(3, TEXT(""));
			}
		}
	}

	void KeyChar(dchar character, uint32 scancode, uint32 modifiers)
	{
		if (scancode == InputReceiver::SC_BACKSPACE)
		{
			SetLine(2, TEXT("> ") + m_curLine.TrimRight(1));
		}

		else if (character >= 32)
		{
			m_curLine << character;
			SetLine(2, TEXT("> ") + m_curLine);
		}
	}

	void KeyDown(dchar character, uint32 scancode, uint32 modifiers)
	{
		switch (scancode)
		{
			case InputReceiver::SC_ENTER:
			{
				SetLine(2, TEXT("> "));
				SetLine(3, TEXT("Unknown command: \"") + m_curLine + TEXT("\""));
				m_time = 4;
				m_curLine.Empty();
			}
			break;

			case InputReceiver::SC_ESCAPE:
			{
				m_win->Close();
			}
			break;
		}
	}


private:
	float m_time;
	Window* m_win;
	String m_curLine;
};

int32 DwarfMain(Array<String> &commandline)
{
	Directory dir(TEXT("../data/fonts"));

	Window win(Rect(20, 20, 640, 480));
	InputServer input(&win);

	ClearView clear(win.Bounds(), Color(0, 0, 0));

	File file(&dir, TEXT("Arial, Bold Italic.font"));
	Font font(file);
	MainView text(&win, win.Bounds(), font, View::KEEP_PROPORTIONS);

	win.AddChild(&clear);
	win.AddChild(&text);

	win.Show();
	win.Run();
	return 0;
}

