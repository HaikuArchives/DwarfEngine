#ifndef D_DRAWIMAGEVIEW_H_
#define D_DRAWIMAGEVIEW_H_

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
#include "Array.h"
#include "FileSystem.h"
#include "Texture.h"
#include "Rect.h"
#include "String.h"
#include "RenderingDevice.h"

namespace dwarf
{


		/*!
			\brief This class has not been fully documented yet.
			\todo document DrawImageView.
		*/
		class DrawImageView : public View
		{
		public:
		
			DrawImageView(Rect frame, HTexture texture, uint32 resize_mode = 0);
			virtual ~DrawImageView();
			
			void SetBlendMode(RenderingDevice::Blendmode src, RenderingDevice::Blendmode dst);

			void AddPatch(const Rect &texture_coords, const Rect &patch_position);
			
			virtual void Tick(float frame_time);
			virtual void Draw(RenderingDevice &device) const;

		private:
			
			float m_time;
			Array<Rect> m_coords;
			Array<Rect> m_pos;
			int32 m_num_patches;
			HTexture m_texture;

			RenderingDevice::Blendmode m_blend_src;
			RenderingDevice::Blendmode m_blend_dst;
		};

		inline void DrawImageView::SetBlendMode(RenderingDevice::Blendmode src, RenderingDevice::Blendmode dst)
		{
			m_blend_src = src;
			m_blend_dst = dst;
		}

} // dwarf

#endif // D_DRAWIMAGEVIEW_H_
