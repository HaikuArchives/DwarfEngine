#ifndef D_GENERIC_PORTAL_H_
#define D_GENERIC_PORTAL_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "Shader.h"
#include "Matrix4x4.h"
#include "Object.h"
#include "Vertex.h"
#include "ListArray.h"
#include "Plane.h"
#include "Camera.h"
#include "Portal.h"

namespace dwarf
{
	class RenderingDevice;

	class GenericPortal : public Portal
	{
	public:
		
		GenericPortal()	{}
		GenericPortal(const Object &target, const ListArray<poly_vertex> &polygon);

		virtual void Draw(RenderingDevice &device, const Camera &camera, int32 depth) const;

		void SetTarget(const Object &target);
		Object &Target() { return m_target; }
		void Update() { SetTarget(m_target); }

	private:

		ListArray<poly_vertex> m_polygon;
		Plane<3> m_poly_plane; // the rotated polygon plane
		Plane<3> m_plane; // the polygon plane
		Object m_target;
		Quaternion m_rotation;
		Vertex<3> m_portal_center;

	};


} // dwarf


#endif // D_GENERIC_PORTAL_H_
