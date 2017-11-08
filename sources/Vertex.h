#ifndef D_VERTEX_H_
#define D_VERTEX_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "Math.h"
#include "Log.h"

namespace dwarf
{
	class Matrix4x4;
	class Quaternion;
	
	/*!
		By it's methods it can be interpreted as both a vertex and a vector.
		\brief describes a point in 3D (or 2D) space
		\image html Vertex.png
	*/
	template<int N> class Vertex
	{
		public:

		Vertex();
		explicit Vertex(float f);
		explicit Vertex(const float f[N]);
		Vertex(const Vertex<N>& v); // copy constructor
		Vertex(float x, float y); // component constructor
		Vertex(float x, float y, float z); // component constructor

		void RotateBy(float angle);
		void RotateBy(const Quaternion &rotation);
		void RotateAround(const Vertex<3> &point, const Quaternion &rotation);

		float Normalize(); // normalize Vertex, return length
		int32 Dominant() const; // dominant axis index
		float Angle(Vertex &v) const;

		float Length2() const; // squared length
		float Length() const; // length of Vertex (distance of point from origin)

		float DotProduct (const Vertex &v) const;

		// only defined for Vertex<3>
		Vertex<3> CrossProduct (const Vertex<3> &v) const;
		void TriNormal(const Vertex<3> &v1, const Vertex<3> &v2, const Vertex<3> &v3);

		void ConstrainTo(const Rect &rect);
		Vertex<2> ConvertSpace(const Rect &from, const Rect &to) const;

		const Vertex<N> &operator = (const Vertex<N> &v); // assignment
		const Vertex<N> &operator += (const Vertex<N> &v); // component add assignment
		const Vertex<N> &operator -= (const Vertex<N> &v); // component sub assignment
		const Vertex<N> &operator *= (float scale); // scalar mul assignment
		const Vertex<N> &operator /= (float scale); // scalar div assignment

		operator const float* () const; // cast to float*

		Vertex<N> operator - () const; // unary component negate
		Vertex<N> operator + (const Vertex<N> &v) const; // component add
		Vertex<N> operator - (const Vertex<N> &v) const; // component sub
		Vertex<N> operator / (const Vertex<N> &v) const; // component div
		float operator * (const Vertex &v) const; // dot product
		Vertex<N> operator * (float scale) const; // scalar mul
		Vertex<N> operator / (float scale) const; // scalar div

		float &operator[](int32 index);

		bool operator !() const; // epsilon degeneracy, near-origin point
		bool operator == (const Vertex<N> &v) const; // epsilon length equality
		bool operator != (const Vertex<N> &v) const; // epsilon length inequality
		bool operator <= (const Vertex<N> &v) const; // epsilon length less or equal
		bool operator >= (const Vertex<N> &v) const; // epsilon length greater or equal
		bool operator < (const Vertex<N> &v) const; // epsilon length less than
		bool operator > (const Vertex<N> &v) const; // epsilon length greater than

		float x() const { return m_components[0]; }
		float y() const { return m_components[1]; }
		float z() const { return m_components[2]; }
		float u() const { return m_components[0]; }
		float v() const { return m_components[1]; }
		
		float &x() { return m_components[0]; }
		float &y() { return m_components[1]; }
		float &z() { return m_components[2]; }
		float &u() { return m_components[0]; }
		float &v() { return m_components[1]; }
		
	private:

		float m_components[N];
	};

	typedef Vertex<2> Point;
}

#include "Matrix4x4.h"


namespace dwarf
{	

	template<int N> inline Vertex<N>::Vertex()
	{
		for (int32 i = 0; i < N; i++) m_components[i] = 0.0f;
	}

	template<int N> inline Vertex<N>::Vertex(const Vertex<N> &v)
	{
		for (int32 i = 0; i < N; i++) m_components[i] = v[i];
	}

	template<int N> inline Vertex<N>::Vertex(float x, float y)
	{
		D_ASSERT_MESSAGE(N >= 2, TEXT("This constructor is only defined on vertices in 2 or more dimensions"));
		m_components[0] = x;
		m_components[1] = y;
		for (int32 i = 2; i < N; i++) m_components[i] = 0.0f;
	}
	
	template<int N> inline Vertex<N>::Vertex(float x, float y, float z)
	{
		D_ASSERT_MESSAGE(N >= 3, TEXT("This constructor is only defined on vertices in 3 or more dimensions"));
		m_components[0] = x;
		m_components[1] = y;
		m_components[2] = z;
		for (int32 i = 3; i < N; i++) m_components[i] = 0.0f;
	}

	/*!
		\brief initializes all components to the same value
		\param f the value to initialize all the components to
	*/
	template<int N> inline Vertex<N>::Vertex(float f)
	{
		for (int32 i = 3; i < N; i++) m_components[i] = f;
	}

	/*!
		\brief copy the components values from the array
		\param f the array to read the x, y and z values from (the array must contain
		at least three floats)
	*/
	template<int N> inline Vertex<N>::Vertex(const float f[N])
	{
		for (int32 i = 0; i < N; i++) m_components[i] = f[i];
	}

