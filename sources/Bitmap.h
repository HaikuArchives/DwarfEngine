#ifndef D_BITMAP_H_
#define D_BITMAP_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"
#include "Array.h"
#include "String.h"
#include "Buffer.h"
#include "Serializable.h"

#ifdef WIN32
// IGNORE is defined in winbase.h
#undef IGNORE
#endif

namespace dwarf
{

		class File;
		class FileSystem;
		class String;
		class Bitmap;

		/*!
			\brief This class has not been fully documented yet.
			\todo document Bitmap.
		*/
		class Bitmap : public Serializable
		{
			public:
		
			enum ResizeMode
			{
				IGNORE = 0,	//!< Ignores the bitmap, after the resize the bitmap will contain undefined data
				CROP,		//!< The bitmap will be cropped or the extended areas will be filled with the color black (no rescaling at all)
				SCALE		//!< The bitmap will be scaled to match the new size
			};

			enum ColorSpace
			{
				STENCIL_INDEX = 0,
				DEPTH_COMPONENT,
				RED,
				GREEN,
				BLUE,
				ALPHA,
				RGB,
				RGBA,
				GRAYSCALE,
				GRAYSCALE_ALPHA,
				BGR,
				BGRA
			};
		
		
			Bitmap();
//			Bitmap(const File &image);
//			Bitmap(int32 width, int32 height, dwarf::core::Bitmap::ColorSpace space = RGB);
			Bitmap(FileSystem* dir, String const& name);
			Bitmap(int32 width, int32 height, ColorSpace space = RGB);
			Bitmap(const Array<uint8> &bitmap,int32 width,int32 height,ColorSpace space = RGB);
			Bitmap(const uint8 *bitmap,int32 width,int32 height,ColorSpace space = RGB);

			virtual ~Bitmap();
		
			int32 Width() const;
			int32 Height() const;
		
			uint8* Data() const;
			int32 DataSize() const;
		
			ColorSpace Space() const;
			void SetSpace(Bitmap::ColorSpace space, bool convert);

			void SetSize(int32 width, int32 height, Bitmap::ColorSpace space);
			void SetSize(int32 width, int32 height, Bitmap::ResizeMode mode = IGNORE);

			uint8 BytesPerPixel() const;

			void PrintToLog() const;

			virtual void Restore(InStream& buffer);
			virtual void Serialize(OutStream& buffer) const;

		private:
		
			uint8 BytesPerPixel(Bitmap::ColorSpace space) const;

			int32 m_width;
			int32 m_height;
			ColorSpace m_color_space;
			Array<uint8> m_buffer;
		};
		
		// inline definitions
		
		inline uint8 Bitmap::BytesPerPixel() const
		{
			return BytesPerPixel(m_color_space);
		}

		inline Bitmap::Bitmap():
			m_width(0),
			m_height(0),
			m_color_space(RGB),
			m_buffer(0)
		{
		}
		
		inline Bitmap::Bitmap(int32 width, int32 height, Bitmap::ColorSpace space):
			m_width(width),
			m_height(height),
			m_color_space(space),
			m_buffer(m_width * m_height * BytesPerPixel())
		{
		}
		
		inline int32 Bitmap::Width() const
		{
			return m_width;
		}
		
		inline int32 Bitmap::Height() const
		{
			return m_height;
		}
		
		inline uint8* Bitmap::Data() const
		{
			return *m_buffer;
		}
		
		inline int32 Bitmap::DataSize() const
		{
			return m_buffer.Length();
		}
			
		inline Bitmap::ColorSpace Bitmap::Space() const
		{
			return m_color_space;
		}

		inline void Bitmap::PrintToLog() const
		{
			logger(TEXT("Bitmap: m_width: %d m_height: %d m_bpp: %d"), m_width, m_height, BytesPerPixel());
		}

} // dwarf

#endif // D_BITMAP_H_
