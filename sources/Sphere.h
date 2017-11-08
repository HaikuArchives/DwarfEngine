#ifndef D_SPHERE_H_
#define D_SPHERE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "Point.h"

#include "Log.h"
#include "Vertex<3>.h"
#include "Vector.h"

namespace dwarf
{
	class Sphere
	{
	public:
		Sphere();
		Sphere(const Vector &origin,float radius);
		Sphere(const Sphere &sphere);
		virtual ~Sphere();

		bool Contains(const Vertex<3> &point) const;
		bool Contains(const Box &rect) const;

	private:
		Vector m_origin;
		float m_radius;
	};

	inline Sphere::Sphere(): 
		m_origin(0.0f, 0.0f, 0.0f),
		m_radius(0.0f)
	{
	}

	inline Sphere::Sphere(const Vector &origin,float radius): 
		m_origin(origin),
		m_radius(radius)
	{
	}

	inline Sphere::Sphere(const Sphere &sphere):
		m_origin(sphere.m_origin),
		m_radius(sphere.m_radius)
	{
	}

	inline Sphere::~Sphere()
	{
	}

	inline bool Sphere::Contains(const Vector &point) const
	{
		return ((point - m_origin).Length() <= m_radius) ? true : false;
	}

	inline bool Sphere::Contains(const Box &rect) const
	{
	}

} // dwarf

#endif D_SPHERE_H_