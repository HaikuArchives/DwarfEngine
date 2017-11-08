#ifndef D_INSTREAM_H_
#define D_INSTREAM_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Serializable.h"
#include "Vertex.h"

namespace dwarf
{

	class Quaternion;
	/*!
		\brief abstract baseclass defining the interface for an input data stream
	*/
	class InStream
	{
	public:

		virtual InStream &operator>>(uint8 &val) throw () = 0;
		virtual InStream &operator>>(uint16 &val) throw () = 0;
		virtual InStream &operator>>(uint32 &val) throw () = 0;
		virtual InStream &operator>>(uint64 &val) throw () = 0;

		virtual InStream &operator>>(bool &val) throw () = 0;
		virtual InStream &operator>>(int8 &val) throw () = 0;
		virtual InStream &operator>>(int16 &val) throw () = 0;
		virtual InStream &operator>>(int32 &val) throw () = 0;
		virtual InStream &operator>>(int64 &val) throw () = 0;

		virtual InStream &operator>>(float &val) throw () = 0;
		virtual InStream &operator>>(double &val) throw () = 0;
		
		InStream &operator>>(Serializable &object) throw ();
		InStream &operator>>(Quaternion &quaternion) throw ();
//		template<int N> InStream &operator>>(Vertex<N> &v) throw ();
		InStream &operator>>(Vertex<2> &v) throw ();
		InStream &operator>>(Vertex<3> &v) throw ();

		virtual int32 Read(int8 *buffer, int32 size) throw () = 0;
	};
}

#include "Quaternion.h"

namespace dwarf
{

	// inline definitions
		
	inline InStream &InStream::operator>>(Serializable &object) throw ()
	{
		object.Restore(*this);
		return *this;
	}
/*
	template<int N> inline InStream &InStream::operator>>(Vertex<N> &v) throw ()
	{
		for (int32 i = 0; i < N; i++) *this >> v[i];
		return *this;
	}
*/
	inline InStream &InStream::operator>>(Vertex<3> &v) throw ()
	{
		for (int32 i = 0; i < 3; i++) *this >> v[i];
		return *this;
	}

	inline InStream &InStream::operator>>(Vertex<2> &v) throw ()
	{
		for (int32 i = 0; i < 2; i++) *this >> v[i];
		return *this;
	}
	

	inline InStream &InStream::operator>>(Quaternion &quaternion) throw ()
	{
		*this >> quaternion.m_v >> quaternion.m_s;
		return *this;
	}
	
} // dwarf

#endif // D_INSTREAM_H_
