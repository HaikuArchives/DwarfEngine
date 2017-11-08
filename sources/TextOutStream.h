#ifndef D_TEXT_OUTSTREAM_H_
#define D_TEXT_OUTSTREAM_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "String.h"
#include "Quaternion.h"
#include "Vertex.h"
#include "Serializable.h"

namespace dwarf
{

	class OutStream;
	class Serializable;

	/*!
		\brief abstract baseclass defining the interface for an output data stream
	*/
	class TextOutStream
	{
	public:
		virtual TextOutStream &operator<<(const dchar *string) = 0;
		virtual TextOutStream &operator<<(const String &string);
		virtual TextOutStream &operator<<(const int32 val);
		virtual TextOutStream &operator<<(const float val);
	};

} // dwarf

#endif // D_TEXT_OUTSTREAM_H_
