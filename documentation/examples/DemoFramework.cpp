#include "Dwarf.h"

using namespace dwarf;


enum messages
{
	ADD_TUNNEL = 1000,
	SET_TEXT,
	MOVE_SET_TEXT_AND_SHOW
};


class MyStringView : public View
{
public:
	MyStringView(Rect frame, float size, const Color &color = Color(1,1,1,1), const Point &move_direction = Point(0,0), uint32 resize_mode = View::FOLLOW_TOP + View::FOLLOW_LEFT):
	  View(frame, resize_mode, View::SPACE_2D_RELATIVE),
		m_direction(move_direction),
		m_color(color),
		m_size(size)
	{
	}

	void SetText(const String &string)
	{
		m_text = string;
		Rect frame = Bounds();
		frame.width = m_font.StringWidth(m_text) * m_size;
		ResizeTo(frame);
	}

	virtual void SetFont(const Font &font)
	{
		m_font = font;
		Rect frame = Bounds();
		frame.width = m_font.StringWidth(m_text) * m_size;
		ResizeTo(frame);
	}

	virtual void MessageReceived(Message *message)
	{
		switch(message->id)
		{
		case MOVE_SET_TEXT_AND_SHOW:
		{
			String str;
			(InStream&)*message >> (Serializable&)str;
			SetText(str);

			Rect frame = Bounds();
			frame.x = ((float)rand()/(float)RAND_MAX) * (ParentFrame().Width() - Bounds().Width());
			frame.y = ((float)rand()/(float)RAND_MAX) * (ParentFrame().Height() - Bounds().Height());
			ResizeTo(frame);
			Show(true);
		} break;
		case SET_TEXT:
		{
			String str;
			(InStream&)*message >> str;
			SetText(str);
		} break;
		default:
			View::MessageReceived(message);
		}
	}

	virtual void Show(bool show)
	{
		if (show) m_timer.Start();
		View::Show(show);
	}

	virtual void Tick(float frame_time)
	{
		if (m_timer.Current() > 3.0) Show(false);

		ResizeTo(Bounds() + m_direction * frame_time);
		m_color.r = m_color.g = m_color.b = 1.0 - (m_timer.Current() / 3.0f);
		View::Tick(frame_time);
	}

	virtual void Draw(RenderingDevice &device) const
	{
		D_ASSERT_MESSAGE(m_font.IsValid(), TEXT("You must call SetFont() (to select a font this MyStringView should use) before you add it to a window."));

		device.LoadIdentity();
//		device.Set2DMode(Bounds().Width(), Bounds().Height());

		device.LoadIdentity();
		device.DisableState(RenderingDevice::DEPTH_TEST);

		m_font.Begin(device);
		device.SetColor(m_color);

//		device.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
		
		m_font.DrawString(device, Point(0,m_font.DescentRatio() * m_size), m_text, m_size);
		m_font.End();
	}

private:
	Font m_font;
	String m_text;
	Point m_direction;
	Color m_color;
	Timer m_timer;
	float m_size;
};



class DemoTunnel : public TunnelView
{
public:
	DemoTunnel(Rect frame, uint32 resize_mode):
		TunnelView(frame, resize_mode)
	{}

	virtual void OnWindowAdd(RenderingDevice &device)
	{
		// load the textures
		Directory dir(TEXT("../data"));
		SetTexture(HTexture(&dir, TEXT("tex1.jpg")));
		SetTransparentTexture(HTexture(&dir, TEXT("tex2.jpg")));

		TunnelView::OnWindowAdd(device);
	}

	virtual void OnWindowRemove()
	{
		// unload the textures
		SetTexture(HTexture());
		SetTransparentTexture(HTexture());
	}
private:
	float m_time;
	Font m_font;
	Point m_direction;
};












class DemoWindow : public Window
{
public:
	struct Notification
	{
		float time;					// the time when the event will occur
		MessageReceiver *target;	// the target that will be notified of the event
		Message message;			// the message that will be sent to the target view
	};


