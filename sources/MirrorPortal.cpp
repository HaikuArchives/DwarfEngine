/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Polygon.h"
#include "MirrorPortal.h"
#include "Shader.h"
#include "Matrix4x4.h"
#include "RenderingDevice.h"
#include "State.h"
#include "MirrorMatrix.h"
#include <GL/gl.h>

using namespace dwarf;

MirrorPortal::MirrorPortal(const ListArray<poly_vertex> &polygon):
	m_polygon(polygon),
	m_plane(polygon[0].vertex, polygon[1].vertex, polygon[2].vertex)
{
	m_center = Vertex<3>(0,0,0);
	for (ListArray<poly_vertex>::Iterator i = m_polygon.Begin(); i.IsValid(); i++)
		m_center += (*i).vertex;
	m_center /= m_polygon.ItemCount();
	m_center += m_plane.Normal() * D_EPSILON;
}

void MirrorPortal::Draw(RenderingDevice &device, const Camera &camera, int32 depth) const
{
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

	// construct the temporary camera, move it, rotate it
	Camera cam(camera);

	// set near and far z clip-planes
	portal_frustum.SetNearClip(m_plane);

	Vector n = Vector(0, 0, camera.FarClip());
	n.RotateBy(camera.Rotation());
	portal_frustum.SetFarClip(Plane<3>(portal_frustum.Position() - n, n));


	// enable the glClipPlane()
	device.EnableState(RenderingDevice::CLIP_PLANE0);

	// finally, set the frustum to the camera before we draw it
	cam.SetFrustum(portal_frustum);

	cam.Mirror(m_plane);

	// make sure everything is ok
//	D_ASSERT_MESSAGE(cam.Position() == portal_frustum.Position(), TEXT("This should not happen, if this happens we have done something wrong, please make a bugreport with an example that reproduces the bug. This happens when the camera is moved to another postion than it's frustum, in the portal class, it can only occur if you use portals."));
	
	// move the cameras virtual position to target
	cam.SetVirtualPosition(m_center);

	device.PushMatrix();
	device.SetClipPlane(portal_frustum.NearClip());
	
	device.MultMatrix(MirrorMatrix(m_plane));

	cam.Draw(device, depth+1);

	device.PopMatrix();

	
	device.DisableState(RenderingDevice::CLIP_PLANE0);
	
	if (camera.ViewFrustum().PortalPoints() == 0)
	{
		// if our frustum was not build around a portal, we have to clear the entire z-buffer
		device.ClearDepthBuffer();
	}
	else
	{
//		State state;
//		device.SetStates(state);

		// clear the z buffer
		device.SetDepthRange(1,1);

		State state;
		state.Disable(State::CULL_FACE + State::TEXTURE_2D + State::TEXTURE_1D + State::ALPHA_TEST);
		state.Enable(State::BLEND + State::DEPTH_TEST);
		state.SetBlendMode(RenderingDevice::ZERO, RenderingDevice::ONE);
		state.SetDepthTestFunction(RenderingDevice::ALWAYS);
		
		device.SetStates(state);
		
		device.SetDepthFunc(RenderingDevice::ALWAYS);
 
		device.Begin(RenderingDevice::TRIANGLE_FAN);
//		device.Begin(RenderingDevice::LINE_LOOP);
		
		for(int32 i = 0; i < camera.ViewFrustum().PortalPoints(); i++)
		{
			device.AddVertex(camera.ViewFrustum().PortalPoly()[i]);
		}
		device.End();
		device.SetDepthRange(0,1);
	}

	if (camera.ViewFrustum().CustomNearPlane())
	{
		device.EnableState(RenderingDevice::CLIP_PLANE0);
		// reset to the current sector's clip plane
		device.SetClipPlane(camera.ViewFrustum().NearClip());
	}
	else
	{
		// if the current sector doesn't have a custom clip plane, disable it the hardware clip plane
//		device.DisableState(RenderingDevice::CLIP_PLANE0);
	}
	

	// draw the frustum ** DEBUG PURPOSE **
/*
	device.DisableState(RenderingDevice::TEXTURE_2D);
	device.DisableState(RenderingDevice::BLEND);
	device.SetColor(Color(1, .1, .5));
	device.Begin(RenderingDevice::LINE_LOOP);
	for (int32 i = 0;i < poly.ItemCount(); i++) device.AddVertex(poly[i].Vertex<3>);
	device.End();
*/

	if (!m_shader.IsValid())
	{
		State state;
		state.Disable(State::CULL_FACE);
		state.Enable(State::BLEND);
		state.SetBlendMode(RenderingDevice::ZERO, RenderingDevice::ONE);
		state.SetDepthTestFunction(RenderingDevice::ALWAYS);
		device.SetStates(state);
		device.Begin(RenderingDevice::TRIANGLE_FAN);

		device.AddVertex(poly[0].vertex);
		for(int32 i = 1; i < poly.ItemCount(); i++)
		{
			device.AddVertex(poly[i].vertex);
		}
		device.End();
	}
	else
	{
		for (ListArray<Shader::Layer>::ConstIterator i = m_shader->Layers(); i.IsValid(); i++)
		{
			device.SetLayerState(*i, 0);
			device.DisableState(RenderingDevice::CULL_FACE);

			device.EnableState(RenderingDevice::ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);

			device.Begin(RenderingDevice::TRIANGLE_FAN);
//			device.Begin(RenderingDevice::POLYGON);

			device.TexCoord(poly[0].texcoord);
			device.AddVertex(poly[0].vertex);
			for(int32 j = 1; j < poly.ItemCount(); j++)
			{
				//! \todo add texture coordinates

				device.TexCoord(poly[j].texcoord);
				device.AddVertex(poly[j].vertex);
			}
			device.End();

			device.DisableState(RenderingDevice::ALPHA_TEST);
		}
		device.ResetTexture();
	}

	device.EnableState(RenderingDevice::DEPTH_TEST);
}



