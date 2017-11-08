#ifndef D_TEXTVIEW_H_
#define D_TEXTVIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "View.h"
#include "Rect.h"
#include "Vertex.h"
#include "Texture.h"
#include "String.h"
#include "ListArray.h"
#include "File.h"
#include "Font.h"
#include "State.h"

namespace dwarf
{

		/*!
			You have to supply a font (in which the text is displayed). It can align the
			text to the left, right and centered. You can also select different colors for
			each line of text (a single line can only be drawn with one color).
			\brief a view that displays text
		*/
		class TextView : public View
		{
		public:
		
			enum Align
			{
				LEFT = 0,
				RIGHT,
				CENTER
			};

			TextView(Rect frame, const Font &font, int32 buffer_size = 256, uint32 resize_mode = 0);
			TextView(Rect frame, int32 buffer_size = 256, uint32 resize_mode = 0);
			virtual ~TextView();

			virtual void PreDraw(RenderingDevice &device) const {}
			virtual void Draw(RenderingDevice &device) const;

			int32 AddFont(const Font &font);
			void SetFont(int32 index, const Font &font) { m_font_list[index] = font; }
			int32 AddLine(const String &string, Align align = LEFT, const Color &color = Color(1.0f, 1.0f, 1.0f), float text_size = 15, int32 font = 0, bool scroll = false);
			
			void SetLine(int32 index, float size);
			void SetLine(int32 index, int32 font_index);
			void SetLine(int32 index, const String &line);
			void SetLine(int32 index, Align align);
			void SetLine(int32 index, const dwarf::Color &color);

			int32 BufferSize() { return m_loop_at + 1; }
			int32 CurrentPosition() {return m_cursor_position; }
			String &operator[](int32 index) { return m_line[index].text; }

			void ScrollBy(float delta);
			void ScrollTo(float pos);

		private:

			struct Line
			{
				Line():
					text(),
					align(TextView::LEFT),
					color(1,1,1),
					size(15),
					position(0),
					descent(0)
				{}
				String text; // the text
				Align align; // LEFT, RIGHT or CENTER
				Color color; // the text color
				float size;	// the font-size
				int32 font; // the font for this line (given as an index into the m_font_list ListArray)
				float position; // the position this line is located on. (0 is on the top and line below is given as a negative number)
				float descent; // the descent of the font (with the current size)
			};
			
			ListArray<Font> m_font_list; // the list of fonts used in the text view
			Array<Line> m_line; // the lines
			int32 m_cursor_position; // the position where new lines are added

			float m_current_position; // the current scroll position
			int32 m_top_line; // the line number to the line visible at the top of the view
			int32 m_loop_at; // the number of lines available, before the text buffer loops
		};
		

} // dwarf

#endif // D_TEXTVIEW_H_
