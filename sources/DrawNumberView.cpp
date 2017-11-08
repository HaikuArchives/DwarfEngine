/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "View.h"
#include "RenderingDevice.h"
#include "DrawNumberView.h"
#include "Color.h"
#include "Vertex.h"

using namespace dwarf;

DrawNumberView::DrawNumberView(Rect frame, HTexture letters, Point lettersize, uint32 resize_mode) : 
	View(frame, resize_mode, View::SPACE_2D_ABSOLUTE),
	m_letters(letters),
	m_color(Color(1, 1, 1)),
	m_lettersize(lettersize),
	m_scale(1.0/(float)letters->Width(), 1.0/(float)letters->Height()),
	m_number(TEXT("0"))
{
}

DrawNumberView::~DrawNumberView()
{
}

void DrawNumberView::Draw(RenderingDevice& device) const
{
	int32 i = 0;
	int32 x = 0;
	float orix = 0.0f;
	float ilsx = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
	
	device.EnableState(RenderingDevice::BLEND);
	device.EnableState(RenderingDevice::TEXTURE_2D);
	device.DisableState(RenderingDevice::DEPTH_TEST);

	device.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
	
	device.UseTexture(0, m_letters);
	
	device.SetColor(m_color);

	device.Translate(Vertex<3>(0,0,0));
	device.LoadIdentity();

	device.Begin(RenderingDevice::QUADS);
	while(m_number[i] >= CHAR('0') && m_number[i] <= CHAR('9'))
	{
		ilsx = i*m_lettersize.x();
		w = m_lettersize.x()*m_scale.x();
		h = m_lettersize.y()*m_scale.y();
		x = m_number[i] - CHAR('0');
		orix = w*(float)x;
		{
			device.TexCoord (Point(orix, 0));
			device.AddVertex(Point(ilsx, 0));
			device.TexCoord (Point(orix+w, 0));
			device.AddVertex(Point(ilsx + m_lettersize.x(), 0));
			device.TexCoord (Point(orix+w, h));
			device.AddVertex(Point(ilsx + m_lettersize.x(), m_lettersize.y()));
			device.TexCoord (Point(orix, h));
			device.AddVertex(Point(ilsx, m_lettersize.y()));
		}
		i++;
	}
	device.End();
}