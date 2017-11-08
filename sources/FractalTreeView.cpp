/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "FractalTreeView.h"
#include "Log.h"
#include "List.h"
#include "Rect.h"
#include "WindowBase.h"
#include "RenderingDevice.h"
#include "Color.h"
#include "Quaternion.h"
#include "Bitmap.h"

using namespace dwarf;

/*!
	\brief initializes the view with the given frame
	\param frame the position and size of the view,
	the position is always given in window-space coordinate system
	(not in parent view coordinate system as one could have suspected)
*/
FractalTreeView::FractalTreeView(Rect frame):
	View(frame, View::FOLLOW_TOP_LEFT, View::SPACE_3D),
	m_time(0.0)
{
	SetFov(D_PI / 2.0f);
}

/*!
	\brief virtual destructor
*/
FractalTreeView::~FractalTreeView()
{
}

/*!
	\brief virtual hook-fuction inherited from MessageReceiver, will be called every frame
	\param frame_time the time (in seconds) since the method was last called
	\warning you should always call the inherited version of Tick(), in your own version of Tick(), it's not empty.
*/
void FractalTreeView::Tick(float frame_time)
{
	m_time += frame_time;
	View::Tick(frame_time);
}

void FractalTreeView::Draw(RenderingDevice &device) const
{
	device.DisableState(RenderingDevice::DEPTH_TEST);

	device.LoadIdentity();
	device.DisableState(RenderingDevice::TEXTURE_2D);

	device.EnableState(RenderingDevice::BLEND);
	device.EnableState(RenderingDevice::LINE_SMOOTH);
	device.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);

	device.Translate(Vertex<3>(0, -3, -10));
	device.Rotate(Quaternion(m_time, Vertex<3>(0, 1, 0)));

	draw_tree(4, device);
	device.LoadIdentity();
	device.DisableState(RenderingDevice::BLEND);
}

void FractalTreeView::draw_tree(int32 depth, RenderingDevice &device) const
{
	device.Begin(RenderingDevice::LINES);

		device.AddVertex(Point(0, 0));
		device.AddVertex(Point(0, 1));

	device.End();

	if (depth == 0) return;

	device.PushMatrix();
		device.Translate(Vertex<3>(0, 0.97476363, 0));
		device.Rotate(Quaternion(.4, Vertex<3>(-.7, .4, .3)));
		device.Scale(Vertex<3>(.7, .7, 1));
		draw_tree(depth - 1, device);
	device.PopMatrix();

	device.PushMatrix();
		device.Translate(Vertex<3>(0, 0.913243, 0));
		device.Rotate(Quaternion(.5, Vertex<3>(.2, .4, .2)));
		device.Scale(Vertex<3>(.75, .75, 1));
		draw_tree(depth - 1, device);
	device.PopMatrix();
	
	device.PushMatrix();
		device.Translate(Vertex<3>(0, 0.98896754, 0));
		device.Rotate(Quaternion(-.3, Vertex<3>(.6, .7, .7)));
		device.Scale(Vertex<3>(.8, .8, 1));
		draw_tree(depth - 1, device);
	device.PopMatrix();
	/*
	device.PushMatrix();
		device.Translate(Vertex<3>(0, 1, 0));
		device.Rotate(Quaternion(.5, Vertex<3>(.5, .8, -.5)));
		device.Scale(Vertex<3>(.8, .8, 1));
		draw_tree(depth - 1, device);
	device.PopMatrix();
	*/
}
