#ifndef D_QUAKESECTOR_H
#define D_QUAKESECTOR_H

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
#include "VertexArray.h"

namespace dwarf
{
	class TorchEntity : public Entity
	{
	public:
		TorchEntity(const Vertex<3> &position,const HMesh &mesh, const HShader shader):
			Entity(position)
		{
			m_mesh = mesh;
			m_mesh->SetShader(shader);
		}

		virtual void Draw(RenderingDevice &device, const Camera &camera) const
		{
			/*State state;

			state.Disable(State::CULL_FACE);
			state.Enable(State::DEPTH_TEST);
			state.SetColor(Color(1,0,0));

			device.SetStates(state);

			device.Begin(RenderingDevice::QUADS);
			device.AddVertex(Vertex<3>(-5,-5,0));
			device.AddVertex(Vertex<3>(5,-5,0));
			device.AddVertex(Vertex<3>(5,5,0));
			device.AddVertex(Vertex<3>(-5,5,0));
			device.End();
			*/

			device.Draw(m_mesh);
		}

	private:
		HMesh m_mesh;
	};

	class QuakeSector : public Sector
	{
	public:
		QuakeSector(Scene &scene,const String &filename,Directory &dir);

		virtual void DrawSector(RenderingDevice &device, const Camera &frustum, int32 depth) const;

		virtual void Load(float time);

	private:
	
		uint8 m_draw_table[9000];

		File m_file;
		Array<Vertex<3> > m_vertices;
		int32 m_num_vertices;
		Array<HTexture> m_textures;
		int32 m_num_textures;
		Array<Plane<3> > m_planes;
		int32 m_num_planes;
		Array<HTexture> m_lightmaps;
		int32 m_num_lightmaps;

		struct lump_s
		{
			int offset;
			int size;
		};

		struct dheader_s
		{
			int version; // Model version, must be 0x17 (23)
			lump_s lumps[15]; // 15 magic number = number of lumps in bspfile
		};

		struct texinfo_s
		{
			float vecs[2][4];		// [s/t][xyz offset]
			int	miptex;
			int	flags;
		};

		struct dface_s
		{
			short		planenum;
			short		side;

			int			firstedge;		// we must support > 64k edges
			short		numedges;	
			short		texinfo;

			// lighting info
			int8		styles[4]; // MAXLIGHTMAPS = 4
			int			lightofs;		// start of [numstyles*surfsize] samples
		};

		struct dedge_s
		{
			unsigned short v[2];		// vertex numbers
		};

		struct dmodel_s
		{
			float		mins[3], maxs[3];
			float		origin[3];
			int			headnode[4];			// MAX_MAP_HULLS = 4
			int			visleafs;				// not including the solid leaf 0
			int			firstface, numfaces;
		};

		struct dnode_s
		{
			int			planenum;
			short		children[2];	// negative numbers are -(leafs+1), not nodes
			short		mins[3];		// for sphere culling
			short		maxs[3];
			unsigned short	firstface;
			unsigned short	numfaces;	// counting both sides
		};

		struct dleaf_s
		{
			int			contents;
			int			visofs;				// -1 = no visibility info

			short		mins[3];			// for frustum culling
			short		maxs[3];

			unsigned short		firstmarksurface;
			unsigned short		nummarksurfaces;

			int8		ambient_level[4]; // NUM_AMBIENTS = 4
		};

		class Polygon
		{
		public:
			Polygon():
				vertices(0),
				uv_coords(0),
				lm_coords(0),
				extents(0,0)
			{
			}

			Array<uint16> vertices;
			Array<Point> uv_coords;
			Array<Point> lm_coords;
			Point extents;
			uint16 texture;
			//HTexture lightmap;
			int32 lightmap; // this in an index into m_lightmaps
		};

		Array<texinfo_s> m_texinfo;
		int32 m_num_texinfo;
		Array<dmodel_s> m_models;
		int32 m_num_models;
		Array<Polygon> m_polygons;
		int32 m_num_polygons;
		Array<uint8> m_vislist;
		Array<dnode_s> m_nodes;
		Array<dleaf_s> m_leafs;
		Array<uint16> m_marksurfaces;
		// vertex arrays
		Array<VertexArray> m_vertex_arrays;
		Array<VertexArray> m_lightmap_arrays;

		uint32 FindLeaf(const Vertex<3> &position,dnode_s *root) const;
		void DrawLeaf(RenderingDevice &device, dleaf_s *lef) const;
		void DrawLeaf(dleaf_s *leaf) const;
		void Recurse(RenderingDevice &device, dnode_s *root) const;
	};

} // dwarf

#endif