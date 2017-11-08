/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "NoiseView.h"
#include "Log.h"
#include "List.h"
#include "Rect.h"
#include "WindowBase.h"
#include "Window.h"
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
NoiseView::NoiseView(Rect frame, uint32 resize_mode):
	View(frame, resize_mode, View::SPACE_2D_RELATIVE),
	m_color(1,1,1,0.25),
	m_time(0.0),
	m_scale(1.0),
	m_texture(),
	m_counter(0)
{
}

void NoiseView::OnWindowAdd(RenderingDevice &device)
{
	Bitmap tex(128, 128, Bitmap::GRAYSCALE);

	// create the noise texture
	for (int32 i = 0; i < tex.DataSize(); i++)
		tex.Data()[i] = rand();

	m_texture = HTexture(tex);
}

/*!
	\brief virtual hook-fuction inherited from MessageReceiver, will be called every frame
	\param frame_time the time (in seconds) since the method was last called
	\warning you should always call the inherited version of Tick(), in your own version of Tick(), it's not empty.
*/
void NoiseView::Tick(float frame_time)
{
	m_time += frame_time;
	View::Tick(frame_time);

	m_counter++;

	if (Win())
	{
		m_scale = ((Win()->Width() / 400.0) + (Win()->Height()/300.0))/2.0f;
	}
}

void NoiseView::Draw(RenderingDevice &device) const
{
	static const Point tex_coord[] =
	{
		Point(1, 1),
		Point(-1, 1),
		Point(-1, -1),
		Point(1, -1)
	};

	device.DisableState(RenderingDevice::DEPTH_TEST);

	device.LoadIdentity();
	device.EnableState(RenderingDevice::TEXTURE_2D);

	device.UseTexture(0, m_texture);
	device.TranslateTexture(Vertex<3>(m_time, m_time, -m_time));
	device.RotateTexture(Quaternion(m_time));

	device.EnableState(RenderingDevice::BLEND);
	device.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
	device.SetColor(m_color);


	device.Begin(RenderingDevice::QUADS);

		device.TexCoord(tex_coord[m_counter&3] + tex_coord[(m_counter>>2)&3] * m_scale);
		device.AddVertex(Point(0.0, 0.0));
		device.TexCoord(tex_coord[(m_counter + 1)&3] + tex_coord[(m_counter>>2)&3] * m_scale);
		device.AddVertex(Point(1.0, 0.0));
		device.TexCoord(tex_coord[(m_counter + 2)&3] + tex_coord[(m_counter>>2)&3] * m_scale);
		device.AddVertex(Point(1.0, 1.0));
		device.TexCoord(tex_coord[(m_counter + 3)&3] + tex_coord[(m_counter>>2)&3] * m_scale);
		device.AddVertex(Point(0.0, 1.0));

	device.End();

	device.DisableState(RenderingDevice::BLEND);
	device.ResetTexture();
}