#ifndef D_IFFBUFFER_H
#define D_IFFBUFFER_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "Buffer.h"
#include "InStream.h"

namespace dwarf
{
	class IFFBuffer : public InStream
	{
	public:

		class UnknownChunk : public Exception
		{
		public:
			UnknownChunk(const String &message, const String &source):
				Exception(message, source, TEXT("IFFBuffer::UnknownChunk")) {};
		};


		IFFBuffer(Buffer &buf):
			m_buffer(buf),
			m_level(0),
			m_size(buf.Size()),
			m_start_position(0),
			m_read_position(0)
		{}

		IFFBuffer(const IFFBuffer &other):
			m_buffer(other.m_buffer),
			m_level(other.m_level),
			m_size(other.m_size),
			m_start_position(other.m_start_position),
			m_read_position(other.m_read_position),
			m_id(other.m_id)
		{}

		IFFBuffer Chunk(uint32 chunk_id) const;
		bool HasChunk(uint32 chunk_id) const;

		void Rewind() { m_read_position = 0; }
		int32 Size() const { return m_size; }
		int32 Level() const { return m_level; }

		virtual InStream &operator >>(uint8 &val);
		virtual InStream &operator >>(uint16 &val);
		virtual InStream &operator >>(uint32 &val);
		virtual InStream &operator >>(uint64 &val);

		virtual InStream &operator >>(int8 &val);
		virtual InStream &operator >>(int16 &val);
		virtual InStream &operator >>(int32 &val);
		virtual InStream &operator >>(int64 &val);
		virtual InStream &operator >>(float &val);
		virtual InStream &operator >>(double &val);
		virtual InStream &operator >>(bool &val);
		
		virtual int32 Read(int8 *buffer, int32 size) throw ();
		uint32 Id() { return m_id; }

		IFFBuffer operator[] (uint32 id) const;

	private:
		IFFBuffer(const IFFBuffer &other,uint32 size,uint32 start_pos,uint32 level, uint32 id):
			m_size(size),
			m_level(level),
			m_start_position(start_pos),
			m_read_position(0),
			m_buffer(other.m_buffer),
			m_id(id)
		{}

		int32	m_start_position;
		int32	m_read_position;		// m_start_position + k
		int32	m_size;
		int32	m_level;
		uint32	m_id; // if this is a form-chunk, it has an id.
		Buffer	&m_buffer;
	};

	/* Operators */

	inline IFFBuffer IFFBuffer::operator [](uint32 id) const
	{
		return Chunk(id);
	}

} // dwarf

#endif