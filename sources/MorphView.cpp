/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "MorphView.h"
#include "Array.h"
#include "Rect.h"
#include "RenderingDevice.h"
#include "Color.h"
#include "Log.h"


using namespace dwarf;


MorphView::MorphView(const Rect &frame,
					HTexture from_texture,
					HTexture to_texture,
					const Array<Triangle> &mesh,
					const Array<Point> &from_points,
					const Array<Point> &to_points,
					float delay):
	View(frame, View::KEEP_PROPORTIONS, View::SPACE_2D_RELATIVE),
	m_current_time(0.0),
	m_delay(delay),
	m_factor(0.0f),
	m_mesh(mesh),
	m_points_from(from_points),
	m_points_to(to_points),
	m_points_current(from_points),
	m_from_texture(from_texture),
	m_to_texture(to_texture)
{
}

/*!
	\brief virtual destructor
*/
MorphView::~MorphView()
{
}

void MorphView::Tick(float frame_time)
{
	m_current_time += frame_time;
	const float fraction = m_current_time / m_delay;

	// interpolate the points

//	if (fraction <= 1.0f)
	{
		m_factor = sin(fraction * D_PI - D_PI/2) / 2.0f + .5;

		for (int32 i = 0; i < m_points_current.Length(); i++)
			m_points_current[i] = (m_points_from[i] * (1 - m_factor) + m_points_to[i] * m_factor);
	}
/*
	else
	{
		for (int32 i = 0; i < m_points_current.Length(); i++)
			m_points_current[i] = m_points_to[i];
	}
*/
	View::Tick(frame_time);
}

void MorphView::Draw(RenderingDevice &device) const
{
	device.LoadIdentity();
	device.EnableState(RenderingDevice::TEXTURE_2D);
	device.DisableState(RenderingDevice::DEPTH_TEST);
	device.SetColor(Color(1, 1, 1));

	if (m_factor <= 1.0f)
	{

		device.DisableState(RenderingDevice::BLEND);
		device.UseTexture(0, m_from_texture);

		DrawMesh(m_points_from, device);

		device.EnableState(RenderingDevice::BLEND);
		device.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
		device.SetColor(Color(1, 1, 1, m_factor));
		device.UseTexture(0, m_to_texture);

		DrawMesh(m_points_to, device);

	}
	else
	{
		device.DisableState(RenderingDevice::BLEND);
		device.UseTexture(0, m_to_texture);

		DrawMesh(m_points_to, device);
	}

}

void MorphView::DrawMesh(const Array<Point> &tex_coords, RenderingDevice &device) const
{
	device.Begin(RenderingDevice::TRIANGLES);

	for(int32 i = 0; i < m_mesh.Length(); i++)
	{
		Triangle &tri = m_mesh[i];
		device.TexCoord(tex_coords[tri.point[0]]);
		device.AddVertex(m_points_current[m_mesh[i].point[0]]);

		device.TexCoord(tex_coords[tri.point[1]]);
		device.AddVertex(m_points_current[m_mesh[i].point[1]]);

		device.TexCoord(tex_coords[tri.point[2]]);
		device.AddVertex(m_points_current[m_mesh[i].point[2]]);
	}

	device.End();
}