#ifndef D_TUNNELVIEW_H_
#define D_TUNNELVIEW_H_

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

		class TunnelView : public View
		{
		public:
		
			TunnelView(Rect frame, uint32 resize_mode = 0);
			virtual ~TunnelView();

			void AddTime(float time) {m_time += time;}
			void SetTexture(HTexture tex) { m_texture = tex; }
			void SetTransparentTexture(HTexture tex) { m_transparent = tex; }

			virtual void Tick(float frame_time);
			virtual void Draw(RenderingDevice &device) const;

		private:
			
			void DrawTunnel(RenderingDevice &device, const float radius, const int32 segments, const int32 parts, float transparency) const;

			float m_time;
			float m_textime;
			HTexture m_texture;
			HTexture m_transparent;
		};
		

} // dwarf

#endif // D_TUNNELVIEW_H_
