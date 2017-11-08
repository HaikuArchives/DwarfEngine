/*
 *		The Dwarf Engine 3D Library
 *		  copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Buffer.h"
#include "Exception.h"
#include "Codec.h"
#include "Mesh.h"
#include "LwobCodec.h"
#include "IFFBuffer.h"

#include <math.h>

using namespace dwarf;

bool LwobCodec::Decode(Buffer& from, RawMesh& to) const
{
	try
	{
		IFFBuffer lwob(from);
		IFFBuffer form = lwob['FORM'];

// set up the RawMesh object
		to.m_objects.SetLength(1);
		RawMesh::object &object = to.m_objects[0];
	

// read vertices
		IFFBuffer points = form['PNTS'];
		int32 vertex_count = points.Size() / 12; // each Vertex<3> is 3 * 4 = 12 bytes
		
		object.m_vertices.SetLength(vertex_count);
		object.m_num_vertices = vertex_count;
		logger(TEXT("\tLWOB: %d vertices"), vertex_count);
	
		// read the vertices
		for (int32 i = 0; i < vertex_count; i++) ((InStream&)points) >> object.m_vertices[i];

// read polygons
		IFFBuffer polygons = form['POLS'];
		int32 polygon_count = polygons.Size() / 3;
		
		object.m_indices.SetLength(polygon_count);
		
		// read the polygons
		int32 i = 0;
		int32 read_bytes = 0;
		while (read_bytes < polygons.Size())
		{
			uint16 num_vertices; // the number of vertices in this polygon
			polygons >> num_vertices;
			switch (num_vertices)
			{
				case 3:
					uint16 index;
					polygons >> index;
					object.m_indices[i].a = index;
					polygons >> index;
					object.m_indices[i].b = index;
					polygons >> index;
					object.m_indices[i].c = index;
			
					i++;
				break;
				default: // unsupported number of vertices
				for (int32 i = 0; i < num_vertices; i++)
				{
					uint16 dummy_index; polygons >> dummy_index;
				}
				break;
			}
			uint16 material;
			polygons >> material;
			read_bytes += num_vertices*2 + 4;
		}
		object.m_num_faces = i;
		logger(TEXT("\tLWOB: %d faces"), i);

// calculate normals
		to.CalculateNormals();
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool LwobCodec::Encode(const RawMesh& from, Buffer& to) const
{
	return false;
}

bool LwobCodec::CanEncode(const RawMesh& data) const
{
	return false;
}

bool LwobCodec::CanDecode(Buffer& data) const
{
	try
	{
		IFFBuffer lwob(data);
		IFFBuffer form = lwob['FORM'];
		return true;
	}
	catch (IFFBuffer::UnknownChunk)
	{
		return false;
	}
}



namespace
{
/*
void xyztoh(float x,float y,float z,float *h) 
{ 
	if (x == 0.0 && z == 0.0) 
		*h = 0.0; 
	else
	{ 
		if (z == 0.0) 
			*h = (x < 0.0) ? D_PI/2 : -D_PI/2; 
		else if (z < 0.0) 
			*h = -atan(x / z) + D_PI; 
		else 
			*h = -atan(x / z); 
	} 
} 

void xyztohp(float x,float y,float z,float *h,float *p) 
{ 
	if (x == 0.0 && z == 0.0) { 
		*h = 0.0; 
		if (y != 0.0) 
			*p = (y < 0.0) ? -D_PI/2 : D_PI/2; 
		else 
			*p = 0.0; 
	} 
	else { 
		if (z == 0.0) 
			*h = (x < 0.0) ? D_PI/2 : -D_PI/2; 
		else if (z < 0.0) 
			*h = -atan(x / z) + PI; 
		else 
			*h = -atan(x / z); 
		x = sqrt(x * x + z * z); 
		if (x == 0.0) 
			*p = (y < 0.0) ? -D_PI/2 : D_PI/2; 
		else 
			*p = atan(y / x); 
	} 
}

Point LwobCodec::CalculateUV(const Vertex<3> &point, const Vertex<3> &texture_center, texture_axis axis, texture_mapping mapping, HTexture texture)
{
	float x = point.x - texture_center.x;
	float y = point.y - texture_center.y;
	float z = point.z - texture_center.z;

	float xTextureSize = texture->Width();
	float yTextureSize = texture->Height();
	float zTextureSize = 1.0f;
	
	float widthTiling = 1.0f;
	float heightTiling = 1.0f;

	float lon, lat, u, v, s, t;
	
	switch(mapping)
	{
	case PLANAR:
		s = (axis == X) ? z / zTextureSize + .5 : x / xTextureSize + .5; 
		t = (axis == Y) ? -z / zTextureSize + .5 : -y / yTextureSize + .5; 
		u = fract(s);
		v = fract(t); 
	break;
	case CYLINDRICAL:
		if (axis == X)
		{
			xyztoh(z,x,-y,&lon);
			t = -x / xTextureSize + .5;
		} 
		else if (axis == Y)
		{ 
			xyztoh(-x,y,z,&lon);
			t = -y / yTextureSize + .5;
		}
		else
		{
			xyztoh(-x,z,-y,&lon);
			t = -z / zTextureSize + .5;
		}
		lon = 1.0 - lon / (2*D_PI);
		if (widthTiling != 1.0)
			lon = fract(lon) * widthTiling;
		u = fract(lon);
		v = fract(t);
	break;
	case SPHERICAL: 
		if (axis == X) 
			xyztohp(z,x,-y,&lon,&lat); 
		else if (axis == Y) 
			xyztohp(-x,y,z,&lon,&lat); 
		else 
			xyztohp(-x,z,-y,&lon,&lat); 
		lon = 1.0 - lon / (2*PI); 
		lat = .5 - lat / PI; 
		if (widthTiling != 1.0) 
			lon = fract(lon) * widthTiling; 
		if (heightTiling != 1.0) 
			lat = fract(lat) * heightTiling; 
		u = fract(lon); 
		v = fract(lat); 
		break;
	}

	return Point(u,v);
}
*/

}