#ifndef D_MIRROR_PORTAL_H_
#define D_MIRROR_PORTAL_H_

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

	class MirrorPortal : public Portal
	{
	public:
		
		MirrorPortal()	{}
		MirrorPortal(const ListArray<poly_vertex> &polygon);

		virtual void Draw(RenderingDevice &device, const Camera &camera, int32 depth) const;

	private:

		ListArray<poly_vertex> m_polygon;
		Plane<3> m_plane; // the polygon plane
		Vertex<3> m_center; // the center of the portal polygon, used as virtual position

	};


} // dwarf


#endif // D_MIRROR_PORTAL_H_
