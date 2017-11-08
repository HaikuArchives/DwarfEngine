#ifndef D_MESH_H_
#define D_MESH_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Vector.h"
#include "Vertex.h"
#include "Handle.h"
#include "Material.h"
#include "ListArray.h"
#include "RawMesh.h"
#include "Shader.h"

namespace dwarf
{

	/*
		Mesh owns all arrays (it is responsible for deleting them),
		but not the material, materials should be refcounted (so should
		meshes and textures).
		\brief a mesh is a 3d model
	*/
	class Mesh;
	typedef Handle<Mesh> HMesh;

	class Mesh : public RefCnt
	{
	public:
		struct FaceIndex
		{
			uint32 a, b, c;
		};

		Mesh(FileSystem *dir, String const& name);
		~Mesh();

		HShader GetShader() const { return m_shader; }
		void SetShader(HShader shader) { m_shader = shader; }

		HMaterial GetMaterial() const { return m_material; }
		void SetMaterial(HMaterial mat) { m_material = mat; }

		FaceIndex* Indices() { return *m_indices; }
		Vertex<3>* Vertices() { return *m_vertices; }
		Vertex<3>* Normals() { return *m_normals; }
		Point* TexCoords() { return *m_tex_coords; }
		Point* LightMapCoords() { return *m_lightmap_coords; }
		Point* DetailMapCoords() { return *m_detailmap_coords; }

		FaceIndex const* Indices() const { return *m_indices; }
		Vertex<3> const* Vertices() const { return *m_vertices; }
		Vertex<3> const* Normals() const { return *m_normals; }
		Point const* TexCoords() const { return *m_tex_coords; }
		Point const* LightMapCoords() const { return *m_lightmap_coords; }
		Point const* DetailMapCoords() const { return *m_detailmap_coords; }


		int32 FaceCount() const { return m_num_faces; }
		int32 VertexCount() const { return m_num_vertices; }

		bool HasMaterial() const { return m_material.IsValid(); }
		bool HasNormals() const { return m_normals.Length() != 0; }
		bool HasTexCoords() const { return m_tex_coords.Length() != 0; }
		bool HasLightMapCoords() const { return m_lightmap_coords.Length() != 0; }
		bool HasDetailMapCoords() const { return m_detailmap_coords.Length() != 0; }

	private:
		int32 m_num_vertices;		//!	Number of vertices in mesh
		int32 m_num_faces;			//! Number of faces in mesh
		
		Array<FaceIndex> m_indices;		//! face indices into other lists
		
		Array<Vertex<3> > m_vertices;			//! Vertex<3> array
		Array<Vertex<3> > m_normals;			//! normals, needed for Vertex<3> lighting and envmapping f.ex.
		Array<Point> m_tex_coords;		//! primary texcoords
		Array<Point> m_lightmap_coords;	//! if mesh has a lightmap, it's texcoords go here
		Array<Point> m_detailmap_coords;	//! if mesh has a detailmap, it's texcoords go here

		HMaterial m_material;
		HShader m_shader;
	};
}

#endif // D_MESH_H_