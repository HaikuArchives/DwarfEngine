#ifndef D_CLEARVIEW_H_
#define D_CLEARVIEW_H_

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
#include "Color.h"
#include "RenderingDevice.h"



namespace dwarf
{

		/*!
			This view is mostly used in example programs to clear the frame buffers between
			frames. You have to add this view to your window before you add any other views. Because
			if any other views are added before this one, they will draw to the frame buffer before this view
			clears it. So, if you add another view to your window before you add this view, what they draw will be
			invisible (cleared). 
			\brief A view that clears the color- and depth buffer every time it's drawn.
		*/
		class ClearView : public  View
		{
		public:

			ClearView(Rect frame,  Color clear_color):
				 View(frame),
				m_color(clear_color)
			{}

			virtual ~ClearView() {}

			virtual void Draw(RenderingDevice &device) const
			{
				device.ClearColorBuffer();
				device.ClearDepthBuffer();
			}

			virtual void OnWindowAdd( RenderingDevice &device)
			{
				device.SetClearColor(m_color);
			}

		private:
			 Color m_color;
		};
		
} // dwarf

#endif // D_CLEARVIEW_H_
