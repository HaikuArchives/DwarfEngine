/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <list>
#include <queue>

#include "Defs.h"

#include "QuakeSector.h"
#include "Box.h"
#include "State.h"
#include "StringTokenizer.h"

using namespace dwarf;

QuakeSector::QuakeSector(Scene &scene,const String &filename,Directory &dir):
	Sector(scene,filename),
	m_file(&dir,filename),
	m_vertices(0),
	m_num_vertices(0),
	m_textures(0),
	m_num_textures(0),
	m_planes(0),
	m_num_planes(0),
	m_lightmaps(0),
	m_num_lightmaps(0),
	m_texinfo(0),
	m_num_texinfo(0),
	m_models(0),
	m_num_models(0),
	m_polygons(0),
	m_num_polygons(0),
	m_vislist(0),
	m_nodes(0),
	m_leafs(0),
	m_marksurfaces(0),
	m_vertex_arrays(0),
	m_lightmap_arrays(0)
{
}

void QuakeSector::Load(float time)
{
	enum
	{
		LUMP_ENTITIES = 0,
		LUMP_PLANES,
		LUMP_TEXTURES,
		LUMP_VERTICES,
		LUMP_VISIBILITY,
		LUMP_NODES,
		LUMP_TEXINFO,
		LUMP_FACES,
		LUMP_LIGHTING,
		LUMP_CLIPNODES,
		LUMP_LEAFS,
		LUMP_MARKSURFACES,
		LUMP_EDGES,
		LUMP_SURFEDGES,
		LUMP_MODELS
	};

	Buffer buf;

	m_file.Read(buf);

	dheader_s file_header;

	buf.Read(reinterpret_cast<int8*>(&file_header),sizeof(file_header));

	logger(TEXT("QuakeBSP version   : %x"),file_header.version);

	// read the vertices
	m_num_vertices = file_header.lumps[LUMP_VERTICES].size / 12;
	logger(TEXT("QuakeBSP vertices  : %d"),m_num_vertices);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_VERTICES].offset);

	m_vertices.SetLength(m_num_vertices,false);
	for(int32 i = 0; i < m_num_vertices; i++)
	{
		dynamic_cast<InStream&>(buf) >> m_vertices[i];
	}

	// read the textures
	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_TEXTURES].offset);

	buf >> m_num_textures;
	logger(TEXT("QuakeBSP textures  : %d"),m_num_textures);

	struct color_t
	{
		uint8 r,g,b;
	} palette[256] = {	0x0,0x0,0x0,0xf,0xf,0xf,0x1f,0x1f,0x1f,0x2f,0x2f,0x2f,0x3f,0x3f,0x3f,0x4b,0x4b,
						0x4b,0x5b,0x5b,0x5b,0x6b,0x6b,0x6b,0x7b,0x7b,0x7b,0x8b,0x8b,0x8b,0x9b,0x9b,0x9b,
						0xab,0xab,0xab,0xbb,0xbb,0xbb,0xcb,0xcb,0xcb,0xdb,0xdb,0xdb,0xeb,0xeb,0xeb,0xf,
						0xb,0x7,0x17,0xf,0xb,0x1f,0x17,0xb,0x27,0x1b,0xf,0x2f,0x23,0x13,0x37,0x2b,
						0x17,0x3f,0x2f,0x17,0x4b,0x37,0x1b,0x53,0x3b,0x1b,0x5b,0x43,0x1f,0x63,0x4b,0x1f,
						0x6b,0x53,0x1f,0x73,0x57,0x1f,0x7b,0x5f,0x23,0x83,0x67,0x23,0x8f,0x6f,0x23,0xb,
						0xb,0xf,0x13,0x13,0x1b,0x1b,0x1b,0x27,0x27,0x27,0x33,0x2f,0x2f,0x3f,0x37,0x37,
						0x4b,0x3f,0x3f,0x57,0x47,0x47,0x67,0x4f,0x4f,0x73,0x5b,0x5b,0x7f,0x63,0x63,0x8b,
						0x6b,0x6b,0x97,0x73,0x73,0xa3,0x7b,0x7b,0xaf,0x83,0x83,0xbb,0x8b,0x8b,0xcb,0x0,
						0x0,0x0,0x7,0x7,0x0,0xb,0xb,0x0,0x13,0x13,0x0,0x1b,0x1b,0x0,0x23,0x23,
						0x0,0x2b,0x2b,0x7,0x2f,0x2f,0x7,0x37,0x37,0x7,0x3f,0x3f,0x7,0x47,0x47,0x7,
						0x4b,0x4b,0xb,0x53,0x53,0xb,0x5b,0x5b,0xb,0x63,0x63,0xb,0x6b,0x6b,0xf,0x7,
						0x0,0x0,0xf,0x0,0x0,0x17,0x0,0x0,0x1f,0x0,0x0,0x27,0x0,0x0,0x2f,0x0,
						0x0,0x37,0x0,0x0,0x3f,0x0,0x0,0x47,0x0,0x0,0x4f,0x0,0x0,0x57,0x0,0x0,
						0x5f,0x0,0x0,0x67,0x0,0x0,0x6f,0x0,0x0,0x77,0x0,0x0,0x7f,0x0,0x0,0x13,
						0x13,0x0,0x1b,0x1b,0x0,0x23,0x23,0x0,0x2f,0x2b,0x0,0x37,0x2f,0x0,0x43,0x37,
						0x0,0x4b,0x3b,0x7,0x57,0x43,0x7,0x5f,0x47,0x7,0x6b,0x4b,0xb,0x77,0x53,0xf,
						0x83,0x57,0x13,0x8b,0x5b,0x13,0x97,0x5f,0x1b,0xa3,0x63,0x1f,0xaf,0x67,0x23,0x23,
						0x13,0x7,0x2f,0x17,0xb,0x3b,0x1f,0xf,0x4b,0x23,0x13,0x57,0x2b,0x17,0x63,0x2f,
						0x1f,0x73,0x37,0x23,0x7f,0x3b,0x2b,0x8f,0x43,0x33,0x9f,0x4f,0x33,0xaf,0x63,0x2f,
						0xbf,0x77,0x2f,0xcf,0x8f,0x2b,0xdf,0xab,0x27,0xef,0xcb,0x1f,0xff,0xf3,0x1b,0xb,
						0x7,0x0,0x1b,0x13,0x0,0x2b,0x23,0xf,0x37,0x2b,0x13,0x47,0x33,0x1b,0x53,0x37,
						0x23,0x63,0x3f,0x2b,0x6f,0x47,0x33,0x7f,0x53,0x3f,0x8b,0x5f,0x47,0x9b,0x6b,0x53,
						0xa7,0x7b,0x5f,0xb7,0x87,0x6b,0xc3,0x93,0x7b,0xd3,0xa3,0x8b,0xe3,0xb3,0x97,0xab,
						0x8b,0xa3,0x9f,0x7f,0x97,0x93,0x73,0x87,0x8b,0x67,0x7b,0x7f,0x5b,0x6f,0x77,0x53,
						0x63,0x6b,0x4b,0x57,0x5f,0x3f,0x4b,0x57,0x37,0x43,0x4b,0x2f,0x37,0x43,0x27,0x2f,
						0x37,0x1f,0x23,0x2b,0x17,0x1b,0x23,0x13,0x13,0x17,0xb,0xb,0xf,0x7,0x7,0xbb,
						0x73,0x9f,0xaf,0x6b,0x8f,0xa3,0x5f,0x83,0x97,0x57,0x77,0x8b,0x4f,0x6b,0x7f,0x4b,
						0x5f,0x73,0x43,0x53,0x6b,0x3b,0x4b,0x5f,0x33,0x3f,0x53,0x2b,0x37,0x47,0x23,0x2b,
						0x3b,0x1f,0x23,0x2f,0x17,0x1b,0x23,0x13,0x13,0x17,0xb,0xb,0xf,0x7,0x7,0xdb,
						0xc3,0xbb,0xcb,0xb3,0xa7,0xbf,0xa3,0x9b,0xaf,0x97,0x8b,0xa3,0x87,0x7b,0x97,0x7b,
						0x6f,0x87,0x6f,0x5f,0x7b,0x63,0x53,0x6b,0x57,0x47,0x5f,0x4b,0x3b,0x53,0x3f,0x33,
						0x43,0x33,0x27,0x37,0x2b,0x1f,0x27,0x1f,0x17,0x1b,0x13,0xf,0xf,0xb,0x7,0x6f,
						0x83,0x7b,0x67,0x7b,0x6f,0x5f,0x73,0x67,0x57,0x6b,0x5f,0x4f,0x63,0x57,0x47,0x5b,
						0x4f,0x3f,0x53,0x47,0x37,0x4b,0x3f,0x2f,0x43,0x37,0x2b,0x3b,0x2f,0x23,0x33,0x27,
						0x1f,0x2b,0x1f,0x17,0x23,0x17,0xf,0x1b,0x13,0xb,0x13,0xb,0x7,0xb,0x7,0xff,
						0xf3,0x1b,0xef,0xdf,0x17,0xdb,0xcb,0x13,0xcb,0xb7,0xf,0xbb,0xa7,0xf,0xab,0x97,
						0xb,0x9b,0x83,0x7,0x8b,0x73,0x7,0x7b,0x63,0x7,0x6b,0x53,0x0,0x5b,0x47,0x0,
						0x4b,0x37,0x0,0x3b,0x2b,0x0,0x2b,0x1f,0x0,0x1b,0xf,0x0,0xb,0x7,0x0,0x0,
						0x0,0xff,0xb,0xb,0xef,0x13,0x13,0xdf,0x1b,0x1b,0xcf,0x23,0x23,0xbf,0x2b,0x2b,
						0xaf,0x2f,0x2f,0x9f,0x2f,0x2f,0x8f,0x2f,0x2f,0x7f,0x2f,0x2f,0x6f,0x2f,0x2f,0x5f,
						0x2b,0x2b,0x4f,0x23,0x23,0x3f,0x1b,0x1b,0x2f,0x13,0x13,0x1f,0xb,0xb,0xf,0x2b,
						0x0,0x0,0x3b,0x0,0x0,0x4b,0x7,0x0,0x5f,0x7,0x0,0x6f,0xf,0x0,0x7f,0x17,
						0x7,0x93,0x1f,0x7,0xa3,0x27,0xb,0xb7,0x33,0xf,0xc3,0x4b,0x1b,0xcf,0x63,0x2b,
						0xdb,0x7f,0x3b,0xe3,0x97,0x4f,0xe7,0xab,0x5f,0xef,0xbf,0x77,0xf7,0xd3,0x8b,0xa7,
						0x7b,0x3b,0xb7,0x9b,0x37,0xc7,0xc3,0x37,0xe7,0xe3,0x57,0x7f,0xbf,0xff,0xab,0xe7,
						0xff,0xd7,0xff,0xff,0x67,0x0,0x0,0x8b,0x0,0x0,0xb3,0x0,0x0,0xd7,0x0,0x0,
						0xff,0x0,0x0,0xff,0xf3,0x93,0xff,0xf7,0xc7,0xff,0xff,0xff,0x9f,0x5b,0x53 };

	m_textures.SetLength(m_num_textures,false);

	for(int32 i = 0; i < m_num_textures; i++)
	{
		// read the offset
		buf.Rewind();
		buf.Seek(file_header.lumps[LUMP_TEXTURES].offset + 4 + i * 4);
		uint32 offset;
		buf >> offset;

		// read the texture header

		struct miptex_s
		{
			char		name[16];
			unsigned	width, height;
			unsigned	offsets[4];		// four mip maps stored
		} texture_header;

		buf.Rewind();
		buf.Seek(file_header.lumps[LUMP_TEXTURES].offset + offset);
		buf.Read(reinterpret_cast<int8*>(&texture_header),sizeof(miptex_s));

		logger(TEXT("QuakeBSP texture   : %s"),texture_header.name);
		logger(TEXT("QuakeBSP dimension : %dx%d"),texture_header.width,texture_header.height);

		buf.Rewind();
		buf.Seek(file_header.lumps[LUMP_TEXTURES].offset + offset + texture_header.offsets[0]);

		uint8 texture_8bit[65536];
		Array<uint8> texture;

		buf.Read(reinterpret_cast<int8*>(texture_8bit),texture_header.width * texture_header.height);
		texture.SetLength(texture_header.width * texture_header.height * 3);

		// convert the texture using hardcoded palette
		for(uint32 x = 0; x < texture_header.width * texture_header.height; x++)
		{
			texture[x*3  ] = palette[texture_8bit[x]].r;
			texture[x*3+1] = palette[texture_8bit[x]].g;
			texture[x*3+2] = palette[texture_8bit[x]].b;
		}

		m_textures[i] = HTexture(Bitmap(texture,texture_header.width,texture_header.height));

		m_textures[i]->SetWrapMode(Texture::WRAP_S_WRAP_T);
	}

	// read texinfo, this is used to calculate uv coords


	m_num_texinfo = file_header.lumps[LUMP_TEXINFO].size / sizeof(texinfo_s);
	m_texinfo.SetLength(m_num_texinfo);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_TEXINFO].offset);
	buf.Read(reinterpret_cast<int8*>(*m_texinfo),m_num_texinfo*sizeof(texinfo_s));

	// read the planes

	struct dplane_s
	{
		float normal[3];
		float dist;
		int type;		// PLANE_X - PLANE_ANYZ ?remove? trivial to regenerate
	} plane;

	m_num_planes = file_header.lumps[LUMP_PLANES].size / sizeof(dplane_s);
	m_planes.SetLength(m_num_planes,false);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_PLANES].offset);

	for(int32 i = 0; i < m_num_planes; i++)
	{
		buf.Read(reinterpret_cast<int8*>(&plane),sizeof(dplane_s));
		// we need to flip the y/z component in the normal, and neg. the dist
		m_planes[i] = Plane<3>(Vertex<3>(plane.normal[0],plane.normal[2],-plane.normal[1]),-plane.dist);
	}

	// read the edges, this is used to construct the faces

	int32 num_edges = file_header.lumps[LUMP_EDGES].size / sizeof(dedge_s);
	Array<dedge_s> edges(num_edges);
	
	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_EDGES].offset);
	buf.Read(reinterpret_cast<int8*>(*edges),num_edges * sizeof(dedge_s));

	// read the surfedges

	int32 num_surfedges = file_header.lumps[LUMP_SURFEDGES].size / sizeof(int32);
	Array<int32> surfedges(num_surfedges);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_SURFEDGES].offset);
	buf.Read(reinterpret_cast<int8*>(*surfedges),num_surfedges * sizeof(int32));

	// read the models

	m_num_models = file_header.lumps[LUMP_MODELS].size / sizeof(dmodel_s);
	m_models.SetLength(m_num_models,false);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_MODELS].offset);
	buf.Read(reinterpret_cast<int8*>(*m_models),m_num_models * sizeof(dmodel_s));

	// read the faces

	int32 num_faces = file_header.lumps[LUMP_FACES].size / sizeof(dface_s);
	Array<dface_s> faces(num_faces);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_FACES].offset);
	buf.Read(reinterpret_cast<int8*>(*faces),num_faces * sizeof(dface_s));

	// construct faces; we want the polygons, not edges

	m_num_polygons = num_faces;
	m_polygons.SetLength(num_faces,false);

	for(int32 i = 0; i < num_faces; i++)
	{
		m_polygons[i].vertices.SetLength(faces[i].numedges,false);
		m_polygons[i].uv_coords.SetLength(faces[i].numedges,false);
		m_polygons[i].lm_coords.SetLength(faces[i].numedges,false);
		m_polygons[i].texture = m_texinfo[faces[i].texinfo].miptex;

		int32 cur_edge = faces[i].firstedge;

		Point mins(999999.0f,999999.0f);
		Point maxs(-999999.0f,-999999.0f);

		for(int16 x = 0; x < faces[i].numedges; x++,cur_edge++)
		{
			int32 edge = surfedges[cur_edge];

			if(edge >= 0)
			{
				m_polygons[i].vertices[x] = edges[edge].v[0];
			}
			else
			{
				m_polygons[i].vertices[x] = edges[-edge].v[1];
			}

			Vertex<3> &vtx = m_vertices[m_polygons[i].vertices[x]];

			m_polygons[i].uv_coords[x].u() =vtx.x() * m_texinfo[faces[i].texinfo].vecs[0][0] +
											vtx.y() * m_texinfo[faces[i].texinfo].vecs[0][1] +
											vtx.z() * m_texinfo[faces[i].texinfo].vecs[0][2] + 
											m_texinfo[faces[i].texinfo].vecs[0][3];

			m_polygons[i].uv_coords[x].v() =vtx.x() * m_texinfo[faces[i].texinfo].vecs[1][0] +
											vtx.y() * m_texinfo[faces[i].texinfo].vecs[1][1] +
											vtx.z() * m_texinfo[faces[i].texinfo].vecs[1][2] + 
													m_texinfo[faces[i].texinfo].vecs[1][3];

			if(m_polygons[i].uv_coords[x].u() < mins.u()) mins.u() = m_polygons[i].uv_coords[x].u();
			if(m_polygons[i].uv_coords[x].v() < mins.v()) mins.v() = m_polygons[i].uv_coords[x].v();
			if(m_polygons[i].uv_coords[x].u() > maxs.u()) maxs.u() = m_polygons[i].uv_coords[x].u();
			if(m_polygons[i].uv_coords[x].v() > maxs.v()) maxs.v() = m_polygons[i].uv_coords[x].v();

			m_polygons[i].lm_coords[x] = m_polygons[i].uv_coords[x];

			m_polygons[i].uv_coords[x].u() /= m_textures[m_polygons[i].texture]->Width();
			m_polygons[i].uv_coords[x].v() /= m_textures[m_polygons[i].texture]->Height();
		}

		Point left_top = mins;

		mins.u() = floor(mins.u()/16);
		mins.v() = floor(mins.v()/16);
		maxs.u() = ceil(maxs.u()/16);
		maxs.v() = ceil(maxs.v()/16);

		for(int32 x = 0; x < faces[i].numedges; x++)
		{
			m_polygons[i].lm_coords[x] -= mins * 16;

			//m_polygons[i].lm_coords[x].u /= ((maxs.u - mins.u + 1)*16);
			//m_polygons[i].lm_coords[x].v /= ((maxs.v - mins.v + 1)*16);

			m_polygons[i].lm_coords[x] += Point(8,8);

			m_polygons[i].lm_coords[x].u() /= 16.0f;
			m_polygons[i].lm_coords[x].v() /= 16.0f;

			m_polygons[i].extents = (maxs - mins) + Point(1,1);
		}

	}

	// read lightmaps

	Array<uint8> lightmaps;

	lightmaps.SetLength(file_header.lumps[LUMP_LIGHTING].size);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_LIGHTING].offset);
	buf.Read(reinterpret_cast<int8*>(*lightmaps),file_header.lumps[LUMP_LIGHTING].size);

	int32 current_texture = 0; // the large lightmap texture currently being processed
	int32 needed_textures = ceil(m_num_polygons / 196); // 225 lightmaps in one texture
	int32 maps_in_current_texture = 1; // one lightmap reserved for black lightmap

	m_lightmaps.SetLength(needed_textures);

	Array<uint8> lightmap(256*256);

	for(int32 i = 0; i < 256*256; i++)
		lightmap[i] = 0;

	for(int32 i = 0; i < m_num_polygons; i++)
	{
		int32 offset = faces[i].lightofs;

		if(offset == -1)
		{
			if(i == (m_num_polygons - 1))
			{
				Bitmap bitmap(lightmap, 256, 256, Bitmap::GRAYSCALE);

				m_lightmaps[m_num_lightmaps] = HTexture(bitmap);
				m_lightmaps[m_num_lightmaps]->SetWrapMode(Texture::CLAMP_S_CLAMP_T);
				m_lightmaps[m_num_lightmaps]->SetMipmapMode(Texture::NO_MIPMAPS);

				m_num_lightmaps++;
			}
			m_polygons[i].lightmap = 0;

			for(int32 v = 0; v < m_polygons[i].vertices.Length(); v++)
			{
				m_polygons[i].lm_coords[v].u() /= 256.0f;
				m_polygons[i].lm_coords[v].v() /= 256.0f;
			}
		}
		else
		{
//			if((int32)m_polygons[i].extents.v == 17)
//				m_polygons[i].extents.v--;

			int32 x_offset = (maps_in_current_texture % 14) * 18 + 1;
			int32 y_offset = (maps_in_current_texture / 14) * 18 + 1;

			for(int32 v = 0; v < (int32)m_polygons[i].extents.v(); v++)
			{
				for(int32 u = 0; u < (int32)m_polygons[i].extents.u(); u++)
				{
					lightmap[(v + y_offset)*256 + (u + x_offset)] = lightmaps[offset];
					offset++;
				}
			}

			// copy first and last row

			for(int32 x = 0; x < (int32)m_polygons[i].extents.u(); x++)
			{
				lightmap[(y_offset-1)*256 + x + x_offset] = lightmap[y_offset*256 + x + x_offset];
				lightmap[(((int32)m_polygons[i].extents.v()) + y_offset)*256 + x + x_offset] = lightmap[(((int32)m_polygons[i].extents.v()-1) + y_offset)*256 + x + x_offset];
			}

			// copy first and last column

			for(int32 x = 0; x < (int32)m_polygons[i].extents.v(); x++)
			{
				lightmap[(y_offset + x) * 256 + x_offset - 1] = lightmap[(y_offset + x) * 256 + x_offset];
				lightmap[(y_offset + x) * 256 + x_offset + (int32)m_polygons[i].extents.u()] = lightmap[(y_offset + x) * 256 + x_offset + (int32)m_polygons[i].extents.u() - 1];
			}

			m_polygons[i].lightmap = current_texture;

//			lightmap.SetLength(16*16);

/*			Bitmap bitmap(*lightmaps + offset,(int32)m_polygons[i].extents.u,(int32)m_polygons[i].extents.v,Bitmap::GRAYSCALE);
			//Bitmap bitmap(*lightmap,(int32)m_polygons[i].extents.u,(int32)m_polygons[i].extents.v,Bitmap::GRAYSCALE);

			bitmap.SetSize(16,16,Bitmap::CROP);

			//m_polygons[i].lightmap = HTexture(Bitmap(lightmap,16,16,Bitmap::GRAYSCALE));
			m_polygons[i].lightmap = HTexture(bitmap);
			m_polygons[i].lightmap->SetWrapMode(Texture::CLAMP_S_CLAMP_T);
*/
			maps_in_current_texture++;

			// adjust lightmap coord to match size of texture
			for(int32 v = 0; v < m_polygons[i].vertices.Length(); v++)
			{
//				m_polygons[i].lm_coords[v].u *= (m_polygons[i].extents.u/256);
//				m_polygons[i].lm_coords[v].v *= (m_polygons[i].extents.v/256);
//				m_polygons[i].lm_coords[v].u += ((float)x_offset / 256.0f); // adjust lm coords to 256*256 texture size
//				m_polygons[i].lm_coords[v].v += ((float)y_offset / 256.0f);
		
				m_polygons[i].lm_coords[v].u() += (float)x_offset;
				m_polygons[i].lm_coords[v].v() += (float)y_offset;

				m_polygons[i].lm_coords[v].u() /= 256.0f;
				m_polygons[i].lm_coords[v].v() /= 256.0f;
			}

			if((maps_in_current_texture == 196) || (i == (m_num_polygons-2)))
			{
				Bitmap bitmap(lightmap, 256, 256, Bitmap::GRAYSCALE);

				m_lightmaps[current_texture] = HTexture(bitmap);
				m_lightmaps[current_texture]->SetWrapMode(Texture::CLAMP_S_CLAMP_T);
				m_lightmaps[current_texture]->SetMipmapMode(Texture::NO_MIPMAPS);
/*
				Buffer bufs(256*256);

				for(int32 b = 0; b < 256*256; b++)
				{
					bufs << lightmap[b];
				}

				String filename;
				filename.Format(TEXT("lm%d.raw"),current_texture);
				Directory dir(TEXT("../data"));
				File save_file(&dir, filename, File::WRITE, File::CREATE);
				save_file.Write(bufs);
*/
				maps_in_current_texture = 0;
				current_texture++;
				m_num_lightmaps++;
			}
		}
	}

	// read visibility data

	m_vislist.SetLength(file_header.lumps[LUMP_VISIBILITY].size,false);
	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_VISIBILITY].offset);
	buf.Read(reinterpret_cast<int8*>(*m_vislist),file_header.lumps[LUMP_VISIBILITY].size);

	// read the nodes

	m_nodes.SetLength(file_header.lumps[LUMP_NODES].size / sizeof(dnode_s));

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_NODES].offset);
	buf.Read(reinterpret_cast<int8*>(*m_nodes),m_nodes.Length() * sizeof(dnode_s));

	// read the leafs

	m_leafs.SetLength(file_header.lumps[LUMP_LEAFS].size / sizeof(dleaf_s));

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_LEAFS].offset);
	buf.Read(reinterpret_cast<int8*>(*m_leafs),file_header.lumps[LUMP_LEAFS].size);

	// read the marksurfaces, used in leafs to index faces

	m_marksurfaces.SetLength(file_header.lumps[LUMP_MARKSURFACES].size/2);

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_MARKSURFACES].offset);
	buf.Read(reinterpret_cast<int8*>(*m_marksurfaces),file_header.lumps[LUMP_MARKSURFACES].size);

	// setup Vertex<3> arrays

	m_vertex_arrays.SetLength(m_num_textures);
	m_lightmap_arrays.SetLength(needed_textures);

	for(int32 i = 0; i < m_num_textures; i++)
	{
		m_vertex_arrays[i].HasTextureCoords(true);
		m_vertex_arrays[i].SetLength(10000,5000);
	}

	for(int32 i = 0; i < m_num_lightmaps; i++)
	{
		m_lightmap_arrays[i].HasTextureCoords(true);
		m_lightmap_arrays[i].SetLength(10000,5000);
	}

	// flip vertices

	for(int32 i = 0; i < m_num_vertices; i++)
	{
		float a = m_vertices[i].z();
		m_vertices[i].z() = -m_vertices[i].y();
		m_vertices[i].y() = a;
	}

	// it would be cool to add some entities here, like torches

	buf.Rewind();
	buf.Seek(file_header.lumps[LUMP_ENTITIES].offset);

	String str((char*)buf.Data() + buf.Position());

	StringTokenizer tokenizer(str,TEXT(" \t\n"),TEXT("\""));

	int32 tokenizer_state = 0;

	enum
	{
		BEGIN_STATE = 0,
		CLASSNAME_STATE = 1,
		TORCH_STATE = 2,
		ORIGIN_STATE = 3,
		FINISH_STATE = 4,
		ORIGIN2_STATE = 5,
	};

	Directory data(TEXT("../data"));
	HMesh torch_mesh = new Mesh(&data, TEXT("billboard.3ds"));

	HShader torch_shader(TEXT("anim"));

	int32 entities = 0;

	Vertex<3> pos;
	bool in_torch = false;

	for(int32 i = 0; i < tokenizer.NumTokens(); i++)
	{
		if(tokenizer[i] == TEXT("classname") && tokenizer[i+1] == TEXT("light_torch_small_walltorch"))
		{
			in_torch = true;
			i+=1;
		}
		else if(tokenizer[i] == TEXT("origin"))
		{
			pos.x() = tokenizer[i+1].ToFloat();
			pos.y() = tokenizer[i+3].ToFloat();
			pos.z() = -tokenizer[i+2].ToFloat();
			i+=3;
		}
		else if(tokenizer[i] == TEXT("}") && in_torch)
		{
			AddEntity(GetScene().AddEntity(new TorchEntity(pos, torch_mesh, torch_shader)));
			entities++;
			in_torch = false;
		}
	}

	logger(TEXT("%d entities in quakesector"),entities);
}