	template<int N> inline float &Vertex<N>::operator[](int32 index)
	{
		D_ASSERT_MESSAGE(index >= 0 && index < N, TEXT("Index out of bounds on vertex"));
		return m_components[index];
	}

	template<int N> inline Vertex<N> Vertex<N>::operator - () const
	{
		Vertex<N> v;
		for (int32 i = 0; i < N; i++) v[i] = -m_components[i];
		return v;
	}

	template<int N> inline Vertex<N> Vertex<N>::operator + (const Vertex<N> &v) const
	{
		Vertex<N> ret;
		for (int32 i = 0; i < N; i++) ret[i] = m_components[i] + v[i];
		return ret;
	}

	template<int N> inline Vertex<N> Vertex<N>::operator - (const Vertex<N> &v) const
	{
		Vertex<N> ret;
		for (int32 i = 0; i < N; i++) ret[i] = m_components[i] - v[i];
		return ret;
	}

	template<int N> inline float Vertex<N>::operator * (const Vertex<N> &v) const
	{
		return DotProduct(v);
	}

	template<int N> inline Vertex<N> Vertex<N>::operator * (float scale) const
	{
		Vertex<N> ret;
		for (int32 i = 0; i < N; i++) ret[i] = m_components[i] * scale;
		return ret;
	}

	/*!
		This method is faster than Length(), because no square root is being
		calculated
		\brief calculates the squared length of the vector
		\return the length of the vector multiplied with itself
		\sa Length()
	*/
	template<int N> inline float Vertex<N>::Length2() const
	{
		float length2 = 0.0f;
		for (int32 i = 0; i < N; i++) length2 += m_components[i] * m_components[i];
		return length2;
	}

	/*!
		\brief calculates the length of the vector
		\return the vector's length
		\warning a square root is calculated, you should'n call this method too
		often
		\sa Length2()
	*/
	template<int N> inline float Vertex<N>::Length() const
	{
		float length2 = 0.0f;
		for (int32 i = 0; i < N; i++) length2 += m_components[i] * m_components[i];
		return sqrt(length2);
	}

	/*!
		if the point is already within the rectangle, nothing happens. If
		the point is aoutside the rectangle it will be moved to a point
		within the rectangle that lies as near the origin point as possible
		\brief makes the point lie within the borders of the rectangle
		\param rect the rectangle to constrain the point to
	*/
	template<int N> void Vertex<N>::ConstrainTo(const Rect &rect)
	{
		D_ASSERT_MESSAGE(N == 2, TEXT("This method is only defined for vertices with 2 dimensions"));
		if (x() > (rect.x + rect.width) )
			x() -= ( x() - (rect.x + rect.width) );
		else
		if (x() < (rect.x) )
			x() += (x() - (rect.x) );
	
		if (y() > (rect.y + rect.height) )
			y() -= (y() - (rect.y + rect.height) );
		else
		if (y() < (rect.y) )
			y() += (y() - (rect.y) );
	}

	template<int N> Vertex<2> Vertex<N>::ConvertSpace(const Rect &from, const Rect &to) const
	{
		D_ASSERT_MESSAGE(N == 2, TEXT("This method is only defined for vertices with 2 dimensions"));
		float x = (this->x() - from.x) / from.width * to.width + to.x;
		float y = (this->y() - from.y) / from.height * to.height + to.y;
		return Vertex<2>(x, y);
	}

	template<int N> inline bool Vertex<N>::operator ! () const
	{
		return Length2() <= D_EPSILON2;
	}

	template<int N> inline Vertex<N>::operator const float* () const
	{
		return m_components;
	}
	
	template<int N> inline bool Vertex<N>::operator == (const Vertex<N> &v) const
	{
		return (*this - v).Length2() <= D_EPSILON2;
	}

	template<int N> inline bool Vertex<N>::operator != (const Vertex &v) const
	{
		return (*this - v).Length2() > D_EPSILON2;
	}

	template<int N> inline bool Vertex<N>::operator <= (const Vertex &v) const
	{
		return Length2() <= v.Length2() + D_EPSILON2;
	}

	template<int N> inline bool Vertex<N>::operator >= (const Vertex &v) const
	{
		return Length2() >= v.Length2() - D_EPSILON2;
	}

	template<int N> inline bool Vertex<N>::operator < (const Vertex &v) const
	{
		return Length2() < v.Length2() - D_EPSILON2;
	}

	template<int N> inline bool Vertex<N>::operator > (const Vertex &v) const
	{
		return Length2() > v.Length2() + D_EPSILON2;
	}

	/*!
		\brief Calculates the dot-product of this vector and the given vector
		\param v the vector
		\returns the dot-product
	*/
	template<int N> inline float Vertex<N>::DotProduct(const Vertex &v) const
	{
		float dot = 0.0f;
		for (int32 i = 0; i < N; i++) dot += m_components[i] * v[i];
		return dot;
	}


