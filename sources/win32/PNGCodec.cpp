/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "libpng/png.h"


#include "PNGCodec.h"
#include "../Buffer.h"
#include "../Bitmap.h"

using namespace dwarf;


void read_function(png_structp a, png_bytep buffer, png_size_t size)
{
	reinterpret_cast<Buffer*>(a->io_ptr)->Read(reinterpret_cast<int8*>(buffer), size);
}


void error_function(png_structp a, png_const_charp message)
{
	logger(TEXT("png: error"));
}

bool PNGCodec::CanDecode(Buffer &data) const
{
	int8 buf[8];
	
	if (data.Read(buf, 8) != 8)
		return false;
	
	/* Check the signature starting at byte 0, and check all 8 bytes */
	return png_check_sig((uint8*)buf, 8) != 0;
}

bool PNGCodec::Decode(Buffer& data, Bitmap& to) const
{
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

	if (png_ptr == 0) return false;


	
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == 0)
	{
		png_destroy_read_struct(&png_ptr,  0, 0);
		return false;
	}
	
	png_set_error_fn(png_ptr, error_function, 0, 0);
	png_set_read_fn(png_ptr, &data, read_function);
	
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, NULL);

	Bitmap::ColorSpace space = Bitmap::RGB;
	
	if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
	{
		space = Bitmap::RGB;
	} else if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY)
	{
		space = Bitmap::GRAYSCALE;
	} else if (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		space = Bitmap::RGBA;
	} else if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		space = Bitmap::GRAYSCALE_ALPHA;
	}
	
	to.SetSize(info_ptr->width, info_ptr->height, space);
	for (int32 i = 0; i < to.Height(); i++)
		memcpy(to.Data() + i*to.Width()*to.BytesPerPixel(), info_ptr->row_pointers[to.Height() - i - 1], to.Width()*to.BytesPerPixel());

	return true;
}
