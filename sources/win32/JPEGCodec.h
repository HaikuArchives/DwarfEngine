#ifndef D_JPEG_H_
#define D_JPEG_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


namespace dwarf
{


	class JPEGCodec : public Codec<Bitmap>
	{
	public:
		
		JPEGCodec();
		virtual ~JPEGCodec();

		// returns true if it succeeded.
		virtual bool Decode(Buffer& from, Bitmap& to) const;
		virtual bool Encode(const Bitmap& from, Buffer& to) const;

		virtual bool CanEncode(const Bitmap& data) const;
		virtual bool CanDecode(Buffer& data) const;

		void SetQuality(int32 quality) { m_quality = quality; }
		
		virtual bool IsEncoder() { return true; }
		virtual bool IsDecoder() { return true; }

	private:

		int32 m_quality;

	};


} // dwarf

#endif // D_JPEG_H_