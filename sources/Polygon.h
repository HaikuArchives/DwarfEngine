#ifndef D_POLYGON_H_
#define D_POLYGON_H_


/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "ListArray.h"
#include "Vertex.h"
#include "Plane.h"

namespace dwarf
{

	class Polygon
	{
	public:
		Polygon(const ListArray<Vertex<3> > &polygon): m_vertices(polygon) {}
		Polygon(const Polygon &polygon): m_vertices(polygon.m_vertices) {}
		Polygon() {}

		void AddVertex(const Vertex<3> &vertex) { D_ASSERT_MESSAGE(NumVertices() < 3 || fabs(Plane<3>(m_vertices[0], m_vertices[1], m_vertices[2]).DistanceTo(vertex)) < D_EPSILON, TEXT("You can only add vertices that are all in the same plane, that's the definition on this polygon.")); m_vertices.AddItem(vertex); }
		int32 NumVertices() const { return m_vertices.ItemCount(); }

		Polygon ClipAgainst(const Plane<3> &plane) const;
		bool IsSplitBy(const Plane<3> &plane) const;

		const Vertex<3> &operator[](int32 i) const { return m_vertices[i]; }

		bool IsValid() const { return m_vertices.ItemCount() >= 3 && (m_vertices[0]-m_vertices[1]).CrossProduct(m_vertices[2]-m_vertices[1]) != Vertex<3>(0,0,0); }
	
	private:

		void ClipPolygon(const ListArray<Vertex<3> > &polygon, ListArray<Vertex<3> > &target, const Plane<3> &plane) const;

		ListArray<Vertex<3> > m_vertices;
	};

	inline Polygon Polygon::ClipAgainst(const Plane<3> &plane) const
	{
		Polygon poly;
		ClipPolygon(m_vertices, poly.m_vertices, plane);
		return poly;
	}
	

} // dwarf

#endif // D_POLYGON_H_
