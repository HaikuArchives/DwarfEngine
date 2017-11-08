/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <list>

#include "Defs.h"

#include "Camera.h"
#include "Object.h"
#include "RenderingDevice.h"
#include "Sector.h"
#include "Matrix4x4.h"
#include "MirrorMatrix.h"

using namespace dwarf;


Camera::Camera():
	Object(),
	m_frustum(),
	m_far_clip(3000.0f),
	m_near_clip(1.0f),
	m_vfov(D_PI*.5f),
	m_hfov(D_PI*.5f),
	m_virtual_position(Position())
{
	m_frustum.SetTo(Position(), Rotation(), m_vfov, m_hfov, m_near_clip, m_far_clip);
}
	
Camera::Camera(const Object &object):
	Object(object),
	m_frustum(object.Position()),
	m_far_clip(3000.0f),
	m_near_clip(1.0f),
	m_vfov(D_PI*.5f),
	m_hfov(D_PI*.5f),
	m_virtual_position(Position())
{
	m_frustum.SetTo(Position(), Rotation(), m_vfov, m_hfov, m_near_clip, m_far_clip);
}

Camera::~Camera()
{
}

void Camera::MoveTo(const Vertex<3> &pos)
{
	m_virtual_position = pos;
	m_position = pos;
	m_frustum.SetTo(Position(), Rotation(), m_vfov, m_hfov, m_near_clip, m_far_clip);
}

void Camera::MoveBy(const Vertex<3> &delta)
{
	m_virtual_position += delta;
	m_position+= delta;
	m_frustum.SetTo(Position(), Rotation(), m_vfov, m_hfov, m_near_clip, m_far_clip);
}

void Camera::RotateTo(const Quaternion &rotation)
{
	m_rotation = rotation;
	m_frustum.SetTo(Position(), Rotation(), m_vfov, m_hfov, m_near_clip, m_far_clip);
}

void Camera::RotateBy(const Quaternion &rotation)
{
	m_rotation *= rotation;
	m_frustum.SetTo(Position(), Rotation(), m_vfov, m_hfov, m_near_clip, m_far_clip);
}

void Camera::Draw(RenderingDevice &device/*, uList<uLight*> *lightsources*/) const
{
	D_ASSERT_MESSAGE(HasSector(), TEXT("You are trying to draw a camera that doesn't belong to any sector."));
	// if the recursive depth is too great, we should probably draw a black poly or something

	device.PushMatrix();
	device.LoadIdentity(); 	// this should always be done

	device.Set3DMode(m_vfov, m_hfov, m_near_clip, m_far_clip);

	device.Rotate(-Rotation());

	// if this is the top call to the camera's draw method (not a recursive call through a portal or something)
	// we have to draw the skydome
//	if (depth == 0)
	{
//		uSkyDome *sky = tmp_sector.Sky();
//		if (sky) sky->Draw();
	}
	device.Translate(-Position());

	GetSector()->DrawSector(device, *this /*, lightsources*/, 0);
	D_ASSERT_MESSAGE(device.GetTextureMatrix() == IdentityMatrix(), TEXT("Your sector did not reset the texture matrix to identity, you have to do that"));

	device.PopMatrix();
}


void Camera::Draw(RenderingDevice &device,/* uList<uLight*> *lightsources,*/ int depth) const
{
	D_ASSERT_MESSAGE(HasSector(), TEXT("You are trying to draw a camera that doesn't belong to any sector."));
	D_ASSERT_MESSAGE(depth > 0, TEXT("You cannot give the Camera::Draw() a depth less than one."));
	// if the recursive depth is too great, we should probably draw a black poly or something
	if (depth > 10) return;
	
	GetSector()->DrawSector(device, *this /*, lightsources*/, depth);
	D_ASSERT_MESSAGE(device.GetTextureMatrix() == IdentityMatrix(), TEXT("Your sector did not reset the texture matrix to identity, you have to do that"));
}


void Camera::Mirror(const Plane<3> &plane)
{
	m_frustum.Mirror(plane);

	MirrorMatrix mirror(plane);

	m_position = mirror * m_position;
}
