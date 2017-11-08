#ifndef D_OUTSTREAM_H_
#define D_OUTSTREAM_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Vertex.h"

namespace dwarf
{
	class Serializable;
	class Quaternion;
	
	/*!
		\brief abstract baseclass defining the interface for an output data stream
	*/
	class OutStream
	{
	public:

		virtual OutStream &operator<<(const dchar *string) = 0;

		virtual OutStream &operator<<(const uint8 val) = 0;
		virtual OutStream &operator<<(const uint16 val) = 0;
		virtual OutStream &operator<<(const uint32 val) = 0;
		virtual OutStream &operator<<(const uint64 val) = 0;

		virtual OutStream &operator<<(const int8 val) = 0;
		virtual OutStream &operator<<(const int16 val) = 0;
		virtual OutStream &operator<<(const int32 val) = 0;
		virtual OutStream &operator<<(const int64 val) = 0;

		virtual OutStream &operator<<(const float val) = 0;
		virtual OutStream &operator<<(const double val) = 0;
		virtual OutStream &operator<<(const bool val) = 0;
		
		OutStream &operator<<(const Serializable &object);
		OutStream &operator<<(const Quaternion &quaternion);
//		template<int N> OutStream &operator<<(const Vertex<N> &v);
		OutStream &operator<<(const Vertex<2> &v);
		OutStream &operator<<(const Vertex<3> &v);
		
		virtual void Flush() = 0;
	};
}

#include "Serializable.h"
#include "Quaternion.h"

namespace dwarf
{

	inline OutStream &OutStream::operator<<(const Serializable &object)
	{
		object.Serialize(*this);
		return *this;
	}
/*
	template<int N> inline OutStream &OutStream::operator<<(const Vertex<N> &v)
	{
		for (int32 i = 0; i < N; i++) *this << v[i];
		return *this;
	}
*/
	inline OutStream &OutStream::operator<<(const Vertex<2> &v)
	{
		for (int32 i = 0; i < 2; i++) *this << v[i];
		return *this;
	}
	inline OutStream &OutStream::operator<<(const Vertex<3> &v)
	{
		for (int32 i = 0; i < 3; i++) *this << v[i];
		return *this;
	}





	inline OutStream &OutStream::operator<<(const Quaternion &quaternion)
	{
		*this << quaternion.m_v << quaternion.m_s;
		return *this;
	}

} // dwarf

#endif // D_OUTSTREAM_H_
