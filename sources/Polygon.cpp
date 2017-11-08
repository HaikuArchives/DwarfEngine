

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Polygon.h"
#include "ListArray.h"
#include "Plane.h"

using namespace dwarf;

// Sutherland-Hodgeman Clipping 
void Polygon::ClipPolygon(const ListArray<Vertex<3> > &polygon, ListArray<Vertex<3> > &target, const Plane<3> &plane) const
{
	// if the polygon is empty, there is nothing to clip
	if (polygon.ItemCount() == 0) return;
	
	Array<float> distance(polygon.ItemCount());
	bool all_in = true;
	bool all_out = true;
	
	// calculate the distance from the plane to each Vertex<3> in the polygon
	int32 index = 0;
	for (ListArray<Vertex<3> >::ConstIterator i = polygon.Begin(); i.IsValid(); i++)
	{
		distance[index] = plane.DistanceTo(*i);
		if (distance[index] < 0) all_in = false;
		if (distance[index] > 0) all_out = false;
		index++;
	}
	
	// if all vertices are outside the plane, return an empty Vertex<3> list
	if (all_out)
	{
		target.Empty();
		return;
	}
	
	// if all vertices are inside the plane, return the original polygon
	if (all_in)
	{
		target = polygon;
		return;
	}
	
	
	
	target.Empty();
	index = 0;
	for (ListArray<Vertex<3> >::ConstIterator i = polygon.Begin(); i.IsValid(); i++)
	{
		// the next index, loops
		const int32 index_next = (index + 1) % polygon.ItemCount();
		ListArray<Vertex<3> >::ConstIterator j = i;
		j++;
		if (!j.IsValid()) j = polygon.Begin();
		
		const float dist1 = distance[index];
		const float dist2 = distance[index_next];
		
		const Vertex<3> v1 = *i;
		const Vertex<3> v2 = *j;
		
		// both inside
		if (dist1 > -D_EPSILON && dist2 > -D_EPSILON)
		{
			target.AddItem(v2);
		}
		// v1 inside and v2 outside, going out
		else if (dist1 > D_EPSILON && dist2 < -D_EPSILON)
		{
			//			target.AddItem(plane.Intersects(Line(v1, v2 - v1)));
			
			// optimization of the above line
			target.AddItem(v1 + (v2-v1) * (dist1/(dist1-dist2)));
		}
		// v1 outside and v2 inside, going in
		else if (dist1 < -D_EPSILON && dist2 > D_EPSILON)
		{
			//			target.AddItem(plane.Intersects(Line(v1, v1 - v2)));
			
			// optimization of the above line
			target.AddItem(v1 + (v2-v1) * (dist1/(dist1-dist2)));
			target.AddItem(v2);
		}
		else if ((dist1< -D_EPSILON || dist1 > D_EPSILON) && fabs(dist2) < D_EPSILON)
		{
			target.AddItem(v2);
		}
		
		index++;
	}
}

bool Polygon::IsSplitBy(const Plane<3> &plane) const
{
	bool inside = plane.IsInside(m_vertices[0]);
	for(int32 i = 1; i < NumVertices(); i++)
	{
		if (plane.IsInside(m_vertices[i]) != inside)
		{
			// ok, this polygon is be split.
			return true;
		}
	}
	return false;
}