void QuakeSector::DrawSector(RenderingDevice &device, const Camera &camera, int32 depth) const
{
//	Sector::DrawSector(device, camera, depth);

	for (std::list<Portal*>::const_iterator i = Portals().begin(); i != Portals().end(); i++)
	{
		(*i)->Draw(device, camera, depth+1);
	}

	// clear Vertex<3> arrays
	for(int32 i = 0; i < m_num_textures; i++)
	{
		m_vertex_arrays[i].Clear();
	}

	for(int32 i = 0; i < m_num_lightmaps; i++)
	{
		m_lightmap_arrays[i].Clear();
	}

	for(int32 i = 0; i < 9000; i++)
	{
		const_cast<uint8*>(m_draw_table)[i] = 0;
	}

	/*m_vertex_arrays[0].AddVertex(Vertex<3>(176, 224, 855), Point(0, 1));
	m_vertex_arrays[0].AddVertex(Vertex<3>(224, 224, 855), Point(1, 1));
	m_vertex_arrays[0].AddVertex(Vertex<3>(224, 128, 855), Point(1, 0));
	m_vertex_arrays[0].AddVertex(Vertex<3>(176, 128, 855), Point(0, 0));

	m_vertex_arrays[0].AddIndex(0);
	m_vertex_arrays[0].AddIndex(1);
	m_vertex_arrays[0].AddIndex(2);
	m_vertex_arrays[0].AddIndex(0);
	m_vertex_arrays[0].AddIndex(2);
	m_vertex_arrays[0].AddIndex(3);*/

	int32 v = m_leafs[FindLeaf(camera.VirtualPosition(), &m_nodes[m_models[0].headnode[0]])].visofs;

	int32 leafs_drawn = 0;

	if(v == -1)
		return;

	uint8 has_drawn[5000];

	for(int32 i = 0; i < 5000; i++)
	{
		has_drawn[i] = 0;
	}

	for (int32 l = 1; l < m_models[0].visleafs; v++)
	{
		if (m_vislist[v] == 0)           // value 0, leaves invisible
		{
			l += m_vislist[v + 1] << 3;    // skip some leaves
			v++;
		}
		else                          // tag 8 leaves, if needed
		{                           // examine bits right to left
			for (uint8 bit = 1; bit != 0; bit = bit * 2, l++)
			{
				if (m_vislist[v] & bit)
					if(l < m_leafs.Length())
					{
						D_ASSERT(has_drawn[l] == 0);
						has_drawn[l] = 1;
						Vertex<3> minv(m_leafs[l].mins[0], m_leafs[l].mins[2], -m_leafs[l].mins[1]);
						Vertex<3> maxv(m_leafs[l].maxs[0], m_leafs[l].maxs[2], -m_leafs[l].maxs[1]);
						Box bbox(minv, maxv);

						if(camera.ViewFrustum().Contains(bbox))
							DrawLeaf(&m_leafs[l]);
						leafs_drawn++;
					}
			}
		}
	}


//	return;

	// draw Vertex<3> arrays

/*	for(int32 i = 0; i < 500; i++)
	{
		logger(TEXT("drawtable %d: %d"),i,m_draw_table[i]);
	}
*/
//	return;

	State state;

	// lightmap array

//	state.Enable(State::BLEND | State::TEXTURE_2D | State::CULL_FACE | State::DEPTH_TEST);
	state.Enable(State::TEXTURE_2D | /*State::CULL_FACE | */State::DEPTH_TEST);
	state.Disable(State::BLEND | State::LIGHTING | State::CULL_FACE);
//	state.Enable(State::BLEND);
	state.SetCullMode(RenderingDevice::FRONT);
	state.SetDepthTestFunction(RenderingDevice::LESS_OR_EQUAL);
	state.SetColor(Color(1,1,1));
	//state.SetBlendMode(RenderingDevice::DST_COLOR, RenderingDevice::SRC_COLOR);
	state.SetBlendMode(RenderingDevice::ONE, RenderingDevice::ONE);

	device.SetStates(state);

	D_ASSERT(device.GetTextureMatrix() == IdentityMatrix());

/*	for(int32 i = 0; i < m_lightmap_arrays[4].NumIndices(); i++)
	{
		logger(TEXT("lm index: %d"),m_lightmap_arrays[4].Indices()[i]);
	}
*/
	for(int32 i = 0; i < m_num_lightmaps; i++)
	{
//		device.SetColor(Color(((float)i/m_num_lightmaps),1-((float)i/m_num_lightmaps),0));
		if(m_lightmap_arrays[i].NumVertices() > 0)
		{
			if(m_lightmaps[i].IsValid())
			{
				device.UseTexture(0,m_lightmaps[i]);
				device.Draw(m_lightmap_arrays[i]);
			}
		}
	}

	// texture array

	state.Enable(State::BLEND);
	state.Disable(State::LIGHTING);
	state.Enable(State::TEXTURE_2D/* | State::CULL_FACE*/ | State::DEPTH_TEST);
	state.SetCullMode(RenderingDevice::FRONT);
	state.SetDepthTestFunction(RenderingDevice::LESS_OR_EQUAL);
	state.SetColor(Color(1,1,1));

	state.SetBlendMode(RenderingDevice::DST_COLOR, RenderingDevice::SRC_COLOR);

	device.SetStates(state);

	for(int32 i = 0; i < m_num_textures; i++)
	{
		if(m_vertex_arrays[i].NumVertices() > 0)
		{
			device.UseTexture(0,m_textures[i]);
			device.Draw(m_vertex_arrays[i]);
		}
	}

	// draw the entities

	for (std::list<int32>::const_iterator i = Entities().begin(); i != Entities().end(); i++)
	{
		Entity &entity = GetScene().GetEntity(*i);
		entity.Invalidate(device, camera);
	}


//		logger(TEXT("leafs drawn: %d"),leafs_drawn);

//		DrawLeaf(device,&m_leafs[i]);
//	}
//	DrawLeaf(device,&m_leafs[FindLeaf( camera.Position(), &(m_nodes[m_models[0].headnode[0]]) )]);


/*	device.DisableState(RenderingDevice::DEPTH_TEST);
	device.DisableState(RenderingDevice::BLEND);
	device.DisableState(RenderingDevice::CULL_FACE);*/
	return;

	device.DisableState(RenderingDevice::BLEND);
	device.EnableState(RenderingDevice::TEXTURE_2D);
	device.SetColor(Color(1,1,1));
	device.EnableState(RenderingDevice::CULL_FACE);
	device.EnableState(RenderingDevice::DEPTH_TEST);
	device.SetDepthFunc(RenderingDevice::LESS_OR_EQUAL);

	device.SetCullMode(RenderingDevice::FRONT);

	for(int32 i = 0; i < m_num_polygons; i++)
	{
		device.UseTexture(0,m_textures[m_polygons[i].texture]);

/*		device.Begin(RenderingDevice::TRIANGLE_FAN);
		for(int32 v = 0; v < m_polygons[i].vertices.Length(); v++)
		{
			device.TexCoord(m_polygons[i].uv_coords[v]);
			device.AddVertex(m_vertices[m_polygons[i].vertices[v]]);
		}
		device.End();
*/
		device.Begin(RenderingDevice::TRIANGLES);
		for(int32 v = 1,a = 2; a < m_polygons[i].vertices.Length(); v++,a++)
		{
			device.TexCoord(m_polygons[i].uv_coords[0]);
			device.AddVertex(m_vertices[m_polygons[i].vertices[0]]);
			device.TexCoord(m_polygons[i].uv_coords[v]);
			device.AddVertex(m_vertices[m_polygons[i].vertices[v]]);
			device.TexCoord(m_polygons[i].uv_coords[a]);
			device.AddVertex(m_vertices[m_polygons[i].vertices[a]]);

		}
		device.End();

	}

/*	device.EnableState(RenderingDevice::BLEND);
	device.SetDepthFunc(RenderingDevice::EQUAL);
	device.SetBlendMode(RenderingDevice::DST_COLOR,RenderingDevice::SRC_COLOR); // src * dst * 2

	for(int32 i = 0; i < m_num_polygons; i++)
	{
		if(m_polygons[i].lightmap.IsValid())
		{
			device.SetColor(Color(1,1,1));
			device.UseTexture(0,m_polygons[i].lightmap);
*/
/*			device.Begin(RenderingDevice::TRIANGLE_FAN);
			for(int32 v = 0; v < m_polygons[i].vertices.Length(); v++)
			{
				device.TexCoord(m_polygons[i].lm_coords[v]);
				device.AddVertex(m_vertices[m_polygons[i].vertices[v]]);
			}
			device.End();
*/
/*			device.Begin(RenderingDevice::TRIANGLES);
			int32 a;
			for(int32 v = 1,a = 2; a < m_polygons[i].vertices.Length(); v++,a++)
			{
				device.TexCoord(m_polygons[i].uv_coords[0]);
				device.AddVertex(m_vertices[m_polygons[i].vertices[0]]);
				device.TexCoord(m_polygons[i].uv_coords[v]);
				device.AddVertex(m_vertices[m_polygons[i].vertices[v]]);
				device.TexCoord(m_polygons[i].uv_coords[a]);
				device.AddVertex(m_vertices[m_polygons[i].vertices[a]]);
			}
			device.End();
		}
		else
		{
			device.SetColor(Color(0,0,0));
			device.Begin(RenderingDevice::TRIANGLE_FAN);*/
/*			for(int32 v = 0; v < m_polygons[i].vertices.Length(); v++)
			{
				device.AddVertex(m_vertices[m_polygons[i].vertices[v]]);
			}
			device.End();
*/
/*			device.Begin(RenderingDevice::TRIANGLES);
			int32 a;
			for(int32 v = 1,a = 2; a < m_polygons[i].vertices.Length(); v++,a++)
			{
				device.TexCoord(m_polygons[i].uv_coords[0]);
				device.AddVertex(m_vertices[m_polygons[i].vertices[0]]);
				device.TexCoord(m_polygons[i].uv_coords[v]);
				device.AddVertex(m_vertices[m_polygons[i].vertices[v]]);
				device.TexCoord(m_polygons[i].uv_coords[a]);
				device.AddVertex(m_vertices[m_polygons[i].vertices[a]]);
			}
			device.End();
		}
	}*/
}

