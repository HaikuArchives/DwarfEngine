#ifndef D_DRAWNUMBERVIEW_H_
#define D_DRAWNUMBERVIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "View.h"
#include "Texture.h"
#include "Color.h"
#include "Vertex.h"
#include "String.h"

namespace dwarf
{

		/*!
			\brief A simple view designed to draw integers on screen.
		*/
		class DrawNumberView : public View
		{
		public:


			DrawNumberView(Rect frame, HTexture letters, Point lettersize, uint32 resize_mode = 0);
			
			virtual ~DrawNumberView();



			virtual void Draw(RenderingDevice& device) const;

			void  SetNumber(int32 n) { m_number.FromInt(n); }
			int32 GetNumber() { return m_number.ToInt(); }

			void SetColor(Color const& color) { m_color = color; }
			Color GetColor() { return m_color; }

			void SetLetterSize(Point const& lsize) { m_lettersize = lsize; }
			Point GetLetterSize() { return m_lettersize; }

			void SetTexture(HTexture new_tex) { m_letters = new_tex; }
			HTexture GetTexture() { return m_letters; }

		private:
			HTexture	m_letters;
			Color		m_color;
			Point		m_lettersize;
			Point		m_scale;
			String		m_number;
		};

}


#endif