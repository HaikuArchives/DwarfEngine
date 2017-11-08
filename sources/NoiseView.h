#ifndef D_NOISEVIEW_H_
#define D_NOISEVIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "View.h"
#include "Rect.h"
#include "Color.h"
#include "Vertex.h"
#include "Texture.h"

namespace dwarf
{

		class NoiseView : public View
		{
		public:

			NoiseView(Rect frame, uint32 resize_mode = 0);
			
			virtual void Tick(float frame_time);
			virtual void Draw(RenderingDevice &device) const;
			
			virtual void OnWindowAdd(RenderingDevice &device);

			void SetNoiseColor(const Color& clr);

		private:

			Color m_color;
			float m_time;
			float m_scale;
			HTexture m_texture;
			int32 m_counter;
		
		};

		// inlines

		inline void NoiseView::SetNoiseColor(const Color& clr)
		{
			m_color = clr;
		}
		
} // dwarf

#endif // D_NOISEVIEW_H_
