#ifndef D_CODECLWOB_H_
#define D_CODECLWOB_H_

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

	class LwobCodec : public Codec<RawMesh>
	{
	public:
		// returns true if it succeeded.
		virtual bool Decode(Buffer& from, RawMesh& to) const;
		virtual bool Encode(const RawMesh& from, Buffer& to) const;

		virtual bool CanEncode(const RawMesh& data) const;
		virtual bool CanDecode(Buffer& data) const;
	private:
		enum texture_mapping
		{
			PLANAR = 0,
			CYLINDRICAL,
			SPHERICAL,
			CUBIC,
			FRONT_PROJECTION
		};
		
		enum texture_axis
		{
			X = 0,
			Y,
			Z
		};
	
		Point CalculateUV(const Vertex<3> &point, const Vertex<3> &texture_center, texture_axis axis, texture_mapping mapping, HTexture texture);
	};

} // dwarf

#endif // D_CODECLWOB_H_