uint32 QuakeSector::FindLeaf(const Vertex<3> &position,dnode_s *root) const
{
	dnode_s *node = root;

	while(1)
	{
		if(m_planes[node->planenum].IsInside(position))
		{
			if(node->children[0] < 0)
				return (-1 - node->children[0]);
			node = &m_nodes[node->children[0]];
		}
		else
		{
			if(node->children[1] < 0)
				return (-1 - node->children[1]);
			node = &m_nodes[node->children[1]];
		}
	}
}

void QuakeSector::DrawLeaf(dleaf_s *leaf) const
{
	uint8 has_drawn[5000];

	for(int32 i = 0; i < 5000; i++)
	{
		has_drawn[i] = 0;
	}
	
	for(uint16 i = 0; i < leaf->nummarksurfaces; i++)
	{
		int32 p = m_marksurfaces[i + leaf->firstmarksurface];
		uint16 texture = m_polygons[p].texture;
		uint32 first_index = m_vertex_arrays[texture].NumVertices();

		VertexArray &current_array = m_vertex_arrays[texture];

		// lightmap array

		//D_ASSERT(m_draw_table[p] == 0);

		if(!m_draw_table[p])
		{
			const_cast<uint8*>(m_draw_table)[p]++;
		
			int32 lm = 0;

			if(m_polygons[p].lightmap != -1)
				lm = m_polygons[p].lightmap;

			int32 first_index_lm = m_lightmap_arrays[lm].NumVertices();
			VertexArray &current_lm_array = m_lightmap_arrays[lm];
			Array<uint16> &current_vertices = m_polygons[p].vertices;
			Array<Point> &current_uv = m_polygons[p].uv_coords;
			Array<Point> &current_lm_uv = m_polygons[p].lm_coords;

			current_array.AddVertex(m_vertices[current_vertices[0]], current_uv[0]);
			current_array.AddVertex(m_vertices[current_vertices[1]], current_uv[1]);
			// lightmap array
			if(m_polygons[p].lightmap != -1)
			{
				current_lm_array.AddVertex(m_vertices[current_vertices[0]], current_lm_uv[0]);
				current_lm_array.AddVertex(m_vertices[current_vertices[1]], current_lm_uv[1]);
			}
		
			for(int32 v = 1, a = 2; a < m_polygons[p].vertices.Length(); v++,a++)
			{
				current_array.AddVertex(m_vertices[current_vertices[a]], current_uv[a]);
				current_array.AddIndex(first_index);
				current_array.AddIndex(first_index+v);
				current_array.AddIndex(first_index+a);
				// lightmap array
				if(m_polygons[p].lightmap != -1)
				{
					current_lm_array.AddVertex(m_vertices[current_vertices[a]], current_lm_uv[a]);
					current_lm_array.AddIndex(first_index_lm);
					current_lm_array.AddIndex(first_index_lm+v);
					current_lm_array.AddIndex(first_index_lm+a);
				}
			}
		}

		D_ASSERT(has_drawn[p] == 0);
		has_drawn[p] = 1;
	}
}

