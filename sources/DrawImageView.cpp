/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "DrawImageView.h"
#include "Log.h"
#include "List.h"
#include "Rect.h"
#include "WindowBase.h"
#include "RenderingDevice.h"
#include "Color.h"
#include "Quaternion.h"
#include "String.h"

using namespace dwarf;

/*!
	\brief initializes the view with the given frame
	\param frame the position and size of the view,
	the position is always given in window-space coordinate system
	(not in parent view coordinate system as one could have suspected)
*/
DrawImageView::DrawImageView(Rect frame, HTexture texture, uint32 resize_mode):
	View(frame, resize_mode, View::SPACE_2D_CENTER),
	m_time(0.0),
	m_coords(0),
	m_pos(0),
	m_num_patches(0),
	m_texture(texture),
	m_blend_src(RenderingDevice::ZERO),
	m_blend_dst(RenderingDevice::ONE_MINUS_SRC_COLOR)
{
}

/*!
	\brief virtual destructor
*/
DrawImageView::~DrawImageView()
{
}

/*!
	\brief virtual hook-fuction inherited from MessageReceiver, will be called every frame
	\param frame_time the time (in seconds) since the method was last called
	\warning you should always call the inherited version of Tick(), in your own version of Tick(), it's not empty.
*/
void DrawImageView::Tick(float frame_time)
{
	m_time += frame_time;
	View::Tick(frame_time);
}

void DrawImageView::AddPatch(const Rect &coord, const Rect &pos)
{
	if (m_coords.Length() <= m_num_patches)
	{
		m_coords.SetLength(m_coords.Length() + 20);
		m_pos.SetLength(m_pos.Length() + 20);
	}

	m_coords[m_num_patches] = coord;
	m_pos[m_num_patches] = pos;
	m_num_patches++;
}

/*!
	You should implement this method to draw the look of your view
	\brief virtual hook-fuction that will be called evey time the view needs to be redrawn
	\param device the rendering device to draw to
	\warning if you want to redraw the view, don't call this method, call Invalidate().
	\sa Invalidate()
*/
void DrawImageView::Draw(RenderingDevice &device) const
{
	const float fade_time = .3f;

	device.DisableState(RenderingDevice::DEPTH_TEST);

//	device.EnableState(RenderingDevice::TEXTURE_2D);
//	device.DisableState(RenderingDevice::TEXTURE_2D);
	
	float a = 3.0f / (m_time *.05f + 1.0f);
//	logger(TEXT("a: %f"), a);
	
	device.LoadIdentity();
	device.Scale(Vertex<3>(a, -a, 1.0f));
	device.Rotate(Quaternion(m_time * .01 - .1));

	device.EnableState(RenderingDevice::TEXTURE_2D);
	device.EnableState(RenderingDevice::BLEND);

	device.SetBlendMode(m_blend_src, m_blend_dst);

	float t = 0; // time counter
	int32 i = m_num_patches - 1; // patch counter
	device.UseTexture(0, m_texture);

	while (t < m_time && i >= 0)
	{
		if (t + fade_time > m_time)
		{
			float l = (m_time - t)/fade_time;
			device.SetColor(Color(l, l, l));
		}
		else
		{
			device.SetColor(Color(1, 1, 1));
		}

		device.Begin(RenderingDevice::QUADS);
/*
			device.TexCoord(m_coords[i].LeftBottom());
			device.AddVertex(m_pos[i].LeftBottom());

			device.TexCoord(m_coords[i].RightBottom());
			device.AddVertex(m_pos[i].RightBottom());

			device.TexCoord(m_coords[i].RightTop());
			device.AddVertex(m_pos[i].RightTop());

			device.TexCoord(m_coords[i].LeftTop());
			device.AddVertex(m_pos[i].LeftTop());
*/

			device.TexCoord(Point(m_coords[i].LeftBottom().x(), 1.0f - m_coords[i].LeftBottom().y()));
			device.AddVertex(Point(m_pos[i].LeftBottom().x() - .5f, -m_pos[i].LeftBottom().y() + .5f));

			device.TexCoord(Point(m_coords[i].RightBottom().x(), 1.0f - m_coords[i].RightBottom().y()));
			device.AddVertex(Point(m_pos[i].RightBottom().x() - .5f, -m_pos[i].RightBottom().y() + .5f));

			device.TexCoord(Point(m_coords[i].RightTop().x(), 1.0f - m_coords[i].RightTop().y()));
			device.AddVertex(Point(m_pos[i].RightTop().x() - .5f, -m_pos[i].RightTop().y() + .5f));

			device.TexCoord(Point(m_coords[i].LeftTop().x(), 1.0f - m_coords[i].LeftTop().y()));
			device.AddVertex(Point(m_pos[i].LeftTop().x() - .5f, -m_pos[i].LeftTop().y() + .5f));

		device.End();

		t += fade_time;
		i--;
	}
}
