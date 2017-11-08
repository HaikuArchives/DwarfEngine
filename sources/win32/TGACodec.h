#ifndef D_TGA_H_
#define D_TGA_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */




namespace dwarf
{

		class TGACodec : public Codec<Bitmap>
		{
			public:
			// returns true if it succeeded.
			virtual bool Decode(Buffer & from, Bitmap& to) const;
			virtual bool Encode(Bitmap const& from, Buffer& to) const;

			virtual bool CanEncode(Bitmap const& data) const;
			virtual bool CanDecode(Buffer& data) const;

			virtual bool IsDecoder() { return true; }

			private:
			bool LoadTGA(uint8 *from, int32 size, Bitmap &to) const;
		};

} // dwarf

#endif // D_TGA_H_