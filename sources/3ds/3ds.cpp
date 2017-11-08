/*
**************************************************************************
Author: Terry Caton
Date: January 24th, 2001
Contact: tcaton@umr.edu
Open Source. Use as you wish, but don't bitch if it breaks something. If you
use this in any project worth mentioning, let me know, I'd love to see what 
you've done. 	
	  
Here's the framework for my 3D Studio File Loader. Currently, it supports
loading the vertex/index list (of course), along with materials and hierarchy
information. This is only a framework, however; you need to supply the code
to input the data into your own buffers, as each API varies widely, as does
each persons preference of how to organize data. At each point where data is
read from the file, take that data and feed it into a buffer, or just discard
it if you wish. 

The way I did it, and the way the framework here encourages, is to save all
the data into a 3dsObject class (one for each object in the file), and then at
the end when everything is done, assemble each into it's compiled vertex array/
vertex buffer/whatever. Granted, this is my first attempt at a 3D engine, so 
there may be a few fundamental flaws here, but hey, go easy on me.

If you so desire, contact me at tcaton@umr.edu. Questions, comments, money, 
whatever. Source to my engine is also available upon request, with a working 
example of this file loader in action. Thanks, and enjoy!!


Special thanks to John Ratcliff and all the guys I worked with, for getting
me interested in all this. 


**************************************************************************
*/

#include "../Defs.h"
#include "../Exception.h"
#include "../Buffer.h"
#include "../Vector.h"
#include "../Vertex.h"
#include "../Math.h"
#include "../RawMesh.h"
#include "../Material.h"

using namespace dwarf;

#include "3ds.h"

Load3ds::Load3ds(Buffer& buffer, RawMesh* out) 
	: mSource(buffer), 
	mMesh(out), 
	mBuffer(0),
	mCurrentChunk(0),
	mTempChunk(new Chunk),
	current_object(0)
{
}


Load3ds::~Load3ds()
{
	delete mTempChunk;
}

bool Load3ds::CanRead()
{
	mCurrentChunk = new Chunk;

	ReadChunk(mCurrentChunk);

	logger(TEXT("\t3DS ID: 0x%x (should be 0x%x)"), mCurrentChunk->mID, PRIMARY);

	if (mCurrentChunk->mID != PRIMARY)
	{
		delete mCurrentChunk;
		return false;
	}
	delete mCurrentChunk;
	return true;
}


void Load3ds::Load()
{
	try
	{
		if (!mMesh)
		{
			throw Exception(TEXT("Load() called with no output mesh given."));
		}

		logger(TEXT("-- 3ds loader --"));
		mCurrentChunk = new Chunk;

		ReadChunk(mCurrentChunk);
		if (mCurrentChunk->mID != PRIMARY)
		{
			delete mCurrentChunk;
			throw Exception(TEXT("Bad PRIMARY chunk."));
		}

		// Largest possible buffer we'd need is MAX_INDICES * sizeof(triangle)
		mBuffer = new uint8[65536 * 3 * sizeof(float)];

		// Begin loading objects, by calling recursive function
		ProcessNextChunk(mCurrentChunk);

		mMesh->CalculateNormals();

		// All data read, compile them into a friendly format
		CompileObjects();

		// Clean up after everything, to prepare for next time
		CleanUp();

		logger(TEXT("-- 3ds loader --"));
	}
	catch(Exception& e)
	{
		throw Exception(TEXT("Failed to load 3DS file: ") + e.Message(), TEXT("Load3ds::Create()"));
	}
}


int32 Load3ds::CleanUp()
{
	delete [] mBuffer;
	delete mCurrentChunk;
	
/*
Add any cleanup routines you may need between calls to Load3ds::Create(),
such as deleting temporary vertex/index lists, materials, or whatever
else you choose to add
*/

	return 1;
}


