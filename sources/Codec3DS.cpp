/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Buffer.h"
#include "Exception.h"
#include "Codec.h"
#include "Mesh.h"
#include "Codec3DS.h"

using namespace dwarf;

#include "3ds/3ds.h"



bool Codec3DS::Decode(Buffer& from, RawMesh& to) const
{
	try
	{
		// do the magic here
		Load3ds ld(from, &to);
		ld.Load();
		return true;
	}
	catch(Exception& e)
	{
		logger(TEXT("%s"), e.Message());
		return false;
	}
}

bool Codec3DS::Encode(const RawMesh& from, Buffer& to) const
{
	return false;
}

bool Codec3DS::CanEncode(const RawMesh& data) const
{
	return false;
}

bool Codec3DS::CanDecode(Buffer& data) const
{
	Load3ds ld(data, 0);
	bool result = ld.CanRead();
	if (result)
	{
		logger(Log::HIGH, TEXT("\t3DS: OK"));
		return true;
	}
	else
	{
		logger(Log::HIGH, TEXT("\t3DS: Failed"));
		return false;
	}
}
