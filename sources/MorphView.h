#ifndef D_MORPHVIEW_H_
#define D_MORPHVIEW_H_

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

namespace dwarf
{


		/*!
			\brief This class has not been fully documented yet.
			\todo document MorphView.
		*/
		class MorphView : public View
		{
		public:
		
			struct Triangle
			{
				// the indices of the points in the point list
				int32 point[3];
			};

			MorphView(const Rect &frame,
					HTexture from_texture,
					HTexture to_texture,
					const Array<Triangle> &mesh,
					const Array<Point> &from_points,
					const Array<Point> &to_points,
					float delay);
			virtual ~MorphView();

			virtual void Tick(float frame_time);
			virtual void Draw(RenderingDevice &device) const;

		private:

			void DrawMesh(const Array<Point> &tex_coords, RenderingDevice &device) const;
			
			float m_current_time;
			const float m_delay; // the times it takes to morph
			float m_factor; // time between [0, 1]

			const Array<Triangle> m_mesh;

			const Array<Point> m_points_from;
			const Array<Point> m_points_to;
			Array<Point> m_points_current;

			HTexture m_from_texture;
			HTexture m_to_texture;
		};
		
} // dwarf

#endif // D_MORPHVIEW_H_
