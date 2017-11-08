/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Frustum.h"
#include "Object.h"
#include "RenderingDevice.h"
#include "MirrorMatrix.h"

using namespace dwarf;

Frustum::Frustum(const Vertex<3> &position):
	m_position(position),
	m_plane(7),
	m_num_planes(0),
	m_custom_near_plane(true),
	m_portal(10),
	m_portal_points(0)
{
	// initialize the planes
}

Frustum::Frustum(const Frustum &frustum):
	m_position(frustum.m_position),
	m_plane(frustum.m_plane),
	m_num_planes(frustum.m_num_planes),
	m_custom_near_plane(frustum.m_custom_near_plane),
	m_portal(10),
	m_portal_points(0)
{
}

void Frustum::Restore(InStream& stream) throw ()
{
	stream >> m_position >> m_num_planes;
	for (int32 i = 0; i < m_num_planes; i++)
		stream >> m_plane[i];
	stream >> m_clip_plane[0] >> m_clip_plane[1];
}

void Frustum::Serialize(OutStream& stream) const
{
	stream << m_position << m_num_planes;
	for (int32 i = 0; i < m_num_planes; i++)
		stream << m_plane[i];
	stream << m_clip_plane[0] << m_clip_plane[1];
}

/*!
	\brief initializes the frustum to a normal rectangular frustum.

	\param position the eye position.
	\param direction the looking direction.
	\param vertical_fov the vertical field of vision, given in radians.
	\param horizontal_fov the horizontal field of vision, given in radians.
	\param near_clip the distance to the near-z clip plane, this may not be greater
	than \e far_clip and it must be greater than 0.
	\param far_clip the distance to the fasr-z clip plane, this must be greater than
	\e near_clip.

*/
void Frustum::SetTo(const Vertex<3> &position,const Quaternion &direction,float vertical_fov,float horizontal_fov, float near_clip, float far_clip)
{
	D_ASSERT_MESSAGE(far_clip > 0, TEXT("You cannot have the far clip plane behind the camera (it must be greater than 0)"));
	D_ASSERT_MESSAGE(near_clip > 0, TEXT("You cannot have the near clip plane behind the camera (it must be greater than 0)"));
	D_ASSERT_MESSAGE(far_clip > near_clip, TEXT("You are trying to set the far clip plane in front of the near clip plane"));
	D_ASSERT_MESSAGE(vertical_fov > 0, TEXT("You are trying to set the vertical field of vision to a negative number"));
	Vertex<3> n;

	m_custom_near_plane = false;

	if(m_plane.Length() < 4) m_plane.SetLength(4);

	m_position = position;

	horizontal_fov *= .5f;
	vertical_fov *= .5f;

	m_num_planes = 4;

	// left plane
	n = Vertex<3>(cos(horizontal_fov),0.0f,-sin(horizontal_fov));
	n.RotateBy(direction);
	m_plane[0].SetTo(position,n);

	// right plane
	n = Vertex<3>(-cos(horizontal_fov),0.0f,-sin(horizontal_fov));
	n.RotateBy(direction);
	m_plane[1].SetTo(position,n);

	// bottom plane
	n = Vertex<3>(0.0f,-cos(vertical_fov),-sin(vertical_fov));
	n.RotateBy(direction);
	m_plane[2].SetTo(position,n);

	// top plane
	n = Vertex<3>(0.0f,cos(vertical_fov),-sin(vertical_fov));
	n.RotateBy(direction);
	m_plane[3].SetTo(position,n);


	// far plane
	n = Vertex<3>(0,0,-far_clip);
	n.RotateBy(direction);
	m_clip_plane[0] = Plane<3>(m_position + n, -n);

	// near plane
	n = Vertex<3>(0,0,-near_clip);
	n.RotateBy(direction);
	m_clip_plane[1] = Plane<3>(m_position + n, n);

	m_position = position;
}


