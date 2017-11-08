#include "Dwarf.h"
#include "TCBSpline.h"

using namespace dwarf;


// the points
const Point points[] =
{
	Point(.2f,.1f),
	Point(.2f,.2f),
	Point(.3f,.2f),
	Point(.3f,.3f),
	Point(.4f,.3f),
	Point(.5f,.5f),
	Point(.4f,.6f),
	Point(.1f,.6f),
	Point(.3f,.9f),
	Point(.7f,.5f),
	Point(.6f,.3f),
	Point(.2f,.1f)
};

const int32 num_points = 12;






class MyView : public View
{
public:
	MyView(const Rect &frame):
		View(frame, View::KEEP_PROPORTIONS, View::SPACE_2D_RELATIVE),
		m_step(0.0f)
	{
		// add the points to the spline
		for (int32 i = 0; i < num_points; i++)
			m_spline.add_key(points[i], 0.0f,0.0f,0.0f, i);

		// and calculate it
		m_spline.calculate_tcb();

		// initialize our state object
		m_state.Enable(State::LINE_SMOOTH + State::BLEND);
		m_state.Disable(State::DEPTH_TEST);
		m_state.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
		m_state.SetColor(Color(.5,0,0));
	}

	virtual void Tick(float frame_time)
	{
		// increase time counter
		m_step += frame_time;

		// we wait a few seconds before we reset it
		if(m_step > 12.5f) m_step = 0.0f;
	}

	virtual void Draw(RenderingDevice &device) const
	{
		// set up the renderer for som lines smoothing
		device.SetStates(m_state);
		
		
		// first draw the linear interpolation between the points
		device.Begin(RenderingDevice::LINE_STRIP);
			for (int32 i = 0; i < num_points; i++) device.AddVertex(points[i]);
		device.End();


		// change color when we draw the spline
		device.SetColor(Color(1,1,1));

		device.Begin(RenderingDevice::LINE_STRIP);
		for (TCBSpline<Point>::const_iterator i = m_spline.begin(); i.time() < m_step; i.step(0.025))
			device.AddVertex(i.value());
		device.End();
		
	}
	
private:

	
	TCBSpline<Point> m_spline;
	float m_step;
	State m_state;


};

int32 DwarfMain(Array<String> &commandline)
{
	Window win(Rect(20, 20, 640, 480), TEXT("Window title"));
	
	// clear the window with the color black
	ClearView clear(win.Bounds(), Color(0, 0, 0));

	// draws the actual spline
	MyView view(win.Bounds());

	// add the views to the window, remember that
	// the order in which you add views matters!
	win.AddChild(&clear);
	win.AddChild(&view);
	
	win.Show();
	win.Run();
	return 0;
}
