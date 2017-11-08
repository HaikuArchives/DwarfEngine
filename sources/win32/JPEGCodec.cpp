/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "../Defs.h"

#include "../Buffer.h"
#include "../Exception.h"
#include "../Codec.h"
#include "../Bitmap.h"


#pragma comment(lib, "ijl15.lib")


#include "JPEGCodec.h"
#include "ijl.h"

using namespace dwarf;

static inline void d_logjpeg(IJLERR err, Log::LogLevel loglevel = Log::NORMAL)
{
	// bizarre... it returns a wide char*, but the pointer contains a char string...
	logger(loglevel, String((const char*)ijlErrorStr(err)));
}

JPEGCodec::JPEGCodec() : Codec<Bitmap>()
{
	m_quality = 80;
}

JPEGCodec::~JPEGCodec()
{
}

bool JPEGCodec::Decode(Buffer& from, Bitmap& to) const
{
	DWORD x = 0, y = 0;
	JPEG_CORE_PROPERTIES jcprops;
	IJLERR jcerr;
	ZeroMemory(&jcprops, sizeof(jcprops));
	Bitmap::ColorSpace space;
	
	if (ijlInit(&jcprops) != IJL_OK)
	{
		return false;
	}

	// Read header info...
	jcprops.JPGBytes = (uint8*)from.Data();
	jcprops.JPGSizeBytes = from.Size();

	if ((jcerr = ijlRead(&jcprops, IJL_JBUFF_READPARAMS )) != IJL_OK)
	{
		d_logjpeg(jcerr);
		ijlFree(&jcprops);
		return false;
	}

	x = jcprops.JPGWidth;
	y = jcprops.JPGHeight;

	jcprops.jprops.needframe = 1; // bugfix, from intels page

    switch(jcprops.JPGChannels)
    {
        case 1:
        jcprops.DIBColor = IJL_G;
		jcprops.JPGColor = IJL_G;
		space = Bitmap::GRAYSCALE;
		jcprops.DIBChannels = 1;
          break;

        case 3:
         jcprops.DIBColor = IJL_RGB;
		 jcprops.JPGColor = IJL_YCBCR;
		space = Bitmap::RGB;
		jcprops.DIBChannels = 3;
          break;

        default:
          // This catches everything else, but no
          // color twist will be performed by the IJL.
          jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;
          jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;
		space = Bitmap::RGB;
		jcprops.DIBChannels = 3;
          break;
        }

	to.SetSize(x, y, space);

	jcprops.DIBBytes  = to.Data();
	jcprops.DIBWidth  = x;
	jcprops.DIBHeight = -((int32)y); // should maybe be -y

	if( (jcerr = ijlRead( &jcprops, IJL_JBUFF_READWHOLEIMAGE )) != IJL_OK )
	{
		logger(TEXT("Failed to read jpeg image."));
		d_logjpeg(jcerr);
		ijlFree(&jcprops);
		return false;
	}


	if( ijlFree( &jcprops ) != IJL_OK )
		logger(TEXT("Cannot free Intel(R) JPEG library"));

	return true;
}

bool JPEGCodec::Encode(const Bitmap& from, Buffer& to) const
{
	JPEG_CORE_PROPERTIES jcprops;
	IJLERR jcerr;
	ZeroMemory(&jcprops, sizeof(jcprops));
	
	if ((jcerr = ijlInit(&jcprops)) != IJL_OK)
	{
		d_logjpeg(jcerr);
		return false;
	}

	jcprops.DIBBytes  = from.Data();
	jcprops.DIBWidth  = from.Width();
	jcprops.DIBHeight = -from.Height(); // should maybe be -y
	jcprops.DIBChannels = from.BytesPerPixel();
	jcprops.DIBPadBytes = 0;
	switch(jcprops.DIBChannels)
	{
	case 4:
		jcprops.DIBColor		= IJL_RGBA_FPX; // no jpeg support for alpha channels
        jcprops.JPGColor		= IJL_YCBCRA_FPX;
        jcprops.JPGChannels		= 4;
        jcprops.JPGSubsampling	= IJL_4114;
		break;
	case 3:
	default:
		jcprops.DIBColor		= IJL_RGB;
        jcprops.JPGColor		= IJL_YCBCR;
        jcprops.JPGChannels		= 3;
        jcprops.JPGSubsampling	= IJL_411;
		break;
	};


	// header info...
	jcprops.JPGBytes = (uint8*)to.SupplyData(from.DataSize());
	jcprops.JPGSizeBytes = from.DataSize();
	jcprops.jquality = m_quality; // set encode quality
	jcprops.JPGWidth = from.Width();
	jcprops.JPGHeight = from.Height();

	if( (jcerr = ijlWrite( &jcprops, IJL_JBUFF_WRITEWHOLEIMAGE )) != IJL_OK )
	{
		logger(TEXT("Failed to write jpeg image."));
		d_logjpeg(jcerr);
		ijlFree(&jcprops);
		return false;
	}
	to.DataWritten(jcprops.JPGSizeBytes);

	if( ijlFree( &jcprops ) != IJL_OK )
		logger(TEXT("Cannot free Intel(R) JPEG library"));

	return true;
}

bool JPEGCodec::CanEncode(const Bitmap& data) const
{
	//TODO: for now we just assume we can encode the data..

	if(data.BytesPerPixel() == 3 || data.BytesPerPixel() == 4)
	{
		logger(Log::HIGH, TEXT("\tJPG: OK"));
		return true;
	}
	logger(Log::HIGH, TEXT("\tJPG: Failed"));
	return false;
}

bool JPEGCodec::CanDecode(Buffer& data) const
{
	bool rval = false;
	JPEG_CORE_PROPERTIES jcprops;
	ZeroMemory(&jcprops, sizeof(jcprops));
	IJLERR jcerr;

	if (ijlInit(&jcprops) == IJL_OK)
	{
		// Read header info...
		jcprops.JPGBytes = (uint8*)data.Data();
		jcprops.JPGSizeBytes = data.Size();

		if ((jcerr = ijlRead(&jcprops, IJL_JBUFF_READPARAMS )) != IJL_OK)
		{
			logger(Log::HIGH, TEXT("\tJPG: Failed"));
//			d_logjpeg(jcerr, Log::HIGH);
			ijlFree(&jcprops);
			return false;
		}
		logger(Log::HIGH, TEXT("\tJPG: OK"));
		ijlFree(&jcprops);
		return true;
	}
	
	return rval;
}


