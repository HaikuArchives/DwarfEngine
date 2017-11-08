#ifndef D_CONSOLEVIEW_H_
#define D_CONSOLEVIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "TextView.h"
#include "Rect.h"
#include "Vertex.h"
#include "Texture.h"
#include "String.h"
#include "ListArray.h"
#include "File.h"
#include "Font.h"
#include "TextOutStream.h"

namespace dwarf
{
		/*!
			\todo document this class
		*/

		class ConsoleView : public TextView, public TextOutStream
		{
		public:
		
			ConsoleView(Rect frame, float text_size = 15.0f, const Color &text_color = Color(1,1,1), int32 buffer_size = 256, uint32 resize_mode = 0);
			virtual ~ConsoleView();
			
			void SetFont(const Font &font) { TextView::SetFont(0, font); }

//			virtual void ResizeTo(const Rect &frame);
			virtual void PreDraw(RenderingDevice &device) const;

			virtual TextOutStream &operator<<(const dchar *string);

		private:
			String m_line; // the current unfinished line
			int32 m_cursor_position; // on the current line
			float m_text_size;
			Color m_text_color;
		};
		

} // dwarf

#endif // D_CONSOLEVIEW_H_
