/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Message.h"
#include "MessageFilter.h"

using namespace dwarf;

MessageFilter::MessageFilter()
{
}

MessageFilter::~MessageFilter()
{
}

bool MessageFilter::Filter(Message *message)
{
	return true;
}
