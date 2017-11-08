/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "TunnelView.h"
#include "Log.h"
#include "List.h"
#include "Rect.h"
#include "WindowBase.h"
#include "RenderingDevice.h"
#include "Color.h"
#include "Timer.h"
#include "Quaternion.h"

using namespace dwarf;

/*!
	\brief initializes the view with the given frame
	\param frame the position and size of the view,
	the position is always given in window-space coordinate system
	(not in parent view coordinate system as one could have suspected)
*/
TunnelView::TunnelView(Rect frame, uint32 resize_mode):
	View(frame, resize_mode, View::SPACE_3D),
	m_time(0.0),
	m_textime(0.0)
{
	SetClip(.2, 200);
}

/*!
	\brief virtual destructor
*/
TunnelView::~TunnelView()
{
}

/*!
	\brief virtual hook-fuction inherited from MessageReceiver, will be called every frame
	\param frame_time the time (in seconds) since the method was last called
	\warning you should always call the inherited version of Tick(), in your own version of Tick(), it's not empty.
*/
void TunnelView::Tick(float frame_time)
{
	m_time += frame_time;
	m_textime += frame_time;
	if(m_textime > 1.0f) m_textime  -= 1.0f;
	View::Tick(frame_time);
}

/*!
	You should implement this method to draw the look of your view
	\brief virtual hook-fuction that will be called evey time the view needs to be redrawn
	\param device the rendering device to draw to
	\warning if you want to redraw the view, don't call this method, call Invalidate().
	\sa Invalidate()
*/

void TunnelView::DrawTunnel(RenderingDevice &device, const float radius, const int32 segments, const int32 parts, float transparency) const
{
	float prev_u = 0.0f;
	for (int32 j = 1; j <= parts; j++)
	{
		const float u = (float)j/(float)parts;

		device.Begin(RenderingDevice::TRIANGLE_STRIP);

		for(int32 i = 0; i <= segments; i++)
		{
			const float t = (float)i/(float)segments;

			device.TexCoord(Point(prev_u * 4.0f, t));
			device.SetColor(Color(prev_u, prev_u, prev_u, transparency));
//			device.AddVertex(Vertex<3>(cosf(t * D_2PI)*radius, sinf(t * D_2PI)*radius + sinf(prev_u*D_2PI + m_time)*1.5f, (1.0 - prev_u) * -20.0f));
			device.AddVertex(Vertex<3>(cosf(t * D_2PI)*radius, sinf(t * D_2PI)*radius + sinf(prev_u*D_2PI + m_time)*2.5f, (1.0 - prev_u) * -20.0f));

			device.TexCoord(Point(u * 4.0f, t));
			device.SetColor(Color(u, u, u, transparency));
//			device.AddVertex(Vertex<3>(cosf(t * D_2PI)*radius, sinf(t * D_2PI)*radius + sinf(u*D_2PI + m_time)*1.5f, (1.0 - u) * -20.0f));
			device.AddVertex(Vertex<3>(cosf(t * D_2PI)*radius, sinf(t * D_2PI)*radius + sinf(u*D_2PI + m_time)*2.5f, (1.0 - u) * -20.0f));
		}

		device.End();
		prev_u = u;
	}
}

void TunnelView::Draw(RenderingDevice &device) const
{
//	device.SetClearColor(Color(0,0,0));
	device.DisableState(RenderingDevice::BLEND);
	device.EnableState(RenderingDevice::DEPTH_TEST);
	device.EnableState(RenderingDevice::TEXTURE_2D);

//	device.ClearColorBuffer();
	device.ClearDepthBuffer();
	device.UseTexture(0, m_texture);

	device.TranslateTexture(Vertex<3>(-m_textime, 0.0, 0.0));

	device.LoadIdentity();
	device.Rotate(Quaternion(m_time / 2.0f));

	DrawTunnel(device, 7.0f/* +  sinf(m_time*3.0)*/, 29, 20, 1.0f);

	device.UseTexture(0, m_transparent);

	device.EnableState(RenderingDevice::BLEND);
//	device.SetBlendMode(RenderingDevice::SRC_COLOR, RenderingDevice::ONE_MINUS_SRC_COLOR);
	device.SetBlendMode(RenderingDevice::ONE, RenderingDevice::ONE);
	device.ResetTexture();
	device.TranslateTexture(Vertex<3>(-m_textime, 0.0, 0.0f));

	DrawTunnel(device, 6.0f, 29, 20, 0.5f);

	device.ResetTexture();
	device.DisableState(RenderingDevice::BLEND);
}