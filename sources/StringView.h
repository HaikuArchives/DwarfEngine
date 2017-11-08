#ifndef D_STRING_VIEW_H_
#define D_STRING_VIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "View.h"
#include "Font.h"
#include "String.h"
#include "Color.h"
#include "State.h"

namespace dwarf
{

	class StringView : public View
	{
	public:
		StringView(Rect frame, const Font &font, float size, const Color &color = Color(1,1,1,1), uint32 resize_mode = View::FOLLOW_TOP + View::FOLLOW_LEFT):
			View(frame, resize_mode, View::SPACE_2D_ABSOLUTE),
			m_color(color),
			m_size(size)
		{
			SetFont(font);
		}
		
		StringView(Rect frame, float size, const Color &color = Color(1,1,1,1), uint32 resize_mode = View::FOLLOW_TOP + View::FOLLOW_LEFT):
			View(frame, resize_mode),
			m_color(color),
			m_size(size)
		{
		}

		void SetString(const String &string)
		{
			m_text = string;
			Rect frame = Frame();
			frame.width = m_font.StringWidth(m_text) * m_size;
			ResizeTo(frame);
		}

		virtual void SetFont(const Font &font)
		{
			m_font = font;
			Rect frame = Frame();
			frame.width = m_font.StringWidth(m_text) * m_size;

			// set the states of the new font
			State &states = m_font.DrawStates();
			states.Disable(State::DEPTH_TEST);
			states.SetColor(m_color);

			ResizeTo(frame);
		}

		virtual void MessageReceived(Message *message)
		{
/*
			switch(message->id)
			{
			case SET_TEXT:
			{
				String str;
				*message >> str;
				SetText(str);
			} break;
			default:
*/				View::MessageReceived(message);
//			}
		}

		virtual void Draw(RenderingDevice &device) const
		{
			D_ASSERT_MESSAGE(m_font.IsValid(), TEXT("You must call SetFont() (to select a font this StringView should use) before you add it to a window, or supply a font to the constructor."));
			device.LoadIdentity();
		
			State state;
			state.Disable(State::CULL_FACE);

			device.SetStates(state);

			m_font.DrawString(device, Point(0, Bounds().Height() - m_size), m_text, m_size);
		}

	private:
		Font m_font;
		String m_text;
		Color m_color;
		float m_size;
	};

} // dwarf

#endif // D_STRING_VIEW_H