void Frustum::RotateBy(const Quaternion &rotation)
{
	// rotates every normal and recalculates every plane
	for (int32 i = 0; i < m_num_planes; i++)
	{
		Vertex<3> normal = m_plane[i].Normal();
		normal.RotateBy(rotation);
		
		// sets the new plane
		m_plane[i].SetTo(m_position, normal);
	}

	// the far & near clip planes never intersect the point (m_position)
	// and are handled separately

	for (int32 i = 0; i < 2; i++)
	{
		float distance = m_clip_plane[i].DistanceTo(m_position);
		Vertex<3> normal = m_clip_plane[i].Normal();
		normal.RotateBy(rotation);
		m_clip_plane[i].SetTo(m_position + normal * distance, normal);
	}

	// rotate the portal points if any
	if (m_portal_points > 0)
	{
		for (int32 i = 0; i < m_portal_points; i++) m_portal[i].RotateAround(m_position, rotation);
	}
	
}

void Frustum::MoveBy(const Vertex<3> &delta)
{
	for (int32 i = 0; i < m_num_planes; i++)
	{
		// moves the plane
		m_plane[i].MoveTo(m_position + delta);
	}

	// the far & near clip planes never intersect the point (m_position)
	// and are therefore handled separately!

	Vertex<3> normal;
	float distance;

	// the far clip plane
	distance = m_clip_plane[0].DistanceTo(m_position);
	normal = m_clip_plane[0].Normal();
	m_clip_plane[0].SetTo(m_position + delta + normal * distance, normal);
	
	// the near clip plane
	distance = m_clip_plane[1].DistanceTo(m_position);
	normal = m_clip_plane[1].Normal();
	m_clip_plane[1].SetTo(m_position + delta + normal * distance, normal);

	// move the portal points, if any
	if (m_portal_points > 0) for (int32 i = 0; i < m_portal_points; i++) m_portal[i] += delta;

	m_position += delta;
}

void Frustum::MoveTo(const Vertex<3> &position)
{
	for (int32 i = 0; i < m_num_planes; i++)
	{
		// moves the plane
		m_plane[i].MoveTo(position);
	}

	// the far & near clip planes never intersect the point (m_position)
	// and are therefore handled separately!

	Vertex<3> normal;
	float distance;

	// the far clip plane
	distance = m_clip_plane[0].DistanceTo(m_position);
	normal = m_clip_plane[0].Normal();
	m_clip_plane[0].SetTo(position + normal * distance, normal);
	
	// the near clip plane
	distance = m_clip_plane[1].DistanceTo(m_position);
	normal = m_clip_plane[1].Normal();
	m_clip_plane[1].SetTo(position + normal * distance, normal);

	// move the portal points, if any
	if (m_portal_points > 0)
	{
		Vertex<3> delta = position - m_position;
		for (int32 i = 0; i < m_portal_points; i++) m_portal[i] += delta;
	}

	m_position = position;
}

void Frustum::Mirror(const Plane<3> &plane)
{
	MirrorMatrix mirror_origo(Plane<3>(plane.Normal(), 0));
	MirrorMatrix mirror(plane);

	Vertex<3> new_pos = mirror * m_position;

	for (int32 i = 0; i < m_num_planes; i++) m_plane[i] = Plane<3>(new_pos, mirror_origo * m_plane[i].Normal());

	for (int32 i = 0; i < 2; i++)
	{
		Vertex<3> point = m_clip_plane[i].PointOnPlane();
		point = mirror * point;
		Vertex<3> normal = mirror_origo * m_clip_plane[i].Normal();
		m_clip_plane[i] = Plane<3>(point, normal);
	}

}

void Frustum::AddPlanes(const ListArray<Vertex<3> > &polygon)
{
	if (m_portal.Length() < polygon.ItemCount()) m_portal.SetLength(polygon.ItemCount());
	m_portal_points = polygon.ItemCount();

	int32 j = 0;
	ListArray<Vertex<3> >::ConstIterator i = polygon.Begin();
	if (!i.IsValid()) return;

	m_portal[j] = *i;
	Vertex<3> point = *i;
	i++;
	j++;

	while(i.IsValid())
	{
		AddPlane(point, *i);
		point = *i;
		m_portal[j] = *i;
		j++;
		i++;
	}
	if(polygon.ItemCount() > 2)
	{
		AddPlane(point, *polygon.Begin());
	}
}

