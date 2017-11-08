/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"
#include "RawMesh.h"

using namespace dwarf;

void RawMesh::CalculateNormals()
{
	logger(TEXT("CalcNormals()"));
	for (uint32 obj = 0; obj < (uint32)m_objects.Length(); obj++)
	{
		D_ASSERT(m_objects[obj].m_normals.Length() == 0);
		
		m_objects[obj].m_normals.SetLength(m_objects[obj].m_num_vertices, false);
		logger(TEXT("Object nr. %d"), obj);
		
		uint32 tot_num_verts = (uint32)m_objects[obj].m_num_vertices;
		uint32 tot_num_faces = (uint32)m_objects[obj].m_num_faces;
		
		Vertex<3>* vertices = *m_objects[obj].m_vertices;
		Vertex<3>* normals = *m_objects[obj].m_normals;
		face_index* indices = *m_objects[obj].m_indices;
		
		for (uint32 i = 0; i < tot_num_verts; i++)
		{
			uint32 num_faces = 0;
			Vertex<3> normal(0,0,0);
			
			for (uint32 c =  0; c < tot_num_faces; c++)
			{
				if (indices[c].a == i || indices[c].b == i || indices[c].c == i)
				{
					Vertex<3> a = vertices[indices[c].a] - vertices[indices[c].c];
					Vertex<3> b = vertices[indices[c].b] - vertices[indices[c].c];
					
					normal += a.CrossProduct(b);
					
					num_faces++;
				}
			}
			
			if (num_faces) normal/=num_faces;
			
			normal.Normalize();
			
			normals[i] = normal;
		}
	}
	logger(TEXT("CalcNormals() done"));
}