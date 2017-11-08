#ifndef D_DRAWFPS_H_
#define D_DRAWFPS_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "DrawNumberView.h"


namespace dwarf
{
		/*!
			\brief A simple view which simply uses the DrawNumberView to display the current FPS.
		 */
		class DrawFPS : public DrawNumberView
		{
		public:
			DrawFPS(Rect frame, HTexture letters, Point lettersize = Point(8, 8), uint32 resize_mode = 0) : 
				DrawNumberView(frame, letters, lettersize, resize_mode)
			{}

			virtual void Tick(float frame_time)
			{
				SetNumber(int(1.0f/frame_time));
			}
		};

}

#endif // D_DRAWFPS_H