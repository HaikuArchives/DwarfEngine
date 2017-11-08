/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Portal.h"
#include "Shader.h"
#include "Matrix4x4.h"
#include "RenderingDevice.h"
#include "State.h"
#include <GL/gl.h>

using namespace dwarf;

// temporary clipfunction
/*
void Portal::ClipPolygon(const ListArray<Vertex<3>> &polygon, ListArray<Vertex<3>> &poly, const Plane<3> &plane) const
{
	Vertex<3> prev; // contains the previous Vertex<3>
	Vertex<3> v; // temporary intersection point
	bool inside;

	poly.Empty();

	// find the first cut
	ListArray<Vertex<3>>::ConstIterator i = polygon.Begin();
	inside = plane.IsInside(*i);
	for (; i.IsValid() && plane.IsInside(*i) == inside; i++)
	{
		prev = *i;
		if (inside) poly.AddItem(*i);
	}
	if (i.IsValid()) // if there was a cut
	{
		// make the first cut
		v = plane.Intersects(Line(*i, *i - prev));
		poly.AddItem(v);

		// find the second cut
		inside = plane.IsInside(*i);
		for (; i.IsValid() && plane.IsInside(*i) == inside; i++)
		{
			prev = *i;
			if (inside) poly.AddItem(*i);
		}
		if (!i.IsValid())
		{
			// the cut may be between the first and last point
			i = polygon.Begin();
		}
		// make the first cut
		v = plane.Intersects(Line(*i,  *i - prev));
		poly.AddItem(v);

		// add the rest of the points that's inside the plane
		if (i != polygon.Begin()) for (; i.IsValid(); i++) poly.AddItem(*i);
	}
}
*/



// Sutherland-Hodgeman Clipping 
void Portal::ClipPolygon(const ListArray<poly_vertex> &polygon, ListArray<poly_vertex> &target, const Plane<3> &plane) const
{
	// if the polygon is empty, there is nothing to clip
	if (polygon.ItemCount() == 0) return;

	Array<float> distance(polygon.ItemCount());
	bool all_in = true;
	bool all_out = true;

	// calculate the distance from the plane to each Vertex<3> in the polygon
	int32 index = 0;
	for (ListArray<poly_vertex>::ConstIterator i = polygon.Begin(); i.IsValid(); i++)
	{
		distance[index] = plane.DistanceTo((*i).vertex);
		if (distance[index] < 0) all_in = false;
		if (distance[index] >= 0) all_out = false;
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
	for (ListArray<poly_vertex>::ConstIterator i = polygon.Begin(); i.IsValid(); i++)
	{
		// the next index, loops
		const int32 index_next = (index + 1) % polygon.ItemCount();
		ListArray<poly_vertex>::ConstIterator j = i;
		j++;
		if (!j.IsValid()) j = polygon.Begin();

		const float dist1 = distance[index];
		const float dist2 = distance[index_next];
		
		const poly_vertex v1 = *i;
		const poly_vertex v2 = *j;

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

