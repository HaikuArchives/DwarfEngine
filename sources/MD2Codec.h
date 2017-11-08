#ifndef D_MD2CODEC_H_
#define D_MD2CODEC_H_

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
	class MD2Codec : public Codec<RawMesh>
	{
	public:
		virtual bool Decode(Buffer& from, RawMesh& to) const;
		virtual bool Encode(const RawMesh& from, Buffer& to) const;

		virtual bool CanEncode(const RawMesh& data) const;
		virtual bool CanDecode(Buffer& data) const;
	};

} // dwarf

#endif