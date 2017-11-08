/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "ConsoleView.h"
#include "String.h"
#include "Color.h"
#include "TextOutStream.h"


using namespace dwarf;

ConsoleView::ConsoleView(Rect frame, float text_size, const Color &text_color, int32 buffer_size, uint32 resize_mode):
	TextView(frame, buffer_size, resize_mode),
	m_line(),
	m_cursor_position(0),
	m_text_size(text_size),
	m_text_color(text_color)
{
	// add a dummy font so we can change it later on, in SetFont();
	AddFont(Font());
}

/*!
	\brief virtual destructor
*/
ConsoleView::~ConsoleView()
{
}

TextOutStream &ConsoleView::operator<<(const dchar *string)
{
	int32 line_start = 0;
	m_line.Insert(m_cursor_position, string);

	for(int32 counter = m_cursor_position; counter < m_line.Length(); counter++)
	{
		dchar character = m_line[counter];
		if (character == CHAR('\n'))
		{
			AddLine(m_line.SubString(line_start, counter - line_start), TextView::LEFT, m_text_color, m_text_size, 0, true);
			line_start = counter + 1;
			m_cursor_position = 0;
			counter = 0;
			m_line = m_line.RightString(m_line.Length() - line_start);
		}
	}

	// update the cursor position
	m_cursor_position = m_line.Length();

	return *this;
}

void ConsoleView::PreDraw(RenderingDevice &device) const
{
	State state;
	state.Enable(State::BLEND);
	state.Disable(State::TEXTURE_1D + State::TEXTURE_2D + State::DEPTH_TEST);
	state.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
	state.SetColor(Color(0, 0, .5, .5));

	device.SetStates(state);

	device.Begin(RenderingDevice::QUADS);

	device.AddVertex(Point(0, 0));
	device.AddVertex(Point(Bounds().Width(), 0));
	device.AddVertex(Point(Bounds().Width(), Bounds().Height()));
	device.AddVertex(Point(0, Bounds().Height()));

	device.End();
}
