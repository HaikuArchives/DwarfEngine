#ifndef D_PLANE_H_
#define D_PLANE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Serializable.h"
#include "Vertex.h"
#include "Line.h"
#include "InStream.h"
#include "OutStream.h"

namespace dwarf
{

		/*!
			A plane is a flat surface that has an infinite area, it's often represented
			as one normal Vertex<3> and one distance from origo.
			\brief represents a plane in 3D space
			\image html Plane.gif
		*/

	template<int N> class Plane : public Serializable
	{
	public:

		Plane(): m_distance(0.0f) {}
		Plane(const Vertex<N> &normal, float distance = 0);
		Plane(const Vertex<N> &v, const Vertex<N> &normal);
		Plane(const Vertex<3> &v1, const Vertex<3> &v2, const Vertex<3> &v3);

		void SetTo(const Vertex<N> &normal, float distance);
		void SetTo(const Vertex<N> &v, const Vertex<N> &normal);
		void SetTo(const Vertex<3> &v1, const Vertex<3> &v2, const Vertex<3> &v3);

		void RotateBy(const Vertex<3> &rotate_around, const Quaternion &rotation);
		void RotateTo(const Vertex<3> &rotate_around, const Quaternion &rotation);
		
		float DistanceTo(const Vertex<N> &v) const;
		void MoveTo(const Vertex<N> &v);

		bool IsInside(const Vertex<N> &v) const;
		bool IsOnPlane(const Vertex<N> &v) const { return fabs(DistanceTo(v)) < D_EPSILON; }

		Vertex<N> PointOnPlane() const { return m_normal * -m_distance; }

		Line<N> Intersects(const Plane<N> &plane) const;
		Vertex<N> Intersects(const Line<N> &plane) const;

		void SetNormal(const Vertex<N> &normal);
		const Vertex<N> &Normal() const;
		float D() const { return m_distance; }
		Plane operator-() { return Plane<N>(-m_normal, -m_distance); }

		virtual void Restore(InStream &stream)
			{ stream >> m_normal >> m_distance; }
		virtual void Serialize(OutStream &stream) const
 			{ stream << m_normal << m_distance; }

	private:

		Vertex<N> m_normal;
		float m_distance;	
	};

	
	
	
	// inline definitions

	/*!
		\brief initializes the plane
		\param normal the plane's normal Vertex<3> (cannot be (0,0,0))
		\param distance the distance between origo and the point on the plane
		that is nearest origo.
	*/
	template<int N> inline Plane<N>::Plane(const Vertex<N> &normal, float dist):
		m_normal(normal),
		m_distance(dist)
	{
		m_normal.Normalize();
	}

	/*!
		\brief initializes the plane
		\param Vertex<3> is a Vertex<3> on the plane
		\param normal the plane's normal
	*/
	template<int N> inline Plane<N>::Plane(const Vertex<N> &v, const Vertex<N> &normal):
		m_normal(normal),
		m_distance(0)
	{
		m_normal.Normalize();
		m_distance = - m_normal.DotProduct(v);
	}

	/*!
		\brief initializes the plane
		\param v1 one Vertex<3> on the plane
		\param v2 another Vertex<3> on the plane
		\param v3 another Vertex<3> on the plane
		\warning these three vertices must together form a surface
		(they may not lie in a straight line and every Vertex<3> must
		be different from the two others)
	*/
	template<int N> inline Plane<N>::Plane(const Vertex<3> &v1, const Vertex<3> &v2, const Vertex<3> &v3):
		m_normal((v1-v2).CrossProduct(v2 - v3)),
		m_distance(0)
	{
		D_ASSERT_MESSAGE(N == 3, TEXT("This constructor is only definied for planes in 3 dimensions"));
//		D_ASSERT_MESSAGE(m_normal != Vertex<3>(0,0,0), TEXT("the points given to the Plane constructor do not span a plane"));
		m_normal.Normalize();
		m_distance = -m_normal.DotProduct(v1);
	}

	template<int N> inline void Plane<N>::SetTo(const Vertex<N> &normal, float distance)
	{
		m_normal = normal;
		m_distance = distance;
		m_normal.Normalize();
	}

