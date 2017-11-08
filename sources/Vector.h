#ifndef D_VECTOR_H_
#define D_VECTOR_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Vertex.h"


namespace dwarf
{
		
		/*!
			\brief A Vector is identical to a Vertex<3> (except conceptually).
		*/
		typedef Vertex<3> Vector;
		
		/*!
			By it's methods it can be interpreted as both a Vertex<3> and a vector.
			\brief describes a point in 2D space
			\image html Vector2D.png
		*/
		
		class Vector2D
		{
			public:

			Vector2D();
			Vector2D(float f);
			Vector2D(const float f[2]);
			Vector2D(const Vector2D& v); // copy constructor
			Vector2D(float x_, float y_); // component constructor
			void SetTo(float x_, float y_ = 0.0);
			
			float Normalize(); // normalize Vector2D, return length
			int Dominant(); // dominant axis index
			float Angle(Vector2D &v);
		
			float Length2() const; // squared length
			float Length() const; // length of Vector2D (distance of point from origin)
		
			const Vector2D &operator = (const Vector2D &v); // assignment
			const Vector2D &operator = (const Point &p);
			const Vector2D &operator += (const Vector2D &v); // component add assignment
			const Vector2D &operator -= (const Vector2D &v); // component sub assignment
			const Vector2D &operator *= (const Vector2D &v); // component mul assignment
			const Vector2D &operator /= (const Vector2D &v); // component div assignment
			const Vector2D &operator *= (float scale); // scalar mul assignment
			const Vector2D &operator /= (float scale); // scalar div assignment
		
			operator float* () const; // cast to float*
		
			Vector2D operator - () const; // unary component negate
			Vector2D operator + (const Vector2D &v) const; // component add
			Vector2D operator + (const Point &v) const; // component add
			Vector2D operator - (const Vector2D &v) const; // component sub
			Vector2D operator * (const Vector2D &v) const; // component mul
			Vector2D operator / (const Vector2D &v) const; // component div
			Vector2D operator * (float scale) const; // scalar mul
			Vector2D operator / (float scale) const; // scalar div
		
			bool operator !() const; // epsilon degeneracy, near-origin point
			bool operator == (const Vector2D &v) const; // epsilon length equality
			bool operator != (const Vector2D &v) const; // epsilon length inequality
			bool operator <= (const Vector2D &v) const; // epsilon length less or equal
			bool operator >= (const Vector2D &v) const; // epsilon length greater or equal
			bool operator < (const Vector2D &v) const; // epsilon length less than
			bool operator > (const Vector2D &v) const; // epsilon length greater than

			union
			{
				float x;
				float u;
			};	//! < the x (or u) component in the point
			
			union
			{
				float y;
				float v;
			}; //! < the y (or v) component in the point
		};



		// Inline definitions

		inline Vector2D::Vector2D():
			x(0),
			y(0)
		{
		}

		inline Vector2D::Vector2D(const Vector2D &v):
			x(v.x),
			y(v.y)
		{
		}	

		inline Vector2D::Vector2D(float x_, float y_):
			x(x_),
			y(y_)
		{
		}

		/*!
			\brief initializes all three components to the same value
			\param f the calue to initialize x, y and z to
		*/
		inline Vector2D::Vector2D(float f):
			x(f),
			y(f)
		{
		}

		/*!
			\brief copy the components values from the array
			\param f the array to read the x, y and z values from (the array must contain
			at least three floats)
		*/
		inline Vector2D::Vector2D(const float f[2]):
			x(f[0]),
			y(f[1])
		{
		}

		inline const Vector2D &Vector2D::operator = (const Vector2D &v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		inline const Vector2D &Vector2D::operator = (const Point &p)
		{
			x = p.x();
			y = p.y();
			return *this;
		}



		inline Vector2D Vector2D::operator - () const
		{
			return Vector2D(-x, -y);
		}

		inline Vector2D Vector2D::operator + (const Vector2D &v) const
		{
			return Vector2D(x + v.x, y + v.y);
		}

		inline Vector2D Vector2D::operator + (const Point &v) const
		{
			return Vector2D(x + v.x(), y + v.y());
		}

		inline Vector2D Vector2D::operator - (const Vector2D &v) const
		{
			return Vector2D(x - v.x, y - v.y);
		}

		inline Vector2D Vector2D::operator * (const Vector2D &v) const
		{
			return Vector2D(x * v.x, y * v.y);
		}

		inline Vector2D Vector2D::operator / (const Vector2D &v) const
		{
			return Vector2D(x / v.x, y / v.y);
		}

		inline Vector2D Vector2D::operator * (float scale) const
		{
			return Vector2D(x * scale, y * scale);
		}

		/*!
			This method is faster than Length(), because no square root is being
			calculated
			\brief calculates the squared length of the vector
			\return the length of the vector multiplied with itself
			\sa Length()
		*/
		inline float Vector2D::Length2() const
		{
			return x*x + y*y;
		}

		/*!
			\brief calculates the length of the vector
			\return the vector's length
			\warning a square root is calculated, you should'n call this method too
			often
			\sa Length2()
		*/
		inline float Vector2D::Length() const
		{
			return sqrt(x*x + y*y);
		}

		inline bool Vector2D::operator ! () const
		{
			return Length2() <= D_EPSILON2;
		}

		inline Vector2D::operator float* () const
		{
			return reinterpret_cast<float*>(const_cast<Vector2D*>(this));
		}

		inline bool Vector2D::operator == (const Vector2D &v) const
		{
			return (*this - v).Length2() <= D_EPSILON2;
		}

		inline bool Vector2D::operator != (const Vector2D &v) const
		{
			return (*this - v).Length2() > D_EPSILON2;
		}

		inline bool Vector2D::operator <= (const Vector2D &v) const
		{
			return Length2() <= v.Length2() + D_EPSILON2;
		}

		inline bool Vector2D::operator >= (const Vector2D &v) const
		{
			return Length2() >= v.Length2() - D_EPSILON2;
		}

		inline bool Vector2D::operator < (const Vector2D &v) const
		{
			return Length2() < v.Length2() - D_EPSILON2;
		}

		inline bool Vector2D::operator > (const Vector2D &v) const
		{
			return Length2() > v.Length2() + D_EPSILON2;
		}

		inline void Vector2D::SetTo(float x_, float y_)
		{
			x = x_;
			y = y_;
		}


}


#endif // D_VECTOR_H_