int32 Load3ds::ProcessNextChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		// Read next chunk
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case VERSION:
			// Check version (must be 3 or less)
			mCurrentChunk->mBytesRead +=
			mSource.Read((int8*)mBuffer, mCurrentChunk->mLength - mCurrentChunk->mBytesRead);
			if (*(unsigned short *) mBuffer > 0x03)
				throw Exception(TEXT("Bad VERSION chunk."));
			
			logger(TEXT("\tVersion: 0x%x"), (uint32)(*(uint16 *) mBuffer));

			break;

		case EDITMATERIAL:
			// Proceed to material loading function
			mMesh->m_material = new Material;
			ProcessNextMaterialChunk(mCurrentChunk);
			break;

		case EDIT3DS:
			// Check mesh verion, then proceed to mesh loading function			
			ReadChunk(mTempChunk);

			mCurrentChunk->mBytesRead += 
			mSource.Read((int8*)mBuffer, mTempChunk->mLength - mTempChunk->mBytesRead);
			
			mCurrentChunk->mBytesRead += mTempChunk->mBytesRead;

			logger(TEXT("\tMesh version: 0x%x"), *(unsigned short *)mBuffer);

			if (mTempChunk->mID != MESHVERSION || *(unsigned short *)mBuffer > 0x03)
				throw Exception(TEXT("Bad EDIT3DS chunk."));
			ProcessNextChunk(mCurrentChunk);
			break;

		case EDITOBJECT:
			mCurrentChunk->mBytesRead += GetString((char *)mBuffer);
			// mBuffer now contains name of the object to be edited
			m_name = (char*)mBuffer; // write name of object to m_name member
			logger(TEXT("\tObject name from EDITOBJECT: %s"), *m_name);

			ProcessNextObjectChunk(mCurrentChunk);
			break;

		case EDITKEYFRAME:
			ProcessNextKeyFrameChunk(mCurrentChunk);
			break;

		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead +=
			mSource.Read((int8*)mBuffer, mCurrentChunk->mLength - mCurrentChunk->mBytesRead);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int32 Load3ds::ProcessNextObjectChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case OBJTRIMESH:
			// at this point, mBuffer will contain the name of the object being described
			m_name = (char*)mBuffer; // write name of object to m_name member
			logger(TEXT("\tObject name from OBJTRIMESH: %s"), *m_name);

			mMesh->m_objects.SetLength(mMesh->m_objects.Length()+1);
			current_object = mMesh->m_objects.Length()-1;
			
			ProcessNextObjectChunk(mCurrentChunk);
//			ComputeNormals();

			break;

		case TRIVERT:
			FillVertexBuffer(mCurrentChunk);
			break;

		case TRIFACE:
			FillIndexBuffer(mCurrentChunk);
			break;

		case TRIFACEMAT:
			// your getting a list of triangles that belong to a certain material
			SortIndicesByMaterial(mCurrentChunk);
			break;

		case TRIUV:
			FillTexCoordBuffer(mCurrentChunk);
			break;

		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead += 
			mSource.Read((int8*)mBuffer, mCurrentChunk->mLength - mCurrentChunk->mBytesRead);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int32 Load3ds::ProcessNextMaterialChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case MATNAME:
			mCurrentChunk->mBytesRead += 
			mSource.Read((int8*)mBuffer, mCurrentChunk->mLength - mCurrentChunk->mBytesRead);
			
			m_name = (char*)mBuffer; // write name of object to m_name member
			logger(TEXT("\tMATNAME: %s"), *m_name);
			break;

		case MATLUMINANCE:
			logger(TEXT("\tLuminance:"));
			ReadColorChunk(mCurrentChunk, MATLUMINANCE);
			break;

		case MATDIFFUSE:
			logger(TEXT("\tDiffuse:"));
			ReadColorChunk(mCurrentChunk, MATDIFFUSE);
			break;

		case MATSPECULAR:
			logger(TEXT("\tSpecular:"));
			ReadColorChunk(mCurrentChunk, MATSPECULAR);
			break;
		
		case MATSHININESS:
			ReadPercentChunk(mCurrentChunk, MATSHININESS);
//			ReadColorChunk(mCurrentChunk, (float *) mBuffer);
			break;
		
		case MATMAP:
			// texture map chunk, proceed
			ProcessNextMaterialChunk(mCurrentChunk);
			break;

		case MATMAPFILE:
			mCurrentChunk->mBytesRead += 
			mSource.Read((int8*)mBuffer, mCurrentChunk->mLength - mCurrentChunk->mBytesRead);

			m_name = (char*)mBuffer; // write name of object to m_name member
			m_name.ToLower();
			logger(TEXT("\tTexture: %s"), *m_name);

			// mBuffer now contains the filename of the next texture; load it if you wish

			{
//				mMesh->m_material->SetTexture(Texture::Load(m_name));
			}

			break;
		
		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead += 
			mSource.Read((int8*)mBuffer, mCurrentChunk->mLength - mCurrentChunk->mBytesRead);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int32 Load3ds::ProcessNextKeyFrameChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	int16 lCurrentID, lCurrentParentID;
	
	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case KFMESH:
			ProcessNextKeyFrameChunk(mCurrentChunk);
			break;

		case KFHEIRARCHY:
			mSource >> lCurrentID;
			mCurrentChunk->mBytesRead += sizeof(lCurrentID);

			// lCurrentID now contains the ID of the current object being described
			//  Save this if you want to support an object hierarchy
			break;

		case KFNAME:
			mCurrentChunk->mBytesRead += GetString((char *)mBuffer);
			// mBuffer now contains the name of the object whose KF info will
			//   be described

			uint32 tmp;
			mSource >> tmp; // useless, ignore
			mCurrentChunk->mBytesRead += sizeof(uint32);

			mSource >> lCurrentParentID;
			mCurrentChunk->mBytesRead += sizeof(lCurrentParentID);

			// lCurrentParentID now contains the ID of the parent of the current object 
			// being described
			logger(TEXT("\tParentID: 0x%x"), lCurrentParentID);
			break;

		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead += 
			mSource.Read((int8*)mBuffer, mCurrentChunk->mLength - mCurrentChunk->mBytesRead);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int32 Load3ds::GetString(char * aBuffer)
{
//	unsigned int lBytesRead = 0;
	int index = 0;

	int8 ch;

	mSource >> ch;

	*aBuffer = ch;

	while (*(aBuffer + index) != 0)
	{
		index++;
		mSource >> ch;
		*(aBuffer + index) = ch;
	}

	return strlen(aBuffer) + 1;
}


