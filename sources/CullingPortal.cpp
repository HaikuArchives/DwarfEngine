/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "CullingPortal.h"
#include "Shader.h"
#include "Matrix4x4.h"
#include "RenderingDevice.h"
#include "State.h"
#include <GL/gl.h>

using namespace dwarf;

CullingPortal::CullingPortal(const Object &target, const ListArray<poly_vertex> &polygon):
	m_polygon(polygon),
	m_target(target)
{
	SetTarget(target);
}

void CullingPortal::Draw(RenderingDevice &device, const Camera &camera, int32 depth) const
{
	D_ASSERT_MESSAGE(m_target.GetSector() != 0, TEXT("You cannot use a portal with an uninitialized target"));
	if (!m_enabled) return;
	
	// if the portal is seen from behind, exit
	if (m_plane.IsInside(camera.Position())) return;

	// Clips the polygon (m_polygon) against the camera's frustum
	// the resulting polygon is put in poly
	ListArray<poly_vertex> poly;
	ListArray<poly_vertex> tmp;


	// first clip against the near and far planes
//	ClipPolygon(m_polygon, tmp, camera.ViewFrustum().NearClip());
//	ClipPolygon(tmp, poly, camera.ViewFrustum().FarClip());

	poly = m_polygon;

	// then clip against the frustum planes
	for (int32 i = 0; i < camera.ViewFrustum().NumPlanes(); i++)
	{
		ClipPolygon(poly, tmp, camera.ViewFrustum()[i]);
		poly = tmp;
		//! \todo this could be optimized
// the lines below introduces an error in the clipping
/*
		i++;
		if (i >= camera.ViewFrustum().NumPlanes())
		{
			poly = tmp;
			break;
		}
		ClipPolygon(tmp, poly, camera.ViewFrustum()[i]);
*/
	}





	// now when we have clipped the portal polygon against the cyrrent frustum, let's
	// see if there's anything left
	if (poly.ItemCount() < 3) return;

	// now we have to build a new frustum around the portal polygon
	Frustum portal_frustum(camera.Position());
	ListArray<Vertex<3> > portal_poly;
	for(ListArray<poly_vertex>::Iterator i = poly.Begin(); i.IsValid(); i++) portal_poly.AddItem((*i).vertex);
	portal_frustum.AddPlanes(portal_poly);
/*
	for(ListArray<poly_vertex>::Iterator i = poly.Begin(); i.IsValid(); i++)
	{
		ListArray<poly_vertex>::Iterator j = i;
		j++;
		if (!j.IsValid()) j = poly.Begin();
		portal_frustum.AddPlane((*i).Vertex<3>,(*j).Vertex<3>);
	}
*/
	// temporary fix, so that we can test this without changing states everywhere else
//	device.DisableState(RenderingDevice::BLEND);

	// construct the temporary camera, move it, rotate it
	Camera cam(m_target);

	// set near and far z clip-planes
	portal_frustum.SetNearClip(m_poly_plane);

	Vector n = Vector(0, 0, camera.FarClip());
	n.RotateBy(camera.Rotation());
	portal_frustum.SetFarClip(Plane<3>(portal_frustum.Position() - n, n));

	// make sure everything is ok
	D_ASSERT_MESSAGE(cam.Position() == portal_frustum.Position(), TEXT("This should not happen, if this happens we have done something wrong, please make a bugreport with an example that reproduces the bug. This happens when the camera is moved to another postion than it's frustum, in the portal class, it can only occur if you use portals."));

	// finally, set the frustum to the camera before we draw it
	cam.SetFrustum(portal_frustum);

	// move the cameras virtual position to target
	cam.SetVirtualPosition(m_target.Position());

	cam.Draw(device, depth+1);

	// draw the frustum ** DEBUG PURPOSE **
/*
	device.DisableState(RenderingDevice::TEXTURE_2D);
	device.DisableState(RenderingDevice::BLEND);
	device.SetColor(Color(1, .1, .5));
	device.Begin(RenderingDevice::LINE_LOOP);
	for (int32 i = 0;i < poly.ItemCount(); i++) device.AddVertex(poly[i].Vertex<3>);
	device.End();
*/

	if (m_shader.IsValid())
	{
		for (ListArray<Shader::Layer>::ConstIterator i = m_shader->Layers(); i.IsValid(); i++)
		{
			device.SetLayerState(*i, 0);
			device.DisableState(RenderingDevice::CULL_FACE);
			device.Begin(RenderingDevice::TRIANGLE_FAN);
//			device.Begin(RenderingDevice::POLYGON);

			device.TexCoord(poly[0].texcoord);
			device.AddVertex(poly[0].vertex);
			for(int32 j = 1; j < poly.ItemCount(); j++)
			{
				device.TexCoord(poly[j].texcoord);
				device.AddVertex(poly[j].vertex);
			}
			device.End();
		}
		device.ResetTexture();
	}

	device.EnableState(RenderingDevice::DEPTH_TEST);
}






void CullingPortal::SetTarget(const Object &target)
{
	m_target = target;


	// calculate the relative rotation
	// by finding the normal of the portal-polygon
	Vector portal_normal;
	portal_normal.TriNormal(m_polygon[0].vertex, m_polygon[1].vertex, m_polygon[2].vertex);

	float heading = (portal_normal.x() != 0 || portal_normal.z() != 0)?atan2(portal_normal.x(), -portal_normal.z()):0.0f;
	float pitch = (portal_normal.y() != 0 || portal_normal.z() != 0)?atan2(-portal_normal.y(), sin(heading) * portal_normal.z() + cos(heading) * portal_normal.x()):0.0f;

	m_rotation = Quaternion(Euler(pitch, heading, 0)) * target.Rotation();
	
	
	// calculate the relative movement
	// by calculating the mean value of all vertices in the portal-polygon
	
	m_portal_center = Vertex<3>(0,0,0);
	for (ListArray<poly_vertex>::Iterator i = m_polygon.Begin(); i.IsValid(); i++)
		m_portal_center += (*i).vertex;
	m_portal_center /= static_cast<float>(m_polygon.ItemCount());


	m_plane = Plane<3>(m_portal_center, portal_normal);

	// construct the polygon's plane (rotated to the target coordinate-system)
	// so it can be used as near-z clip plane.
	Vector n = portal_normal;
	n.RotateBy(m_rotation);
	m_poly_plane = Plane<3>(target.Position(), n);
}

