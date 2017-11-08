#ifndef D_IMAGETRANSLATOR_H_
#define D_IMAGETRANSLATOR_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Codec.h"
#include "Bitmap.h"



namespace dwarf
{


		class ImageTranslator : public Codec<Bitmap>
		{
			public:

			// returns true if it succeeded.
			virtual bool Decode(Buffer& from, Bitmap& to) const;
			virtual bool Encode(const Bitmap& from, Buffer& to) const;

			virtual bool CanEncode(const Bitmap& data) const;
			virtual bool CanDecode(Buffer& data) const;

		};


} // dwarf

#endif // D_IMAGETRANSLATOR_H_