int32 Load3ds::ReadChunk(Chunk * aChunk)
{

	mSource >> aChunk->mID;
	mSource >> aChunk->mLength;

	aChunk->mBytesRead = sizeof(aChunk->mID) + sizeof(aChunk->mLength);

	return 1;
}


int32 Load3ds::ReadColorChunk(Chunk * aChunk, uint32 type)
{
	ReadChunk(mTempChunk);

	int32 len = mTempChunk->mLength - mTempChunk->mBytesRead;

	mTempChunk->mBytesRead +=
	mSource.Read((int8*)mBuffer, len);

	Color color(1,1,1);

	switch (mTempChunk->mID)
	{
	case RGBF:
		{
			color = Color(
				((float*)mBuffer)[0], 
				((float*)mBuffer)[1], 
				((float*)mBuffer)[2]
				);
		} break;

	case RGB24:
		{
			color.FromBytes(
				((uint8*)mBuffer)[0], 
				((uint8*)mBuffer)[1], 
				((uint8*)mBuffer)[2]
				);
		} break;

	default:
		{
			logger(TEXT("\tUnknown Color chunk: 0x%x"), mTempChunk->mID);
		} break;
	};

	switch (type)
	{
	case MATDIFFUSE:
		{
			mMesh->m_material->Diffuse() = color;
		} break;
	case MATLUMINANCE:
		{
			mMesh->m_material->Luminance() = color;
		} break;
	case MATSPECULAR:
		{
			mMesh->m_material->Specular() = color;
		} break;
	default:
		{
		} break;
	}

	logger(TEXT("(%f, %f, %f)"), color.r, color.g, color.b);

	aChunk->mBytesRead += mTempChunk->mBytesRead;
	return 1;
}


int32 Load3ds::ReadPercentChunk(Chunk * aChunk, uint32 type)
{
	ReadChunk(mTempChunk);

	mTempChunk->mBytesRead += 
	mSource.Read((int8*)mBuffer, mTempChunk->mLength - mTempChunk->mBytesRead);

	float* percent = (float*)mBuffer;

	*percent = (float) *((short int *) mBuffer) / 100.0f;

	String str_type(TEXT("Unknown"));
	if (type == MATSHININESS) str_type = TEXT("Shininess");

	logger(TEXT("\t%s: %3.1f%%"), *str_type, *percent);

	aChunk->mBytesRead += mTempChunk->mBytesRead;
	return 1;
}


int32 Load3ds::FillIndexBuffer(Chunk * aPreviousChunk)
{
	int16 lNumFaces;

	mSource >> lNumFaces; aPreviousChunk->mBytesRead += sizeof(lNumFaces);

	aPreviousChunk->mBytesRead += 
	mSource.Read((int8*)mBuffer, aPreviousChunk->mLength - aPreviousChunk->mBytesRead);

	logger(TEXT("\tFace count: %d"), lNumFaces);

	// mBuffer now contains an array of indices (unsigned short ints)
	//   Careful, the list consists of 3 vertex indices and then an edge
	//   flag (safe to ignore, probably
	//   * bit 0: CA visible, bit 1: BC visible, bit 2: AB visible

	mMesh->m_objects[current_object].m_num_faces = lNumFaces;

	mMesh->m_objects[current_object].m_indices.SetLength(lNumFaces);

	for (int i = 0; i < lNumFaces; i++)
	{
		// note that the order of the two last indices are swapped to get the trinagles
		// opengl-compatible
		mMesh->m_objects[current_object].m_indices[i].a = ((int16*)mBuffer)[i*4+0];
		mMesh->m_objects[current_object].m_indices[i].b = ((int16*)mBuffer)[i*4+2];
		mMesh->m_objects[current_object].m_indices[i].c = ((int16*)mBuffer)[i*4+1];
	}

	return 1;
}


