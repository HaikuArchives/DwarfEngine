/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "TextView.h"
#include "Log.h"
#include "List.h"
#include "Rect.h"
#include "WindowBase.h"
#include "RenderingDevice.h"
#include "Color.h"
#include "Timer.h"
#include "Quaternion.h"


#include "String.h"


using namespace dwarf;

/*!
	\brief initializes the view with the given frame
	\param frame the position and size of the view,
	the position is always given in window-space coordinate system
	(not in parent view coordinate system as one could have suspected)
	\param font a File that refers to a font. This font will be loaded and used
	in the textbuffer.
	\param num_lines the number of lines that should be visible in the view
	\param buffer_size the number of lines in the buffer. If you scroll down
	to the end of the buffer, it will loop (and show the first lines again)
	\warning the frame must at least be one pixel high (other wise we get a
	division by zero)
*/
TextView::TextView(Rect frame, int32 buffer_size, uint32 resize_mode):
	View(frame, resize_mode),
	m_font_list(10),
	m_line(buffer_size),
	m_cursor_position(0),
	m_current_position(0.0f),
	m_top_line(0),
	m_loop_at(buffer_size)
{
	float pos = 0;
	for (int32 i = 0; i < m_loop_at; i++)
		pos = m_line[i].position = pos - m_line[i].size;
}

TextView::TextView(Rect frame, const Font &font, int32 buffer_size, uint32 resize_mode):
	View(frame, resize_mode, View::SPACE_2D_ABSOLUTE),
	m_font_list(10),
	m_line(buffer_size),
	m_cursor_position(0),
	m_current_position(0.0f),
	m_top_line(0),
	m_loop_at(buffer_size)
{
	AddFont(font);
	float pos = 0;
	for (int32 i = 0; i < m_loop_at; i++)
		pos = m_line[i].position = pos - m_line[i].size;
}


/*!
	\brief virtual destructor
*/
TextView::~TextView()
{
}

/*!
	\brief adds a font to the text view, which enables it to use that font
	\param font the font to add
	\return an index number that refers to the font. Use this number as font-id when calling AddLine()
	\sa AddLine()
*/
int32 TextView::AddFont(const Font &font)
{
	return m_font_list.AddItem(font);
}


/*!
	\brief adds a line at the bottom of the text buffer
	\param string the text line you want to add
	\param align selects the align of the line. You can choose LEFT, RIGHT and CENTER.
	\param color the color of the line
	\param text_size the size of the text
	\param font the font-id that was returned from AddFont() or 0 to use the default font (added when the TextView was constructed)
	\param scroll if true: the display will scroll to make sure that the newly added
	line becomes visible, otherwise the line may be added outside the visible area
	\return the index to the line added
	\sa AddFont()
*/
int32 TextView::AddLine(const String &string, Align align, const Color &color, float text_size, int32 font, bool scroll)
{
	D_ASSERT_MESSAGE(m_cursor_position < m_loop_at, TEXT("You are trying to add more lines than the buffer size in you TextView."));

	int32 ret = m_cursor_position;
	Line &line = m_line[m_cursor_position];
	line.text = string;
	line.align = align;
	line.color = color;
	line.size = text_size;
	line.font = font;
	if (m_cursor_position == 0) line.position = -text_size;
	else line.position = m_line[m_cursor_position - 1].position - text_size;

	line.descent = m_font_list[font].DescentRatio() * text_size;

	// If the font for this line was bigger than before, we have to
	// push the lines below this one down, to adjust this.
	float pos = line.position;
	for (int32 i = m_cursor_position+1; i < m_loop_at; i++)
		pos = m_line[i].position = pos - m_line[i].size;

	// increase the cursor position one step, loop if we reached the end of the buffer
	m_cursor_position++;
	m_cursor_position %= m_loop_at;
	
/*	
	if (scroll)
	{
		if (m_cursor_position < m_current_position
			|| m_cursor_position > (int32)(m_current_position + m_line_count) % m_loop_at)
		{
			m_current_position = fmod(m_loop_at + m_cursor_position - (int32)m_line_count, m_loop_at);
		}
	}
*/
	return ret;
}

