#ifndef D_BUFFER_H_
#define D_BUFFER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Exception.h"
#include "Array.h"
#include "OutStream.h"
#include "InStream.h"

namespace dwarf
{

		class Rect;

		/*!
			The Buffer class uses a dynamic array (Array) to store it's data. The interface to the buffer works
			like a queue, you pushes data onto the buffer on one end, and reads data from the same end (first in first out). You
			can push many different types of data, and you have to make sure you pops the same kind of data (there's no type information stored
			in the buffer, just raw data).
			So you could push an int32 and pop 4 int8's.
			The Buffer is used when communicating over the network sockets (ClientSocket), for the internal messages, when reading files and by the codecs
			to convert data.
			\warning you should NOT use this class as a general purpose queue (see Queue for that). If you keep pushing data onto the buffer it will grow more and more.
			To make sure it doesn't grow to big, call Empty() to empty the buffer.
			\brief Represents a memory buffer, that can contain any data, and has a simple interface setting and getting that data
		*/
	
		class Buffer: public OutStream, public InStream
		{
		public:
		
		
			/*!
				\brief this exception is thrown when you do an illegal pop (>>) operation on a Buffer
				\sa Buffer
			*/
			class IllegalPop : public Exception
			{
				public:
				IllegalPop(const String &message, const String &source):
					Exception(message, source, TEXT("Buffer::IllegalPop")) {};
			};



			Buffer(int32 size = 0);
			Buffer(const void *buffer, int32 size);
			Buffer(const Buffer &buf);
			virtual ~Buffer();

//			template <class T> Buffer &operator <<(const T val);
//			template <class T> Buffer &operator >>(T &val) throw(IllegalPop);

			Buffer &operator <<(const Buffer &buffer);

			Buffer &operator >>(Buffer &buffer) throw ();

			virtual OutStream &operator <<(const dchar *string);
			virtual OutStream &operator <<(const uint8 val);
			virtual OutStream &operator <<(const uint16 val);
			virtual OutStream &operator <<(const uint32 val);
			virtual OutStream &operator <<(const uint64 val);
			virtual OutStream &operator <<(const int8 val);
			virtual OutStream &operator <<(const int16 val);
			virtual OutStream &operator <<(const int32 val);
			virtual OutStream &operator <<(const int64 val);

			virtual OutStream &operator <<(const float val);
			virtual OutStream &operator <<(const double val);
			virtual OutStream &operator <<(const bool val);
			
			virtual InStream &operator >>(uint8 &val) throw();
			virtual InStream &operator >>(uint16 &val) throw();
			virtual InStream &operator >>(uint32 &val) throw();
			virtual InStream &operator >>(uint64 &val) throw();
			virtual InStream &operator >>(int8 &val) throw();
			virtual InStream &operator >>(int16 &val) throw();
			virtual InStream &operator >>(int32 &val) throw();
			virtual InStream &operator >>(int64 &val) throw();
			//virtual InStream &operator >>(Vertex<3> &val) throw();

			virtual InStream &operator >>(float &val) throw();
			virtual InStream &operator >>(double &val) throw();
			virtual InStream &operator >>(bool &val) throw();
			
			virtual int32 Read(int8 *data, int32 size) throw ();

			virtual void Flush() {}

			int32 Size() const;
		
			void SetSize(int32 buffer_size);
			void Empty();

			void Rewind();
			void Seek(int32 position);

			int32 Position() const;

			void SetTo(const void *buffer, int32 size);

			bool EndOfBuffer() const;
			void *Data() const;
			
			Buffer operator +(const Buffer &buf) const;
			Buffer &operator +=(const Buffer &buf);
			Buffer &operator =(const Buffer &buf);

			void AddData(const uint8 *data, int32 size);

			void *SupplyData(int32 num_bytes);
			void DataWritten(int32 num_bytes);

			protected:
		
		
			//! The current read/write position in the buffer, the position where we read data from
			int32 m_position;

			//! The size of the buffer
			int32 m_size;

			//! The internal buffer used to store the data
			Array<uint8> m_buffer;

		private:

			enum
			{
				BLOCK_SIZE = 30
			};
		
		};

	
	
		/*!
			\brief forces the buffer to reallocate, to match the new size
			\param buffer_size the new size, in bytes
		*/
		inline void Buffer::SetSize(int32 buffer_size)
		{
			D_ASSERT_MESSAGE(buffer_size >= 0, TEXT("You are trying to set the buffersize to a negative number"));
			m_buffer.SetLength(buffer_size);
		}

		/*!
			\brief Sets the current file position.

			Subsequent calls to Read() and Write() will read from the given position in the file.
			\param position the new file position
		*/
		inline void Buffer::Seek(int32 position)
		{
			int32 new_pos =  position;
			if (new_pos < 0) m_position = 0;
			else if (new_pos > Size()) m_position = Size();
			else m_position = new_pos;
		}

