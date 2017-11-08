/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Buffer.h"
#include "Exception.h"
#include "String.h"

using namespace dwarf;

/*!
	\brief Buffer constructor
	\param size sets the initial size of the buffer, default is 0
*/
Buffer::Buffer(int32 buffer_size):
	m_position(0),
	m_size(0),
	m_buffer(buffer_size)
{
}

Buffer::Buffer(const void *buffer, int32 size):
	m_position(0),
	m_size(size),
	m_buffer(size)
{
	memcpy(*m_buffer, buffer, size);
}

/*!
	\brief destructor
*/
Buffer::~Buffer()
{
}

void Buffer::SetTo(const void *buffer, int32 size)
{
	m_position = 0;
	m_size = size;
	m_buffer.SetLength(size);
	memcpy(*m_buffer, buffer, size);
}

/*!
	\brief Empties the buffer, removes all of it's contents
*/
void Buffer::Empty()
{
	m_size = 0;
	m_position = 0;
}

Buffer Buffer::operator +(const Buffer &buf) const
{
	Buffer ret(Size() + buf.Size());
	ret += *this;
	ret += buf;
	return ret;
}

Buffer &Buffer::operator +=(const Buffer &buf)
{
	if (m_position + buf.Size() > m_buffer.Length())
		m_buffer.SetLength(m_position + buf.Size() + BLOCK_SIZE);
	
	memcpy(&m_buffer[m_position], buf.Data(), buf.Size());
	m_size += buf.Size();
	return *this;
}


// ******* WRITE *******

OutStream &Buffer::operator<<(const dchar *str)
{
	int32 len = 0;
	while (str[len]) len++;

	*this << len;
#ifdef D_UNICODE
	for(int32 i = 0; i < len; i++)
		*this << str[i];
#else
	for(int32 i = 0; i < len; i++)
	{
		uint16 ch = str[i] << 8;
		*this << ch;
	}
#endif
	return *this;
}

/*!
	\brief pushes a uint32 onto the data queue
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const uint32 val)
{
	if (m_position + (int32)sizeof(val) > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<uint32*>(&m_buffer[m_position]) = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes a uint64 onto buffer
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const uint64 val)
{
	if (m_position + (int32)sizeof(val) > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<uint32*>(&m_buffer[m_position]) = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes an int32 onto the data queue
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const int32 val)
{
	if (m_position + (int32)sizeof(val) > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<int32*>(&m_buffer[m_position]) = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes an int64 onto the buffer
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const int64 val)
{
	if (m_position + (int32)sizeof(val) > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<int32*>(&m_buffer[m_position]) = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes a uint16 onto the data queue
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const uint16 val)
{
	if (m_position + (int32)sizeof(val) > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<uint16*>(&m_buffer[m_position]) = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes an int16 onto the data queue
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const int16 val)
{
	if (m_position + (int32)sizeof(val) > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<int16*>(&m_buffer[m_position]) = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes a uint8 onto the data queue
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const uint8 val)
{
	if ((int32)sizeof(val) + m_position > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	m_buffer[m_position] = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes an int8 onto the data queue
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const int8 val)
{
	if ((int32)sizeof(val) + m_position > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<int8*>(&m_buffer[m_position]) = ToLittleEndian(val);

	m_position+= sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes a float onto the data queue
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const float val)
{
	if ((int32)sizeof(val) + m_position > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<float*>(&m_buffer[m_position]) = ToLittleEndian(val);
	m_position += sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}

/*!
	\brief pushes a double onto the buffer
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const double val)
{
	if ((int32)sizeof(val) + m_position > m_buffer.Length())
		m_buffer.SetLength(m_position + sizeof(val) + BLOCK_SIZE);

	*reinterpret_cast<float*>(&m_buffer[m_position]) = ToLittleEndian(val);
	m_position += sizeof(val);
	if (m_position > m_size) m_size = m_position;
	return *this;
}


/*!
	\brief pushes a double onto the buffer
	\param val the value to push
	\return a reference to this buffer
*/
OutStream &Buffer::operator <<(const bool val)
{
	if (1 + m_position > m_buffer.Length())
		m_buffer.SetLength(m_position + 1 + BLOCK_SIZE);

	m_buffer[m_position] = val?255:0;
	m_position += 1;
	if (m_position > m_size) m_size = m_position;
	return *this;
}


