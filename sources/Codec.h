#ifndef D_CODEC_H_
#define D_CODEC_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "nommgr.h"
#include <list>

#include "Defs.h"

#include "Array.h"
#include "Buffer.h"
#include "Exception.h"


namespace dwarf
{

		/*!
			\brief This class has not been fully documented yet.
			\todo document UnknownType.
		*/
		class UnknownType : public Exception
		{
			public:
			UnknownType(const String &source) : 
				Exception(TEXT("Cannot find a encoder/decoder compatible with the given format"), source, TEXT("Codec::UnknownType")) {}
		};

		/*!
			\brief This class has not been fully documented yet.
			\todo document DecodeError.
		*/
		class DecodeError : public Exception
		{
			public:
			DecodeError(const String &source) :
				Exception(TEXT("Error decoding buffer"), source, TEXT("Codec::DecodeError")) {}
		};

		/*!
			\brief This class has not been fully documented yet.
			\todo document EncodeError.
		*/
		class EncodeError : public Exception
		{
			public:
			EncodeError(const String &source) :
				Exception(TEXT("Error encoding buffer"), source, TEXT("Codec::EncodeError")) {}
		};

		
		/*!
			\brief This class is the base class for all kinds of codecs.

			\code
	// read the file into a buffer
	Directory dir;
	File image_file(&dir, TEXT("myimage.tga"));
	Buffer image_data;
	image_file.Read(image_data);

	// find a decoder and tell it to decode
	Bitmap image;
	Codec<Bitmap> codec = Codec<Bitmap>::FindDecoder(image_data);
	codec.Decode(image_data, minbild);
			\endcode
			This could be simplified by just using another constructor
			on Bitmap
			\code
	Directory dir;
	Bitmap image(&dir, TEXT("myimage.tga"));
			\endcode
			\todo document Codec.
		*/
		template<class TARGET> class Codec
		{
		public:

			Codec()
			{
				m_codecs.push_back(this);
			}

			virtual ~Codec()
			{
				m_codecs.remove(this);
			}

			// returns true if it succeeded.
			virtual bool Decode(Buffer& from, TARGET& to) const = 0;
			virtual bool Encode(const TARGET& from, Buffer& to) const = 0;

// These are not needed, we already have CanEncode() & CanDecode()
//			virtual bool IsEncoder() { return false; }
//			virtual bool IsDecoder() { return false; }

			// checks, based on filename and/or file contents
			// whether it is capable of encoding/decoding the
			// specified file. Used by the codec registry.
			virtual bool CanEncode(const TARGET& data) const = 0;
			virtual bool CanDecode(Buffer& data) const = 0;
			
			// Attempts to find a suitable codec for the given file.
			// If it fails, it throws an exception.
			static const Codec<TARGET> *FindDecoder(Buffer& data);
			static const Codec<TARGET> *FindEncoder(const TARGET& data);

		private:
			static std::list<Codec<TARGET>* > m_codecs;
		};

		// inline definitions

		template <class TARGET> std::list<Codec<TARGET>* > Codec<TARGET>::m_codecs;

		template <class TARGET>
		Codec<TARGET> const* Codec<TARGET>::FindDecoder(Buffer& data)
		{
			logger(Log::HIGH, TEXT("Codec::FindDecoder()"), m_codecs.size());
			for(std::list<Codec<TARGET>*>::iterator i = m_codecs.begin(); i != m_codecs.end(); i++)
			{
				data.Rewind();
				if((*i)->CanDecode(data))
				{
					data.Rewind();
					return *i;
				}
			}
			data.Rewind();
			throw UnknownType(TEXT("Codec::FindDecoder"));
		}

		template <class TARGET>
		Codec<TARGET> const* Codec<TARGET>::FindEncoder(const TARGET& data)
		{
			for (std::list<Codec<TARGET>* >::iterator i = m_codecs.begin(); i != m_codecs.end(); i++)
			{
				if((*i)->CanEncode(data) )
				{
					return *i;
				}
			}
			
			throw UnknownType(TEXT("Codec::FindEncoder"));
		}


} // dwarf

#endif // D_CODEC_H_