void TextView::ScrollBy(float delta)
{
	if (delta == 0) return;

	float old = m_current_position;

	m_current_position += delta;

	m_current_position = fmod(m_current_position, m_line[m_loop_at-1].position);
	if (m_current_position < 0) m_current_position -= m_line[m_loop_at-1].position;

//	printf("current_position: %f\n", m_current_position);

	if (m_current_position - old < 0 && delta > 0)
	{
		int32 i = 0;
		while (-m_current_position < m_line[i].position) i++;
		m_top_line = i;
	}
	else if (m_current_position - old > 0 && delta < 0)
	{
		int32 i = m_loop_at - 1;
		while (-m_current_position > m_line[i].position) i--;
		m_top_line = i + 1;
	}
	else if (m_current_position < 0)
	{
		m_current_position = 0;
		m_top_line = 0;
	}
	else if (delta < 0)
	{
		int32 i = m_top_line;
		while (-m_current_position > m_line[i].position)
		{
			i--;
			if (i < 0) break;
		}
		m_top_line = i + 1;
	}
	else // if delta > 0
	{
		int32 i = m_top_line;
		while (-m_current_position < m_line[i].position)
		{
			i++;
			if (i >= m_loop_at) i -= m_loop_at;
		}
		m_top_line = i;
	}
//	printf("m_top_line: %d\n", m_top_line);
}

void TextView::ScrollTo(float pos)
{
	m_current_position = pos;
	m_current_position = fmodf(m_current_position, m_line[m_loop_at-1].position);
	if (m_current_position < 0) m_current_position -= m_line[m_loop_at-1].position;

	int32 i = 0;
	while(i < m_line.Length() && -m_current_position < m_line[i].position) i++;
	if (i >= m_loop_at) i -= m_loop_at;
	m_top_line = i;
}

void TextView::SetLine(int32 index, float size)
{
	D_ASSERT(index < m_line.Length());
	
	float delta = size - m_line[index].size;
	m_line[index].size = size;
	
	for (int i = index; i < m_line.Length(); i++)
		m_line[i].position += delta;
}

void TextView::SetLine(int32 index, int32 font_index)
{
	D_ASSERT(index < m_line.Length());
	D_ASSERT_MESSAGE(font_index < m_font_list.ItemCount(), TEXT("You are trying to use a font that isn't added to the font list."));
	
	m_line[index].font = font_index;
}

void TextView::SetLine(int32 index, const String &line)
{
	D_ASSERT(index < m_line.Length());

	m_line[index].text = line;
}

void TextView::SetLine(int32 index, Align align)
{
	D_ASSERT (index < m_line.Length());

	m_line[index].align = align;
}

void TextView::SetLine(int32 index, const Color &color)
{
	D_ASSERT (index < m_line.Length());

	m_line[index].color = color;
}

void TextView::Draw(RenderingDevice &device) const
{
	device.LoadIdentity();
	PreDraw(device);

	// draw the lines of text
	int32 i = m_top_line;
	float pos = m_line[i].position + m_line[i].size + m_current_position + Bounds().Height();

//	printf("pos: %f\nm_top_line: %ld\n", pos, m_top_line);

	while(true)
	{
		Line &line = m_line[i % m_loop_at];
		pos -= line.size;

		// when we reach the bottom of the view, stop
		if (pos + line.size < 0) break;

/*
		// TEMP!
		if (i == m_top_line)
		{
			char buffer[1024];
			line.text.ToChar(buffer, 1024);
			puts(buffer);
		}
*/
		if (!line.text.IsEmpty())
		{
			Font &font = m_font_list[line.font];
			font.DrawStates().SetColor(line.color);
			switch(line.align)
			{
				case LEFT:
				{
 					font.DrawString(device, Point(0, pos + line.descent), line.text, line.size);
				} break;
				case RIGHT:
				{
					float string_length = font.StringWidth(line.text) * line.size;
					font.DrawString(device, Point(Bounds().Width() - string_length, pos + line.descent), line.text, line.size);
				} break;
				case CENTER:
				{
					float string_length = font.StringWidth(line.text) * line.size;
					font.DrawString(device, Point((Bounds().Width() / 2.0f) - (string_length / 2.0f), pos + line.descent), line.text, line.size);
				} break;
			}
		}
		i++;
	}
}
