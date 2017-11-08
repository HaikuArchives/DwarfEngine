#ifndef D_RECT_H_
#define D_RECT_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Vertex.h"
#include "Log.h"
#include "Serializable.h"


namespace dwarf
{

		/*!
			describes the rectangle by storing it's bottom-left point and it's
			width and height
			\brief represents a rectangle
			\image html Rect.gif
		*/
		
		class Rect : public Serializable
		{
			public:
		
			Rect();
			Rect(const Rect &rect);
			Rect(float x,float y,float width,float height);
			Rect(const Point &point,float width,float height);
		
			float Left() const;
			float Top() const;
			float Right() const;
			float Bottom() const;
		
			void PrintToLog() const;
		
			Point LeftTop() const;
			Point RightBottom() const;
			Point LeftBottom() const;
			Point RightTop() const;
		
			Point Midpoint() const;
		
			/*!
				\brief sets the left-top corner
				\param point the new left-top corner
			*/
			void SetLeftTop(const Point &point);
		
			/*!
				\brief sets the right-bottom corner
				\param point the new right bottom corner
			*/
			void SetRightBottom(const Point &point);
			
			/*!
				\brief sets the left-bottom corner
				\param point the new left-bottom corner
			*/
			void SetLeftBottom(const Point &point);
			
			/*!
				\brief sets the right-top corner
				\param point the new right-top corner
			*/
			void SetRightTop(const Point &point);
		
			void SetTo(float x,float y,float width,float height);
		
			Rect operator+(const Point &point) const;
			Rect operator-(const Point &point) const;
			
			void MoveBy(const Point &point);
			const Rect &MoveTo(const Point &point);
		
			const Rect &ScaleBy(float x, float y = 1.0f);

			const Rect &Scale(const Rect &source, const Rect &destination);
			const Rect &InsetBy(const Point &point);
			const Rect &InsetBy(float dx, float dy);

		// comparison
			bool operator==(const Rect &rect) const;
			bool operator!=(const Rect &rect) const;
		
		// intersection and union
			Rect operator&(const Rect &rect) const;
			Rect operator|(const Rect &rect) const;
		
		// utilities
			bool Intersects(const Rect &r) const;
		
			float Width() const;
			int32 IntegerWidth() const;
			float Height() const;
			int32 IntegerHeight() const;
		
			bool Contains(const Point &point) const;
			bool Contains(const Rect &rect) const;
		
			float x, y, width, height;

			virtual void Restore(InStream &buffer);
			virtual void Serialize(OutStream &buffer) const;
		
		};
		
		
		
		inline Rect::Rect():
			x(0.0),
			y(0.0),
			width(0.0),
			height(0.0)
		{
		}
		
		/*!
			\brief initializes the rectangle
			\param x the x position of the left left edge
			\param y the y position of the bottom edge
			\param width the width of the rectangle
			\param height the height of the rectangle
		*/
		inline Rect::Rect(float x_,float y_,float width_,float height_):
			x(x_),
			y(y_),
			width(width_),
			height(height_)
		{
		}


		inline Rect::Rect(const Point &left_bottom,float width_,float height_):
			x(left_bottom.x()),
			y(left_bottom.y()),
			width(width_),
			height(height_)
		{
		}
		


		inline Rect::Rect(const Rect &rect):
			x(rect.x),
			y(rect.y),
			width(rect.width),
			height(rect.height)
		{
		}
		
		/*!
			\brief sets the rectangle
			\param x the x position of the left edge
			\param y the y position of the bottom edge
			\param width the width of the rectangle
			\param height the height of the rectangle
		*/
		inline void Rect::SetTo(float x_,float y_,float width_,float height_)
		{
			x = x_;
			y = y_;
			width = width_;
			height = height_;
		}
		
		/*!
			\return the left-bottom corner
		*/
		inline Point Rect::LeftBottom() const
		{
			return Point(x,y);
		}
		
		/*!
			\return the left-top corner
		*/
		inline Point Rect::LeftTop() const
		{
			return Point(x,y + height);
		}
		
		/*!
			\return the right-bottom corner
		*/
		inline Point Rect::RightBottom() const
		{
			return Point(x + width,y);
		}
		
		/*!
			\return the right-top corner
		*/
		inline Point Rect::RightTop() const
		{
			return Point(x + width,y + height);
		}
		
		/*!
			\brief prints this rectangles position and size to the log stream
		*/
		inline void Rect::PrintToLog() const
		{
			logger(TEXT("Rect (%f,%f)(%f,%f)"),x,y,width,height);
		}
		
		/*!
			\brief tells you the x corrdinate for the left edge
			\return the left edge's x coordinate
		*/
		inline float Rect::Left() const
		{
			return x;
		}
		
		/*!
			\brief tells you the y coordinate for the top edge
			\return the top edge's y coordinate
		*/
		inline float Rect::Top() const
		{
			return y + height;
		}
		
		/*!
			\brief tells you the x coordinate for the right edge
			\return the right edge's x coordinate
		*/
		inline float Rect::Right() const
		{
			return x + width;
		}
		
		/*!
			\brief tells you the y coordinate for the bottom edge
			\return the bottom edge's y coordinate
		*/
		inline float Rect::Bottom() const
		{
			return y;
		}
		
		inline float Rect::Width() const
		{
			return width;
		}
		
		inline float Rect::Height() const
		{
			return height;
		}
		
		/*!
			\brief calculates the midpoint of the rectangle
			\return the midpoint of the rectangle
		*/
		inline Point Rect::Midpoint() const
		{
			return Point(width / 2.0f + x, height / 2.0f + y);
		}
		
		/*!
			\brief moves the rectangle relative to it's current position
			\param point the point to add to the current position of the bottom-left corner
		*/
		inline Rect Rect::operator+(const Point &point) const
		{
			return Rect(x + point.x(), y + point.y(), width, height);
		}
		
		inline Rect Rect::operator-(const Point &point) const
		{
			return Rect(x - point.x(), y - point.y(), width, height);
		}
		
		/*!
			\brief moves the rectangle relative to it's current position
			\param point the point to add to the current position of the bottom-left corner
		*/
		inline void Rect::MoveBy(const Point &p)
		{
			x += p.x();
			y += p.y();
		}
		
		/*!
			\brief moves the rectangle to the given coordinate
			\param point the new position for the bottom-left corner
			\return a reference to itself
		*/
		inline const Rect &Rect::MoveTo(const Point &p)
		{
			x = p.x();
			y = p.y();
			return *this;
		}
		
		inline bool Rect::Contains(const Point &point) const
		{
			return (point.x() >= x && point.x() <= x + width && point.y() >= y && point.y() <= y + height);
		}
		
		inline const Rect &Rect::InsetBy(float dx, float dy)
		{
			x += dx;
			y += dy;
			width -= 2*dx;
			height -= 2*dy;
			return *this;
		}

		inline bool Rect::operator!=(const Rect &rect) const
		{
			return rect.x != x || rect.y != y || rect.width != width || rect.height != height;
		}

		inline bool Rect::operator==(const Rect &rect) const
		{
			return rect.x == x && rect.y == y && rect.width == width && rect.height == height;
		}

		inline const Rect &Rect::ScaleBy(float x, float y)
		{
			width*=x;
			height*=y;
			return *this;
		}

} // dwarf

#endif // D_RECT_H_
