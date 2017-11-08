#ifndef D_FRACTALTREEVIEW_H_
#define D_FRACTALTREEVIEW_H_

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

namespace dwarf
{

		class FractalTreeView : public View
		{
		public:
		
			FractalTreeView(Rect frame);
			virtual ~FractalTreeView();
			
			virtual void Tick(float frame_time);
			virtual void Draw(RenderingDevice &device) const;

			void draw_tree(int32 depth, RenderingDevice &device) const;
		private:

			float m_time;
		
		};
		
} // dwarf

#endif // D_FRACTALTREEVIEW_H_
