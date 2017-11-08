#ifndef D_PNG_H_
#define D_PNG_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Codec.h"

namespace dwarf
{

	class Bitmap;

	class PNGCodec : public Codec<Bitmap>
	{
	public:
		
		// returns true if it succeeded.
		virtual bool Decode(Buffer& from, Bitmap& to) const;
		virtual bool Encode(const Bitmap& from, Buffer& to) const { return false; };

		virtual bool CanEncode(const Bitmap& data) const { return false; };
		virtual bool CanDecode(Buffer& data) const;

		virtual bool IsEncoder() { return false; }
		virtual bool IsDecoder() { return true; }

	};


} // dwarf

#endif // D_PNG_H_