	DemoWindow(const Rect &frame, FileSystem &directory, const String &filename):
		Window(frame, TEXT("Tjoho")),
		m_sound(&directory, filename),
		m_clear(Bounds(), Color(0,0,0)),
		m_tunnel(Bounds(), View::FOLLOW_ALL_SIDES),
		m_textpatch1(Rect(10, 300, 100, 100), 100),
		m_textpatch2(Rect(10, 300, 100, 100), 100),
		m_textpatch3(Rect(10, 300, 100, 100), 100),
		m_textpatch4(Rect(10, 300, 100, 100), 100),
		m_textpatch5(Rect(10, 300, 100, 100), 100),
		m_textpatch6(Rect(10, 300, 100, 100), 100),
		m_textpatch7(Rect(10, 300, 100, 100), 100),
		m_textpatch8(Rect(10, 300, 100, 100), 100),
		m_console(Rect(0,0,Bounds().Width(), Bounds().Height() / 3), 10, Color(1,1,1), 100, View::FOLLOW_LEFT + View::FOLLOW_RIGHT + View::FOLLOW_BOTTOM)
	{
		Directory dir(TEXT("../data"));
		File font_file(&dir, TEXT("fonts/Verdana, Bold Italic.font"));
		Font font(font_file);

		m_console.SetFont(font);

		m_textpatch1.SetFont(font);
		m_textpatch2.SetFont(font);
		m_textpatch3.SetFont(font);
		m_textpatch4.SetFont(font);
		m_textpatch5.SetFont(font);
		m_textpatch6.SetFont(font);
		m_textpatch7.SetFont(font);
		m_textpatch8.SetFont(font);

		AddChild(&m_clear);

		// add the tunnel to the window
		AddChild(&m_tunnel);

		AddChild(&m_console);

		AddChild(&m_textpatch1);
		AddChild(&m_textpatch2);
		AddChild(&m_textpatch3);
		AddChild(&m_textpatch4);
		AddChild(&m_textpatch5);
		AddChild(&m_textpatch6);
		AddChild(&m_textpatch7);
		AddChild(&m_textpatch8);

		m_textpatch1.Show(false);
		m_textpatch2.Show(false);
		m_textpatch3.Show(false);
		m_textpatch4.Show(false);
		m_textpatch5.Show(false);
		m_textpatch6.Show(false);
		m_textpatch7.Show(false);
		m_textpatch8.Show(false);

		Message msg(MOVE_SET_TEXT_AND_SHOW);

		msg.Empty();
		msg << String(TEXT("as"));
		AddNotification(41, &m_textpatch1, msg);

		msg.Empty();
		msg << String(TEXT("the"));
		AddNotification(41.5, &m_textpatch2, msg);

		msg.Empty();
		msg << String(TEXT("iron"));
		AddNotification(41.75, &m_textpatch3, msg);

		msg.Empty();
		msg << String(TEXT("turns"));
		AddNotification(42.8, &m_textpatch4, msg);

		msg.Empty();
		msg << String(TEXT("to"));
		AddNotification(43.33, &m_textpatch5, msg);

		msg.Empty();
		msg << String(TEXT("rust"));
		AddNotification(43.85, &m_textpatch6, msg);

		msg.Empty();
		msg << String(TEXT("like"));
		AddNotification(45, &m_textpatch7, msg);

		msg.Empty();
		msg << String(TEXT("the"));
		AddNotification(45.5, &m_textpatch8, msg);

/*
		AddNotification(5.5, tunnel, D_SHOW);
		AddNotification(7, tunnel, D_HIDE);
		AddNotification(7.5, tunnel, D_SHOW);
		AddNotification(9, tunnel, D_HIDE);
		AddNotification(9.5, tunnel, D_SHOW);
		AddNotification(11, tunnel, D_HIDE);
		AddNotification(11.5, tunnel, D_SHOW);
		AddNotification(13, tunnel, D_HIDE);
		AddNotification(13.5, tunnel, D_SHOW);
		AddNotification(15, tunnel, D_HIDE);
		AddNotification(15.5, tunnel, D_SHOW);
		AddNotification(17, tunnel, D_HIDE);
		AddNotification(17.5, tunnel, D_SHOW);
		AddNotification(19, tunnel, D_HIDE);
		AddNotification(19.5, tunnel, D_SHOW);
		AddNotification(21, tunnel, D_HIDE);
		AddNotification(21.5, tunnel, D_SHOW);
		AddNotification(23, tunnel, D_HIDE);
		AddNotification(23.5, tunnel, D_SHOW);
*/

		// play the music
		m_player = m_sound.Play();
	}

	virtual ~DemoWindow()
	{
	}

	virtual void KeyDown(dchar character, uint32, uint32)
	{
		if (character == CHAR(' '))
		{
			String str;
			str.FromFloat(m_player.Time());
			m_console << str << TEXT("\n");
		}
	}
/*
	virtual void MessageReceived(Message *message)
	{
		switch(message->id)
		{
			
		}
	}
*/
	virtual void Tick(float frame_time)
	{
		// this presumes that the notification_list is sorted
		if (!m_player.IsPlaying()) Close();
		if (m_notification_list.Size() > 0
			&& m_player.Time() >= m_notification_list.First().time)
		{
			Notification &not = m_notification_list.First();
			not.target->PostMessage(new Message(not.message));

			// remove the first element
			m_notification_list.Erase(m_notification_list.Begin());
		}
		Window::Tick(frame_time);
	}

	void AddNotification(float time, MessageReceiver *target, const Message &message)
	{
		Notification n;
		n.time = time;
		n.target = target;
		n.message = message;
		m_notification_list.PushBack(n);
		// binary search inserting
	}

protected:

	SoundPlayer m_player;
	SoundFile m_sound;

private:

	List<Notification> m_notification_list;

	HTexture m_outer;
	HTexture m_inner;

	ClearView m_clear;
	DemoTunnel m_tunnel;

	MyStringView m_textpatch1;
	MyStringView m_textpatch2;
	MyStringView m_textpatch3;
	MyStringView m_textpatch4;
	MyStringView m_textpatch5;
	MyStringView m_textpatch6;
	MyStringView m_textpatch7;
	MyStringView m_textpatch8;

	ConsoleView m_console;
};


int32 DwarfMain(Array<String> &commandline)
{
	Directory dir(TEXT("../data"));
	DemoWindow win(Rect(10, 10, 640, 480), dir, TEXT("Music.mp3"));
	InputServer input(&win);
	input.Add(&win);

	win.Show();
	win.Run();
	return 0;
}
