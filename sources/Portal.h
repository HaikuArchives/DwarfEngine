#ifndef D_PORTAL_H_
#define D_PORTAL_H_

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

namespace dwarf
{
	class RenderingDevice;

	class Portal
	{
	public:

		struct poly_vertex
		{
		public:
			poly_vertex():vertex(Vertex<3>(0,0,0)),texcoord(Point(0,0)) {}
			poly_vertex(const Vertex<3> &vertex, const Point &texcoord = Point(0,0)):vertex(vertex),texcoord(texcoord) {}
			poly_vertex operator+(const poly_vertex &v) const { return poly_vertex(vertex + v.vertex, texcoord + v.texcoord); }
			poly_vertex operator-(const poly_vertex &v) const { return poly_vertex(vertex - v.vertex, texcoord - v.texcoord); }
			poly_vertex operator*(float scalar) const { return poly_vertex(vertex * scalar, texcoord *scalar); }

			Vertex<3> vertex;
			Point texcoord;
		};


		Portal(): m_enabled(true) {}

		virtual ~Portal() {}

		virtual void Draw(RenderingDevice &device, const Camera &camera, int32 depth) const = 0;

		void Enable(bool enabled) { m_enabled = enabled; }
		bool IsEnabled() { return m_enabled; }
		void SetShader(const HShader &shader) { m_shader = shader; }

	protected:

		void ClipPolygon(const ListArray<poly_vertex> &polygon, ListArray<poly_vertex> &target, const Plane<3> &plane) const;

		HShader m_shader;
		bool m_enabled;
	};


} // dwarf


#endif // D_PORTAL_H_
