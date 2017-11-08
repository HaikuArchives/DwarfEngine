/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <TranslationUtils.h>
#include <Bitmap.h>
#include <DataIO.h>


#include "ImageTranslator.h"
#include "Buffer.h"
#include "Exception.h"
#include "Codec.h"

using namespace dwarf;

bool ImageTranslator::Decode(Buffer& from, Bitmap& to) const
{
	BMemoryIO stream(from.Data(), from.Size());
	BBitmap *bitmap = BTranslationUtils::GetBitmap(&stream);

	if (bitmap == 0) return false;
	
	Bitmap::ColorSpace space;
	bool big_endian = false;
	
	switch (bitmap->ColorSpace())
	{
		case B_RGB32_BIG:
		case B_RGBA32_BIG:
			big_endian = true;
		case B_RGB32:
		case B_RGBA32:
			space = Bitmap::RGBA;
			break;
		default:
			space = Bitmap::GRAYSCALE;
	}
	
	to.SetSize(bitmap->Bounds().Width() + 1, bitmap->Bounds().Height() + 1, space);

	if (to.BytesPerPixel() == 4 && !big_endian)
	{
		uint8 *to_ptr = to.Data();
		uint8 *from_ptr = (uint8*)bitmap->Bits();
		for (int32 i = 0; i < to.DataSize(); i += 4)
		{
			to_ptr[0] = from_ptr[2];
			to_ptr[1] = from_ptr[1];
			to_ptr[2] = from_ptr[0];
			to_ptr[3] = from_ptr[3];

			to_ptr += 4;
			from_ptr += 4;
		}
	}
	else
	{
		memcpy(to.Data(), bitmap->Bits(), to.DataSize());
	}

	delete bitmap;
	return true;
}

bool ImageTranslator::Encode(const Bitmap& from, Buffer& to) const
{
	return false;
}

bool ImageTranslator::CanEncode(const Bitmap& data) const
{
	return false;
}

bool ImageTranslator::CanDecode(Buffer& data) const
{
	BMemoryIO stream(data.Data(), data.Size());
	BBitmap *bitmap = BTranslationUtils::GetBitmap(&stream);
	
	return bitmap != 0;
}


