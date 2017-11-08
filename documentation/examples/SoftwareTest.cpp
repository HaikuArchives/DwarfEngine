#include "Dwarf.h"

#include "ConsoleView.h"

using namespace dwarf;

class TestView : public View
{
public:
	TestView(const Rect &frame):
		View(frame, View::FOLLOW_ALL_SIDES, View::SPACE_3D),
		m_time(0)
	{}

	virtual void Tick(float frame_time)
	{
		m_time += frame_time;
		m_rotation = Quaternion(m_time, Vertex<3>(2, 6, 1));
	}

	virtual void Draw(RenderingDevice &device) const
	{
		device.LoadIdentity();
		device.Translate(Vertex<3>(0,0,40));
		device.Rotate(m_rotation);
		device.SetColor(Color(1,1,1));
		device.Begin(RenderingDevice::TRIANGLES);

		device.AddVertex(Vertex<3>(1, -10,0));
		device.AddVertex(Vertex<3>(-15, 20, 0));
		device.AddVertex(Vertex<3>(15, 20, 0));

		device.End();
	}

private:

	float m_time;
	Quaternion m_rotation;

};


int32 DwarfMain(Array<String> &commandline)
{
	Directory dir;
	dir.OpenFileSystem(TEXT("../data"));
	
	Texture::SetFileSystem(&dir);
	
	Window win(Rect(20, 20, 320, 240), TEXT("Window title"));

	ClearView clear(win.Bounds(), Color(0, 0, 0));
	TestView view(win.Bounds());

	win.AddChild(&clear);
	win.AddChild(&view);

	win.Show();
	win.Run();

	return 0;
}
