/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "IFFBuffer.h"

using namespace dwarf;

bool IFFBuffer::HasChunk(uint32 chunk_id) const
{
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position);

	while(m_buffer.Position() < (m_start_position + m_size))
	{
		uint32	id;
		int32	size;

		m_buffer >> id;

		id = FromBigEndian(id);
		if(m_level < 2)
		{
			m_buffer >> size;
			size = FromBigEndian(size);
		}
		else
		{
			uint16	temp;
			m_buffer >> temp;
			temp = FromBigEndian(temp);
			size = temp;
		}

		if(id == 'FORM')
		{
			uint32 a;
			m_buffer >> a;
		}

		if(size & 0x1)
			size++;

		if( id == chunk_id )
		{
			return true;
		}
		else
		{
			if (size + m_buffer.Position() >= m_start_position + m_size)
			{
				// if the size of the chunk we found is too big, we just have to assume that there
				// are no more subchunks, the rest may be extra attributes
				return false;
			}
			m_buffer.Seek(size);
		}
	}

	return false;
}


IFFBuffer IFFBuffer::Chunk(uint32 chunk_name) const
{
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position);

	while(m_buffer.Position() < (m_start_position + m_size))
	{
		uint32	name;
		int32	size;

		m_buffer >> name;
		name = FromBigEndian(name);

		if(m_level < 2)
		{
			m_buffer >> size;
			size = FromBigEndian(size);
		}
		else
		{
			uint16	temp;
			m_buffer >> temp;
			temp = FromBigEndian(temp);
			size = temp;
		}

		uint32 chunk_id = 0;
		if(name == 'FORM')
		{
			m_buffer >> chunk_id;
			chunk_id = FromBigEndian(chunk_id);
		}

		if( name == chunk_name )
		{
			return IFFBuffer(*this, size, m_buffer.Position(), m_level+1, chunk_id);
		}
		else
		{
			// pad the size to whole uint16
			if(size & 0x1) size++;
			if (size + m_buffer.Position() >= m_start_position + m_size)
			{
				// if the size of the chunk we found is too big, we just have to assume that there
				// are no more subchunks, the rest may be extra attributes
				throw UnknownChunk(TEXT("Chunk not in buffer"),TEXT("IFFBuffer::Chunk"));
			}
			m_buffer.Seek(m_buffer.Position() + size);
		}
	}

	throw UnknownChunk(TEXT("Chunk not in buffer"),TEXT("IFFBuffer::Chunk"));
}

InStream &IFFBuffer::operator >>(uint8 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(int8 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(uint16 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(int16 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(uint32 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(int32 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(int64 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(uint64 &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}


InStream &IFFBuffer::operator >>(float &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(double &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	val = FromBigEndian(val);
	m_read_position+= sizeof(val);

	return *this;
}

InStream &IFFBuffer::operator >>(bool &val)
{
	D_ASSERT(m_read_position < m_size);
	m_buffer.Rewind();
	m_buffer.Seek(m_start_position + m_read_position);
	m_buffer >> val;
	m_read_position+= 1;
	
	return *this;
}

int32 IFFBuffer::Read(int8 *buffer, int32 size) throw ()
{
	D_ASSERT(m_read_position + size < m_size);
	return m_buffer.Read(buffer, size);
}