	template<int N> inline void Plane<N>::RotateBy(const Vertex<3> &rotate_around, const Quaternion &rotation)
	{
		D_ASSERT_MESSAGE(N == 3, "This method is only definied for planes in 3 dimensions");
		m_normal.RotateBy(rotation);
		m_distance = -m_normal.DotProduct(rotate_around);
	}

	template<int N> inline void Plane<N>::RotateTo(const Vertex<3> &rotate_around, const Quaternion &rotation)
	{
		D_ASSERT_MESSAGE(N == 3, "This method is only definied for planes in 3 dimensions");
		m_normal = Vertex<3>(0,0,1);
		m_normal.RotateBy(rotation);
		m_distance = -m_normal.DotProduct(rotate_around);
	}

	template<int N> inline void Plane<N>::SetTo(const Vertex<N> &pos, const Vertex<N> &normal)
	{
		m_normal = normal;
		m_normal.Normalize();
		m_distance = - m_normal.DotProduct(pos);
	}

	// calculates the normal of a plane using three points, one
	// common startpoint and two endpoints
	template<int N> inline void Plane<N>::SetTo(const Vertex<3> &v1, const Vertex<3> &v2, const Vertex<3> &v3)
	{
		D_ASSERT_MESSAGE(N == 3, "This method is only definied for planes in 3 dimensions");
		SetTo(v1, (v1-v2).CrossProduct(v2 - v3));
	}

	/*!
		\brief moves the plane so the given point lies on the plane
		\param Vertex<3> the point that the plane should be moved to
	*/
	template<int N> inline void Plane<N>::MoveTo(const Vertex<N> &v)
	{
		m_distance = - m_normal.DotProduct(v);
	}

	/*!
		\brief sets the normal
		\param normal the new normal for the plane. Remember that the plane will
		be rotated around origo, if you want the plane to be rotated around any
		other point, use SetTo(const Vertex<3> &normal, float distance)
	*/
	template<int N> inline void Plane<N>::SetNormal(const Vertex<N> &normal)
	{
		m_normal = normal;
		m_normal.Normalize();
	}

	/*!
		\brief lets you know the plane's current normal
		\return the normal of the plane
	*/
	template<int N> inline const Vertex<N> &Plane<N>::Normal() const
	{
		return m_normal;
	}

	/*!
		\brief calculates the distance from the given point to the plane
		\param Vertex<3> the point to measure from
		\return the distance from the plane to the given point multiplied with the
		absolute value of the normal (distance * abs(normal))
		\sa Normalize()
	*/
	template<int N> inline float Plane<N>::DistanceTo(const Vertex<N> &v) const
	{
		return m_normal.DotProduct(v) + m_distance;
	}

	/*!
		\brief tests which side of the plane a Vertex<3> is
		\return true if the Vertex<3> is on the same side as the plane is faced
		(if the normal points to that side)
	*/
	template<int N> inline bool Plane<N>::IsInside(const Vertex<N> &v) const
	{
		return DistanceTo(v) > 0;
	}

	template<int N> inline Line<N> Plane<N>::Intersects(const Plane<N> &plane) const
	{
		if (N == 3)
		{
			Vertex<N> n1 = Normal();
			Vertex<N> n2 = plane.Normal();
			float d1 = m_distance;
			float d2 = plane.m_distance;

			Vertex<N> dir; // the direction of the line
			Vertex<N> point; // a point on the line

			float root = n2.x()/n1.x();

			dir.x() = n1.y()*(n2.z()-n1.z()*root)/(n2.y()-n1.y()*root) - n1.z();
			dir.y() = -(n2.z()-n1.z()*root)/(n2.y()-n1.y()*root);
			dir.z() = 1.0f;

			point.x() = -n1.y()*(d2-d1*root)/(n2.y()-n1.y()*root) - d1;
			point.y() = -(d2-d1*root)/(n2.y()-n1.y()*root);
			point.z() = 0.0f;
			return Line<N>(point, dir);
		}
	}

	template<int N> inline Vertex<N> Plane<N>::Intersects(const Line<N> &line) const
	{
		//! \todo add exceptions
		float t = (- m_normal.DotProduct(line.Position()) - m_distance) / m_normal.DotProduct(line.Direction());
		return line.Position() + line.Direction() * t;
	}

} // dwarf

#endif // D_PLANE_H_
