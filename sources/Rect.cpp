/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"
#include "Rect.h"
#include "Buffer.h"

using namespace dwarf;


/*!
	\brief scales the rectangle exactly the same way the given rectangle is
	scaled
	\param source the reference rectangle before transformation
	\param destination the reference rectangle after transformation
	\return returns a reference to itself
*/
const Rect &Rect::Scale(const Rect &source, const Rect &destination)
{
	if (source.width)
	{
		x *= destination.width / source.width + (destination.x - source.x);
		width = width * destination.width / source.width;
	}
	else
	{
		x += destination.x - source.x;
		width = 0;
	}

	if (source.height)
	{
		y *= destination.height / source.height + (destination.y - source.y);
		height = height * destination.height / source.height;
	}
	else
	{
		y += destination.y - source.y;
		height = 0;
	}
	return *this;
}

void Rect::Restore(InStream &stream)
{
	stream >> x >> y >> width >> height;
}

void Rect::Serialize(OutStream &stream) const
{
	stream << x << y << width << height;
}
