#ifndef D_BOX_H_
#define D_BOX_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Log.h"
#include "Vertex.h"

namespace dwarf
{

		/*!
			describes the rectangle by storing it's bottom-left point and it's
			width and height
			\brief represents an axis-aligned box
			\image html Box.gif
		*/
		
		class Box
		{
			public:


			Box();
			Box(Box const& rect);
			
			Box(Vertex<3> const& origin, float width, float height, float length);
			Box(const Vertex<3> &min, const Vertex<3> &max);
			
			~Box();
		
			void PrintToLog() const;
		
			void SetTo(const Vertex<3> &origin, float width,float height, float depth);
		
			Vertex<3> Midpoint() const;
		
		// transformation
			void InsetBy(Vertex<3> const& delta);
		
			void OffsetBy(const Vertex<3> &delta);
			
			Box operator+(const Vertex<3> &delta);	
			Box operator-(const Vertex<3> &delta);	
			const Box &OffsetTo(const Vertex<3> &pos);
		
			void Scale(const Box &source, const Box &destination);
		
		// expression transformations
			Box &InsetBySelf(const Vertex<3> &delta);
			Box InsetByCopy(const Vertex<3> &delta);
			Box &OffsetBySelf(const Vertex<3> &delta);
			Box OffsetByCopy(const Vertex<3> &delta);
			Box &OffsetToSelf(const Vertex<3> &delta);
			Box OffsetToCopy(const Vertex<3> &delta);
		
		// comparison
			bool operator==(const Box &rect) const;
			bool operator!=(const Box &rect) const;
		
		// intersection and union
			Box operator&(const Box &rect) const;
			Box operator|(const Box &rect) const;
		
		// utilities
			bool Intersects(const Box &r) const;
		
			float Length() const;
			float Width() const;
			float Height() const;
		
			bool Contains(const Vertex<3> &point) const;
			bool Contains(const Box &rect) const;
		
			Vertex<3> origin; // the corner with the smallest coordinates of the box
			float width,height,depth;


		};

		// inline definitions
		
		inline Box::Box():
			origin(0.0, 0.0, 0.0),
			width(0.0),
			height(0.0),
			depth(0.0)
		{
		}
		
		inline Box::Box(const Box &box):
			origin(box.origin),
			width(box.width),
			height(box.height),
			depth(box.depth)
		{
		}
		
		/*!
			\brief initializes the box
			\param x the x position of the left left edge
			\param y the y position of the bottom edge
			\param width the width of the rectangle
			\param height the height of the rectangle
		*/
		inline Box::Box(const Vertex<3> &origin_,float width_,float height_,float depth_):
			origin(origin_),
			width(width_),
			height(height_),
			depth(depth_)
		{
		}
		
		inline Box::Box(const Vertex<3> &min, const Vertex<3> &max):
			width(max.x() - min.x()),
			height(max.y() - min.y()),
			depth(max.z() - min.z())
		{
			origin = min;
		}

		/*!
			\brief sets the rectangle
			\param x the x position of the left edge
			\param y the y position of the bottom edge
			\param width the width of the rectangle
			\param height the height of the rectangle
		*/
		inline void Box::SetTo(const Vertex<3> &origin_,float width_,float height_,float depth_)
		{
			origin = origin_;
			width = width_;
			height = height_;
			depth = depth_;
		}
		
		inline Box::~Box()
		{
		}
		
		/*!
			\brief prints this rectangles position and size to the log stream
		*/
		inline void Box::PrintToLog() const
		{
			logger(TEXT("Box (%f, %f, %f)(%f, %f, %f)"),origin.x(), origin.y(), origin.z(), width,height,depth);
		}
		
		/*!
			\brief calculates the midpoint of the box
			\return the midpoint of the box
		*/
		inline Vertex<3> Box::Midpoint() const
		{
			return Vertex<3>(width * .5f, height * .5f, depth * .5f) + origin;
		}
		/*
		inline void Box::OffsetBy(float dx,float dy)
		{
			x += dx;
			y += dy;
		}
		
		inline Box Box::operator+(const uPoint &point)
		{
			return Box(x + point.x, y + point.y, width, height);
		}
		
		inline Box Box::operator-(const uPoint &point)
		{
			return Box(x - point.x, y - point.y, width, height);
		}
		
		inline const Box &Box::OffsetTo(float x_,float y_)
		{
			x = x_;
			y = y_;
			return *this;
		}
		
		inline void Box::Scale(const Box &source, const Box &destination)
		{
		//	logger(L"Result of Scale:");
			if (source.width)
			{
				x *= destination.width / source.width + (destination.x - source.x);
				width = width * destination.width / source.width;
			}
			else
			{
				x += destination.x - source.x;
				width = 0;
			}
		
			if (source.height)
			{
				y *= destination.height / source.height + (destination.y - source.y);
				height = height * destination.height / source.height;
			}
			else
			{
				y += destination.y - source.y;
				height = 0;
			}
		//	PrintToLog();
		}
		*/

} // dwarf

#endif // D_BOX_H_

