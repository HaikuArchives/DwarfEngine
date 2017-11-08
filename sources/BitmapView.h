#ifndef D_BITMAPVIEW_H_
#define D_BITMAPVIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "RenderingDevice.h"
#include "View.h"
#include "Rect.h"

namespace dwarf
{

		/*!
			\brief a view that just displays a bitmap.
		*/
		
		class BitmapView : public View
		{
			public:
		
			BitmapView(Rect const& frame, HTexture texture, uint32 resize_mode = 0):
				View(frame, resize_mode, View::SPACE_2D_RELATIVE),
				m_texture(texture)
			{
			}

			virtual void Draw(RenderingDevice& device) const
			{
				device.LoadIdentity();
				device.SetColor(Color(1, 1, 1, 1));
				device.EnableState(RenderingDevice::TEXTURE_2D);

				device.UseTexture(0, m_texture);

				device.Begin(RenderingDevice::QUADS);

				device.TexCoord( Point(0, 0));
				device.AddVertex( Point(0, 0));
				device.TexCoord( Point(1, 0));
				device.AddVertex( Point(1, 0));
				device.TexCoord( Point(1, 1));
				device.AddVertex( Point(1, 1));
				device.TexCoord( Point(0, 1));
				device.AddVertex( Point(0, 1));

				device.End();
			}

			private:

			 HTexture m_texture;	// the texture of the bitmap to display
		};

} // dwarf

#endif // D_BITMAPVIEW_H_
