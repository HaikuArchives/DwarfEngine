/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "TextOutStream.h"

using namespace dwarf;

TextOutStream &TextOutStream::operator<<(const String &string)
{
	*this << *string;
	return *this;
}

TextOutStream &TextOutStream::operator<<(const int32 val)
{
	String str;
	str.Format(TEXT("%ld"), val);
	*this << str;
	return *this;
}

TextOutStream &TextOutStream::operator<<(const float val)
{
	String str;
	str.Format(TEXT("%f"), val);
	*this << str;
	return *this;
}