int32 Load3ds::SortIndicesByMaterial(Chunk * aPreviousChunk)
{
	aPreviousChunk->mBytesRead += GetString((char *) mBuffer);
	// mBuffer contains the name of the material that is associated
	//  with the following triangles (set of 3 indices which index into the vertex list
	//  of the current object chunk)

	String materialName((char*)mBuffer);
	logger(TEXT("\tMaterial name: %s"), *materialName);

	uint16 lNumFaces;
	mSource >> lNumFaces;
	aPreviousChunk->mBytesRead += sizeof(lNumFaces);

	logger(TEXT("\tNumFaces: %u"), (uint32)lNumFaces);

	aPreviousChunk->mBytesRead += 
		mSource.Read((int8*)mBuffer, aPreviousChunk->mLength - aPreviousChunk->mBytesRead);

	// mBuffer now contains a list of triangles that use the material specified above

	return 1;
}


int32 Load3ds::FillTexCoordBuffer(Chunk * aPreviousChunk)
{
	int16 lNumTexCoords;
	mSource >> lNumTexCoords;
	aPreviousChunk->mBytesRead += sizeof(lNumTexCoords);

	aPreviousChunk->mBytesRead += 
	mSource.Read((int8*)mBuffer, aPreviousChunk->mLength - aPreviousChunk->mBytesRead);
	
	// mBuffer now contains a list of UV coordinates (2 floats)

	logger(TEXT("\tTexCoord count: %d"), lNumTexCoords);

	mMesh->m_objects[current_object].m_texcoords.SetLength(lNumTexCoords);

	for (int i = 0; i < lNumTexCoords; i++)
	{
		mMesh->m_objects[current_object].m_texcoords[i] = 
			Point(
			((float*)mBuffer)[i*2+0], 
			((float*)mBuffer)[i*2+1]);

//		logger(TEXT("TC: (%f, %f)"), mMesh->m_texcoords[i].u, mMesh->m_texcoords[i].v);
	}


	return 1;
}


int32 Load3ds::FillVertexBuffer(Chunk * aPreviousChunk)
{
	int16 lNumVertices;
	mSource >> lNumVertices;
	aPreviousChunk->mBytesRead += sizeof(lNumVertices);

	aPreviousChunk->mBytesRead += 
	mSource.Read((int8*)mBuffer, aPreviousChunk->mLength - aPreviousChunk->mBytesRead);

	// mBuffer now contains a list of vertex coordinates (3 floats)

	logger(TEXT("\tVertex count: %d"), lNumVertices);

	mMesh->m_objects[current_object].m_num_vertices = lNumVertices;

	mMesh->m_objects[current_object].m_vertices.SetLength(lNumVertices);
	
	for (int i = 0; i < lNumVertices; i++)
	{
		mMesh->m_objects[current_object].m_vertices[i] = 
			Vertex<3>(
			((float*)mBuffer)[i*3+0], 
			((float*)mBuffer)[i*3+2],	//! 3DS models have a different coordinate system
			((float*)mBuffer)[i*3+1]);	//!	than we do. Y and Z should switch places.
		
//		logger(TEXT("V: (%f, %f, %f)"), mMesh->m_vertices[i].x, mMesh->m_vertices[i].y, mMesh->m_vertices[i].z);
	}


	return 1;
}


int32 Load3ds::ComputeNormals()
{
/*
Compute your normals here. Quick way of doing it (no smoothing groups, though):

  for (each vertex in vertex list)
	{
	for (each triangle in triangle list)
	  {
	  if (vertex is in triangle)
	    {
		compute normal of triangle
		add to total_normal
		}
	  }
	normalize(total_normal)
	save_to_normal_list(total_normal)
	total_normal = 0
	}
*/

/*
This seemed to work well for me, though it's my first time having to compute
normals myself. One might think that normalizing the normal right after it's 
computed and added to total_normal would be best, but I've found this not to be
the case. If this is done, then small triangles have just as much influence on the 
final normal as larger triangles. My way, the model comes out looking much more 
smooth, especially if there's small flaws in the model. 
*/

	return 1;
}


int32 Load3ds::CompileObjects()
{

/*
By now, you should have a collection of objects (or a single object), with 
proper hierarchy information, vertices, indices, and materials. Now's your
time to assemble these into however you want them to send them down the 
OGL/D3D pipeline.

*/
	return 1;
}


///////////////////////////////////////////////////////////////////



Chunk::Chunk()
{
	mID = 0;
	mLength = 0;
	mBytesRead = 0;
}


Chunk::~Chunk()
{
}
