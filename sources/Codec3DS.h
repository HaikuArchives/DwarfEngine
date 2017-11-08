#ifndef D_CODEC3DS_H_
#define D_CODEC3DS_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Codec.h"
#include "RawMesh.h"

namespace dwarf
{

	class Codec3DS : public Codec<RawMesh>
	{
		public:
		// returns true if it succeeded.
		virtual bool Decode(Buffer& from, RawMesh& to) const;
		virtual bool Encode(const RawMesh& from, Buffer& to) const;

		virtual bool CanEncode(const RawMesh& data) const;
		virtual bool CanDecode(Buffer& data) const;
	};

} // dwarf

#endif // D_CODEC3DS_H_