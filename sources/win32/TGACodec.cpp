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



#include "TGACodec.h"

using namespace dwarf;

#pragma pack( 1 )
struct tgaheader_t
{
    uint8  idlen;
    uint8  cmtype;
    uint8  imgtype;
	uint16 cmorig;
	uint16 cmlen;
	uint8  cmbitsize;
    uint16 xorig, yorig;
    uint16 width, height;
    uint8  pixsize;
    uint8  imgdesc;
};
#pragma pack()

bool TGACodec::Decode(Buffer& from, Bitmap& to) const
{
	return LoadTGA((uint8*)from.Data(), from.Size(), to);
}

bool TGACodec::Encode(const Bitmap& from, Buffer& to) const
{
	return false;
}

bool TGACodec::CanEncode(const Bitmap& data) const
{
	return false;
}

bool TGACodec::CanDecode(Buffer& data) const
{
	if(data.Size() < sizeof(tgaheader_t))
		return false;

    tgaheader_t *tgahead = (tgaheader_t*)data.Data();
	
	switch(tgahead->imgtype)
	{
	case 2: case 10: break;
	case 1: case 9: logger(Log::HIGH, TEXT("Only type 2 and 10 tga's are supported."));
	default:
		logger(Log::HIGH, TEXT("\tTGACodec: Failed"));
		return false;
	}

	switch(tgahead->pixsize)
	{
	case 24: case 32: break;
	case 8: case 16: logger(Log::HIGH, TEXT("Only 24 and 32 bit TGA's are supported."));
	default:
		logger(Log::HIGH, TEXT("\tTGACodec: Failed"));
		return false;
	}

	logger(Log::HIGH, TEXT("\tTGACodec: OK"));
	return true;
}

bool TGACodec::LoadTGA(uint8 *from, int32 size, Bitmap &to) const
{
	// pointer to file header
    tgaheader_t *tgahead = (tgaheader_t*)from;
    uint8	*img = 0,			// img will point to the decoded image
			*tgacur = 0,		// tgacur is the current position in the file		
			*tgaend = from + size;// tgaend points to the end of the file
    int32	depth = 0;			// will contain the bitdepth of the image
	Bitmap::ColorSpace space;

	switch(tgahead->pixsize)
	{
	case 24:
		depth = 3;
		space = Bitmap::RGB;
		break;
	case 32:
		depth = 4;
		space = Bitmap::RGBA;
		break;
	case 8: /* should we treat 8 bit tgas as grayscales or as RGB's? */
	case 16:
	default:
		logger(TEXT("Fatal Error: Nonreachable code has been reached."));
		return false;
	}
  
    int32 len = tgahead->width * tgahead->height * depth;
	
	to.SetSize(tgahead->width, tgahead->height, space);
	img = to.Data();

    tgacur = from + sizeof(tgaheader_t) + tgahead->idlen;
    
	switch(tgahead->imgtype)
	{
	case 10:
		{
			int i, j, packetlen;
			uint8 packethead;
			uint8 *c = img, *end = img + len;
			uint8 rlc[4];
		
			while (c < end)
			{
				packethead = *tgacur;
				if (++tgacur > tgaend)
				{
					logger(TEXT("Unexpected end of TGA file."));
					return false;
				}

				if (packethead & 0x80)
				{
					// Run-length packet
					packetlen = (packethead & 0x7f) + 1;
					memcpy(rlc, tgacur, depth);
					if ((tgacur += depth) > tgaend)
					{
						logger(TEXT("Unexpected end of TGA file."));
						return false;
					}

					for (j=0; j < packetlen; ++j)
						for(i=0; i < depth; ++i)
							*c++ = rlc[i];
				}
				else
				{
					// Raw data packet
					packetlen = packethead + 1;
					memcpy(c, tgacur, depth * packetlen);
					if ((tgacur += depth * packetlen) > tgaend)
					{
						logger(TEXT("Unexpected end of TGA file."));
						return false;
					}

					c += packetlen * depth;
				}
			}

			int linelen = tgahead->width * depth;
			for(i=0; i < tgahead->height; ++i)
				memcpy(&img[i * linelen], &tgacur[i * linelen], linelen);
		}
		break;
	case 2:
		{
			if (tgaend - tgacur + 1 < len)
			{
				logger(TEXT("Bad TGA image data length."));
				return false;
			}

			int linelen = tgahead->width * depth;
			for(int i=0; i < tgahead->height; ++i)
				memcpy(&img[i * linelen], &tgacur[i * linelen], linelen);
		}    
		break;
	default:
		logger(TEXT("Fatal Error: Nonreachable code has been reached."));
		break;
	}

    /* Exchange B and R to get RGBA ordering */
    {
		int i;
		uint8 temp;

		for (i=0; i < len; i += depth)
		{
		    temp = img[i];
			img[i] = img[i+2];
			img[i+2] = temp;
		}
    }

	return true;
}


