#ifndef D_SERIALIZABLE_H_
#define D_SERIALIZABLE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

namespace dwarf
{

		class OutStream;
		class InStream;

		/*!
			When you serialize an object, you record it's states into a Buffer.
			\brief An abstract interface class for serializable objects
		*/
 		class Serializable
		{
			public:


			/*!
				If you implement your own class to derive from this one, it's vitally important
				that you only read from the buffer, nothing else.
				\brief restores all states of the serializable object from the given buffer.
				\param buffer the buffer where the object's states have been stored (from
				a previous call to Serialize())
				\sa Serialize()
			*/
			virtual void Restore(InStream &stream) = 0;
			
			/*!
				If you implement your own class to derive from this one, it's vitally important
				that you only write to the buffer, nothing else.
				\brief stores all states of the serializable object into the given buffer.
				\param buffer the buffer to write the object's content to
			*/
			virtual void Serialize(OutStream &buffer) const = 0;
	
	
		};


} // dwarf

#endif // D_SERIALIZABLE_H_