/*!
	\brief pushes another Buffer onto the data queue. The given buffer is copied, so you still have responsibility to delete the buffer
	\param buffer a pointer to a Buffer
	\return a reference to this buffer
*/
Buffer &Buffer::operator <<(const Buffer &buf)
{
	*this << buf.Size();

	if (m_position + buf.Size() > m_buffer.Length())
		m_buffer.SetLength(m_position + buf.Size() + BLOCK_SIZE);
	
	memcpy(&m_buffer[m_position], buf.Data(), buf.Size());
	m_position += buf.Size();
	if (m_position > m_size) m_size = m_position;
	return *this;
}


// ******* READ *******

/*!
	\brief pops a uint64 from the buffer
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(uint64 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(uint64 &)"));

	val = ToLittleEndian(*reinterpret_cast<uint32*>(&m_buffer[m_position]));
	
	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops a uint32 from the buffer
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(uint32 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(uint32 &)"));

	val = ToLittleEndian(*reinterpret_cast<uint32*>(&m_buffer[m_position]));
	
	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops an int64 from the buffer
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(int64 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(int64 &)"));

	val = ToLittleEndian(*reinterpret_cast<int32*>(&m_buffer[m_position]));

	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops an int32 from the data queue
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(int32 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(int32 &)"));

	val = ToLittleEndian(*reinterpret_cast<int32*>(&m_buffer[m_position]));

	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops a uint16 from the data queue
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(uint16 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(uint16 &)"));

	val = ToLittleEndian(*reinterpret_cast<uint16*>(&m_buffer[m_position]));

	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops an int16 from the data queue
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(int16 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(int16 &)"));

	val = ToLittleEndian(*reinterpret_cast<int16*>(&m_buffer[m_position]));

	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops a uint8 from the data queue
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(uint8 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(uint8 &)"));

	val = ToLittleEndian(m_buffer[m_position]);

	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops an int8 from the data queue
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(int8 &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(int8 &)"));

	val = ToLittleEndian(*reinterpret_cast<int8*>(&m_buffer[m_position]));

	m_position+= sizeof(val);
	return *this;
}

/*!
	\brief pops a float from the buffer
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(float &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(float &)"));

	val = ToLittleEndian(*reinterpret_cast<float*>(&m_buffer[m_position]));

	m_position += sizeof(val);
	return *this;
}

/*!
	\brief reads a double from the buffer
	\return a reference to this buffer
*/
InStream &Buffer::operator >>(double &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(val) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(val) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(double &)"));

	val = ToLittleEndian(*reinterpret_cast<float*>(&m_buffer[m_position]));

	m_position += sizeof(val);
	return *this;
}

InStream &Buffer::operator >>(bool &val) throw ()
{
	D_ASSERT_MESSAGE(m_position + 1 <= m_size, TEXT("You tried to read after the end of a buffer"));
	//	if (m_position + 1 > m_size)
	//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(bool &)"));
	
	val = m_buffer[m_position] != 0;

	m_position += 1;
	return *this;
}


/*!
	\brief pops a buffer from the data queue
	\return a reference to this buffer
*/
Buffer &Buffer::operator >>(Buffer &buf) throw()
{
	buf.Empty();
	D_ASSERT_MESSAGE(m_position + (int32)sizeof(uint32) <= m_size, TEXT("You tried to read after the end of a buffer"));
//	if (m_position + (int32)sizeof(uint32) > m_size)
//		throw IllegalPop(TEXT("read on empty buffer"), TEXT("Buffer::operator>>(Buffer &)"));

	uint16 size = 0;
	*this >> size;

	D_ASSERT_MESSAGE(m_position + size <= m_size, TEXT("You tried to read after the end of a buffer"));
	
//	if (m_position + size > m_size)
//		throw IllegalPop(TEXT("Tried to pop a buffer where there are no buffer"), TEXT("Buffer::operator>>(Buffer &)"));

	m_position += size;
	buf.m_buffer.SetLength(size + BLOCK_SIZE);
	memcpy(buf.Data(), reinterpret_cast<void*>(&m_buffer[m_position - size]), m_buffer.Length());

	buf.m_size = Size();
	return *this;
}
