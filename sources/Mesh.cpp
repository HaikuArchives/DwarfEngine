/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Math.h"
#include "File.h"
#include "Mesh.h"
#include "Codec.h"
#include "Buffer.h"
#include "Directory.h"
#include "RawMesh.h"

using namespace dwarf;
/*
		int32 m_num_vertices;		//!	Number of vertices in mesh
		int32 m_num_faces;			//! Number of faces in mesh
		
		FaceIndex* m_indices;		//! face indices into other lists
		
		Vertex<3>* m_vertices;			//! Vertex<3> array
		Vertex<3>* m_normals;			//! normals, needed for Vertex<3> lighting and envmapping f.ex.
		Point* m_tex_coords;		//! primary texcoords
		Point* m_lightmap_coords;	//! if mesh has a lightmap, it's texcoords go here
		Point* m_detailmap_coords;	//! if mesh has a detailmap, it's texcoords go here

		HMaterial m_material;

// RAWMESH

  		int32 m_num_vertices;
		int32 m_num_faces;
		int32* m_indices;	//! sizeof(m_indices) == m_num_faces * 3
		Vertex<3>* m_vertices;
		Vertex<3>* m_normals;
		Point* m_texcoords;

		String m_texture_map;
		Color m_diffuse;
		Color m_luminance;
		Color m_specular;

*/

Mesh::Mesh(FileSystem *dir, String const& name) 
	: RefCnt(),
	m_num_vertices(0),
	m_num_faces(0),
	m_vertices(0),
	m_normals(0),
	m_tex_coords(0)
{
	RawMesh tmp;

	Buffer buf;
	
	File file(dir, name);

	file.Read(buf);
	Codec<RawMesh> const* codec = Codec<RawMesh>::FindDecoder(buf);

	FileSystem* prev_dir = Texture::GetFileSystem();

	Texture::SetFileSystem(dir);

	codec->Decode(buf, tmp);
	buf.Empty();

	Texture::SetFileSystem(prev_dir);

	D_ASSERT (tmp.m_objects.Length() > 0);

	logger(TEXT("Number of objects loaded: %d"), tmp.m_objects.Length());

	m_num_faces = tmp.m_objects[0].m_num_faces;
	m_num_vertices = tmp.m_objects[0].m_num_vertices;

	m_vertices = tmp.m_objects[0].m_vertices;
	m_tex_coords = tmp.m_objects[0].m_texcoords;
	m_normals = tmp.m_objects[0].m_normals;

	m_indices.SetLength(m_num_faces);
	for (int i = 0; i < m_num_faces; i++)
	{
		m_indices[i].a = tmp.m_objects[0].m_indices[i].a;
		m_indices[i].b = tmp.m_objects[0].m_indices[i].b;
		m_indices[i].c = tmp.m_objects[0].m_indices[i].c;
	}

	m_lightmap_coords.SetLength(0);
	m_detailmap_coords.SetLength(0);

	m_material = tmp.m_material;
}

Mesh::~Mesh()
{
}