	template<int N> const Vertex<N> &Vertex<N>::operator = (const Vertex<N> &v)
	{
		for (int32 i = 0; i < N; i++) m_components[i] = v[i];
		return *this;
	}

	template<int N> const Vertex<N> &Vertex<N>::operator += (const Vertex<N> &v)
	{
		for (int32 i = 0; i < N; i++) m_components[i] += v[i];
		return *this;
	}

	template<int N> const Vertex<N> &Vertex<N>::operator -= (const Vertex<N> &v)
	{
		for (int32 i = 0; i < N; i++) m_components[i] -= v[i];
		return *this;
	}

	template<int N> const Vertex<N> &Vertex<N>::operator *= (float scale)
	{
		for (int32 i = 0; i < N; i++) m_components[i] *= scale;
		return *this;
	}

	template<int N> const Vertex<N> &Vertex<N>::operator /= (float scale)
	{
		D_ASSERT_MESSAGE(fabs(scale) > D_EPSILON2,TEXT("Vertex division by zero."));

		float inv_scale = 1.0/scale;
		for (int32 i = 0; i < N; i++) m_components[i] *= inv_scale;
		return *this;
	}

	template<int N> void Vertex<N>::RotateBy(float angle)
	{
		D_ASSERT_MESSAGE(N == 2, TEXT("This method is only defined for vertices with 2 dimensions."));
		float nx = cos(angle) * x() - sin(angle) * y();
		float ny = cos(angle) * y() + sin(angle) * x();
		x() = nx;
		y() = ny;
	}
	
	/*!
		\brief rotates the vertex
		\param rotation a quaternion describing the rotation
	*/
	template<int N> void Vertex<N>::RotateBy(const Quaternion &rotation)
	{
		D_ASSERT_MESSAGE(N == 3, TEXT("This method is only defined for vertices with 3 dimensions."));
		Matrix4x4 rot_matrix = rotation.ToMatrix();
		(*this) = rot_matrix * (*this);
	}


	template<int N> void Vertex<N>::RotateAround(const Vertex<3> &point, const Quaternion &rotation)
	{
		D_ASSERT_MESSAGE(N == 3, TEXT("This method is only defined for vertices with 3 dimensions."));

		Vertex<3> v = *this - point;
		v.RotateBy(rotation);
		*this = v + point;
	}


	/*!
		Normalizing a vector is done by dividing it with it's length and thus
		making the new length 1.0
		\brief normalizes the vector
		\warning a square root is calculated here, don't call this
		method too often
	*/
	template<int N> float Vertex<N>::Normalize()
	{
		float l = Length();


		if (fabs(l) > D_EPSILON2)
		{
			(*this) /= l;
		}
		return l;
	}

	/*!
		\brief tells you the index of the most dominant (greatest absolute value) axis value 0:x, 1:y, 2:z.
		\return the index
	*/
	template<int N> int32 Vertex<N>::Dominant() const
	{
//			int32 d(fabs(y) > fabs(x));
//			if(fabs(z) > fabs((*this)[d]))
//				d = 2;
//			return d;
		return 0;
	}

	/*!
		\brief interprets the given vertex as a vector and calculates the angle between the vectors.

		The angle is (as always) given in radians.
		\param inV the vector to compare to
	*/
	template<int N> float Vertex<N>::Angle(Vertex<N> &v) const
	{
		return acos(DotProduct(v));
	}

	/*!
		\brief Calculates the cross-product of this vector and the given vector.

		\param v the vector
		\return the cross-product
	*/
	template<int N> Vertex<3> Vertex<N>::CrossProduct(const Vertex<3> &v) const
	{
		D_ASSERT_MESSAGE(N == 3, TEXT("This method is only defined for vertices with 3 dimensions."));
		return Vertex<3>(y()*v.z() - z()*v.y(), z()*v.x() - x()*v.z(), x()*v.y() - y()*v.x());
	}

	template<int N> Vertex<N> Vertex<N>::operator / (float scale) const
	{
		D_ASSERT_MESSAGE(fabs(scale) > D_EPSILON2, TEXT("Vertex division by zero."));
		float inv_scale = 1.0 / scale;
		Vertex<N> ret;
		for (int32 i = 0; i < N; i++) ret[i] = m_components[i] * inv_scale;
		return ret;
	}

	/*!
		\brief sets the vector to the normal of the plane the three vertices describes.

		\param v1 a vertex on the plane
		\param v2 another vertx on the plane
		\param v3 another vertex on the plane
		\warning the three vertices must not lie in a straight line nor may any
		vertex be the samy as any other of the three vertices
	*/
	template<int N> void Vertex<N>::TriNormal(const Vertex<3> &v1, const Vertex<3> &v2, const Vertex<3> &v3)
	{
		D_ASSERT_MESSAGE(N == 3, TEXT("This method is only defined on vertices with 3 dimensions."));
		*this = (v1-v2).CrossProduct(v2 - v3);
		Normalize();
	}

} // dwarf

#endif // D_VERTEX_H_