		inline int32 Buffer::Position() const
		{
			return m_position;
		}

		/*!
			\brief a copy constructor
			\param buf the buffer to copy
		*/
		inline Buffer::Buffer(const Buffer &buf):
			m_position(buf.m_position),
			m_size(buf.m_size),
			m_buffer(buf.m_buffer)
		{
		}
		
		/*!
			\brief standard copy operator
			\param buf a const reference to a Buffer to copy
			\return a reference to this object
		*/
		inline Buffer &Buffer::operator=(const Buffer &buf)
		{
			m_position = buf.m_position;
			m_size = buf.m_size;
			m_buffer = buf.m_buffer;
			return *this;
		}
		
		/*!
			\brief tells you how many bytes of data written to the buffer
			\return the number of bytes of data written to the buffer so far
		*/
		inline int32 Buffer::Size() const
		{
			return m_size;
		}

		/*!
			\brief gives you the pointer to the internal buffer
			\warning This pointer may be invalid as soon as you call any non const methods. You should never store this pointer, it's only here for debugging purposes.
			\return The internal buffer pointer
		*/
		inline void *Buffer::Data() const
		{
			return reinterpret_cast<void*>(*m_buffer);
		}

		// resets the read offset to the beginning of the buffer
		inline void Buffer::Rewind()
		{
			m_position = 0;
		}

		inline bool Buffer::EndOfBuffer() const
		{
			return m_position >= m_size;
		}

		/*!
			Inserts a chunk of data at the end of the buffer
			\brief Low-level method, please avoid using it
		*/
		inline void Buffer::AddData(const uint8 *data, int32 size)
		{
			if (size + m_position > m_buffer.Length())
				m_buffer.SetLength(m_position + size + BLOCK_SIZE);

			memcpy(const_cast<uint8*>(&m_buffer[m_position]), static_cast<const void*>(data), size);
			m_position += size;
		}

		/*!
			Reads a chunk of data from the buffer. It will, of course, also
			move the current position-pointer forward in the buffer (the number of bytes
			that was read).
			\brief Low-level method, please avoid using it
		*/
		inline int32 Buffer::Read(int8 *data, int32 size) throw ()
		{
			if (size + m_position > m_size) size = m_size - m_position;

			memcpy(static_cast<void*>(data), static_cast<const void*>(&m_buffer[m_position]), size);
			m_position += size;
			return size;
		}

		/*!
			This method is always used together with DataWritten(). You don't always know
			how many bytes the function actally will write to the buffer, therefore you always
			have to call DataWritten() and tell the Buffer how many bytes where added to the buffer.
			And remember this is a low-level method, only use this if you feel sure about what you are doing.
			In most cases you probably want to Empty() the buffer before you add data to it this way (but
			you don't have to).
			Example usage:
			\code
	Buffer buf;
	sprintf(buf.SupplyData(30), "%d", 30);
	buf.DataWritten(30);
			\endcode
			\brief low-level method when another function needs a buffer to fill
			\param num_bytes The maximum buffer size that the function needs
			(the function that will fill the buffer that is)
			\return returns a pointer to the current write-position in the buffer and
			guarantees that there will be at least num_bytes allocated after this
			point in the buffer. 
			\warning You really have to call DataWritten() after this call, or else it will have no effect at all
			\sa DataWritten()
		*/
		inline void *Buffer::SupplyData(int32 num_bytes)
		{
			if (m_buffer.Length() - m_position < num_bytes)
				m_buffer.SetLength(m_position + num_bytes + BLOCK_SIZE);
			return &m_buffer[m_position];
		}

		/*!
			his method will increase the write offset with the given number of bytes
			\brief low-level method, use ONLY after a call to SupplyData()
			\param num_bytes The number of bytes you previously wrote to the buffer
			through the SupplyData() call.
		*/
		inline void Buffer::DataWritten(int32 num_bytes)
		{
			if (m_buffer.Length() < m_position + num_bytes) throw Exception(TEXT("You tried to call DataWritten() with a too large number (more bytes than exists in the byffer)"), TEXT("Buffer::DataWritten()"));
			m_position += num_bytes;
			if (m_position > m_size) m_size = m_position;
		}

/*
		template <class T>
		inline T Buffer::ToBigEndian(T val) const
		{
			#ifdef D_LITTLE_ENDIAN
			
			return swap_byteorder(val);

			#else

			return val;
			
			#endif
		}

		template <class T>
		inline T Buffer::ToLittleEndian(T val) const
		{
			#ifdef D_BIG_ENDIAN
			
			return swap_byteorder(val);

			#else

			return val;
			
			#endif
		}

*/

} // dwarf


#endif //D_BUFFER_H_