void QuakeSector::DrawLeaf(RenderingDevice &device, dleaf_s *leaf) const
{
	for(int32 i = 0; i < leaf->nummarksurfaces; i++)
	{
		int32 p = m_marksurfaces[i + leaf->firstmarksurface];

		const_cast<uint8*>(m_draw_table)[p]+=1;

		// draw texture

		device.DisableState(RenderingDevice::BLEND);
		device.EnableState(RenderingDevice::TEXTURE_2D);
		device.SetColor(Color(1,1,1));
		device.EnableState(RenderingDevice::CULL_FACE);
		device.EnableState(RenderingDevice::DEPTH_TEST);
		device.SetDepthFunc(RenderingDevice::LESS_OR_EQUAL);

		device.SetCullMode(RenderingDevice::FRONT);

		device.UseTexture(0,m_textures[m_polygons[p].texture]);

		device.Begin(RenderingDevice::TRIANGLE_FAN);
		for(int32 v = 0; v < m_polygons[p].vertices.Length(); v++)
		{
			device.TexCoord(m_polygons[p].uv_coords[v]);
			device.AddVertex(m_vertices[m_polygons[p].vertices[v]]);
		}
		device.End();

		/*device.Begin(RenderingDevice::TRIANGLES);
		int32 a;
		for(int32 v = 1,a = 2; a < m_polygons[p].vertices.Length(); v++,a++)
		{
			device.TexCoord(m_polygons[p].uv_coords[0]);
			device.AddVertex(m_vertices[m_polygons[p].vertices[0]]);
			device.TexCoord(m_polygons[p].uv_coords[v]);
			device.AddVertex(m_vertices[m_polygons[p].vertices[v]]);
			device.TexCoord(m_polygons[p].uv_coords[a]);
			device.AddVertex(m_vertices[m_polygons[p].vertices[a]]);
		}
		device.End();*/

//		return;

		// draw lightmap

		device.EnableState(RenderingDevice::BLEND);
		device.SetDepthFunc(RenderingDevice::EQUAL);
		device.SetBlendMode(RenderingDevice::DST_COLOR,RenderingDevice::SRC_COLOR); // src * dst * 2

		if(m_polygons[p].lightmap != -1)
		{
			device.SetColor(Color(1,1,1));
			device.UseTexture(0,m_lightmaps[m_polygons[p].lightmap]);

			device.Begin(RenderingDevice::TRIANGLE_FAN);
			for(int32 v = 0; v < m_polygons[p].vertices.Length(); v++)
			{
				device.TexCoord(m_polygons[p].lm_coords[v]);
				device.AddVertex(m_vertices[m_polygons[p].vertices[v]]);
			}
			device.End();
		}
		else
		{
			device.SetColor(Color(0,0,0));
			device.Begin(RenderingDevice::TRIANGLE_FAN);
			for(int32 v = 0; v < m_polygons[p].vertices.Length(); v++)
			{
				device.AddVertex(m_vertices[m_polygons[p].vertices[v]]);
			}
			device.End();
		}
	}
}

void QuakeSector::Recurse(RenderingDevice &device, dnode_s *node) const
{
	if(node->children[0] < 0)
		DrawLeaf(device, &m_leafs[-1 - node->children[0]]);
	else if(node->children[0] == 0)
		return;
	else
		Recurse(device, &m_nodes[node->children[0]]);

	if(node->children[1] < 0)
		DrawLeaf(device, &m_leafs[-1 - node->children[1]]);
	else if(node->children[1] == 0)
		return;
	else
		Recurse(device, &m_nodes[node->children[1]]);
}