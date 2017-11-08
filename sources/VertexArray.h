#ifndef D_VERTEXARRAY_H
#define D_VERTEXARRAY_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Sector.h"
#include "Dwarf.h"
#include "Vertex.h"

namespace dwarf
{
	class VertexArray
	{
	public:
		VertexArray():
			m_vertices(0),
			m_num_vertices(0),
			m_normals(0),
			m_has_normals(false),
			m_texture_coords(0),
			m_has_texture_coords(false),
			m_colors(0),
			m_has_colors(false),
			m_indices(0),
			m_has_indices(true),
			m_num_indices(0),
			m_primitive(RenderingDevice::TRIANGLES)
		{ }

		VertexArray(const VertexArray &array):
			m_vertices(array.Vertices()),
			m_num_vertices(array.NumVertices()),
			m_normals(array.Normals()),
			m_has_normals(array.HasNormals()),
			m_texture_coords(array.TextureCoords()),
			m_has_texture_coords(array.HasTextureCoords()),
			m_colors(array.Colors()),
			m_has_colors(array.HasColors()),
			m_indices(array.Indices()),
			m_has_indices(array.HasIndices()),
			m_num_indices(array.NumIndices()),
			m_primitive(array.Primitive())
		{ }

		VertexArray(RenderingDevice::Primitive primitive):
			m_vertices(0),
			m_num_vertices(0),
			m_normals(0),
			m_has_normals(false),
			m_texture_coords(0),
			m_has_texture_coords(false),
			m_colors(0),
			m_has_colors(false),
			m_indices(0),
			m_has_indices(true),
			m_num_indices(0),
			m_primitive(primitive)
		{ }

		VertexArray(RenderingDevice::Primitive primitive, int32 length, bool has_texcoords = false, bool has_colors = false):
			m_vertices(length),
			m_num_vertices(0),
			m_normals(0),
			m_has_normals(false),
			m_texture_coords(0),
			m_has_texture_coords(false),
			m_colors(0),
			m_has_colors(has_colors),
			m_indices(0),
			m_has_indices(true),
			m_num_indices(0),
			m_primitive(primitive)
		{
			if(has_texcoords)
			{
				m_has_texture_coords = true;
				m_texture_coords.SetLength(length);
			}
		}

		virtual ~VertexArray() {}
		RenderingDevice::Primitive Primitive() const { return m_primitive;}
		const Array<Vertex<3> > &Vertices() const { return m_vertices; }
		const Array<Point> &TextureCoords() const { return m_texture_coords; }
		const Array<Vertex<3> > &Normals() const { return m_normals; }
		const Array<Color> &Colors() const { return m_colors; }
		const Array<uint32> &Indices() const { return m_indices; }
		bool HasNormals() const { return m_has_normals; }
		bool HasTextureCoords() const { return m_has_texture_coords; }
		bool HasTextureCoords(bool b) { return m_has_texture_coords = b; }
		bool HasIndices() const { return m_has_normals; }
		bool HasColors() const { return m_has_colors; }

		void AddVertex(const Vertex<3> &vertex)
		{
			m_vertices[m_num_vertices] = vertex;
			m_num_vertices++;
		}

		void AddVertex(const Vertex<3> &vertex, const Point &texcoord)
		{
			D_ASSERT_MESSAGE(m_has_texture_coords , TEXT("this array does not contain texcoords"));
			m_vertices[m_num_vertices] = vertex;
			m_texture_coords[m_num_vertices] = texcoord;
			m_num_vertices++;
		}

		void AddVertex(const Vertex<3> &vertex, const Point &texcoord, const Color &color)
		{
			D_ASSERT_MESSAGE(m_has_texture_coords , TEXT("this array does not contain texcoords"));
			D_ASSERT_MESSAGE(m_has_colors , TEXT("this array does not contain colors"));
			m_vertices[m_num_vertices] = vertex;
			m_texture_coords[m_num_vertices] = texcoord;
			m_colors[m_num_vertices] = color;
			m_num_vertices++;
		}

		void AddIndex(uint32 index)
		{
			m_indices[m_num_indices] = index;
			m_num_indices++;
		}

		int32 NumIndices() const { return m_num_indices; }
		int32 NumVertices() const { return m_num_vertices; }

		void Clear()
		{
			m_num_vertices = 0;
			m_num_indices = 0;
		}

		void Lock() { }
		void Unlock() { }

		void SetLength(int32 vertices, int32 indices = 0)
		{
			m_vertices.SetLength(vertices,false);
			if(m_has_texture_coords) m_texture_coords.SetLength(vertices,false);
			if(m_has_normals) m_normals.SetLength(vertices,false);
			if(m_has_colors) m_colors.SetLength(vertices,false);
			if(m_has_indices) m_indices.SetLength(indices,false);
		}

	private:


		Array<Vertex<3> > m_vertices;
		int32 m_num_vertices;

		Array<Vertex<3> > m_normals;
		bool m_has_normals;

		Array<Point> m_texture_coords;
		bool m_has_texture_coords;

		Array<Color> m_colors;
		bool m_has_colors;

		Array<uint32> m_indices;
		bool m_has_indices;
		int32 m_num_indices;

		RenderingDevice::Primitive m_primitive;
	
	};

} // dwarf

#endif