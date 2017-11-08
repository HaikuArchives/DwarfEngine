/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Bitmap.h"
#include "Buffer.h"
#include "File.h"
#include "Codec.h"

using namespace dwarf;

Bitmap::Bitmap(FileSystem *dir, const String &name):
	m_width(0),
	m_height(0),
	m_color_space(RGB),
	m_buffer(0)
{
	D_ASSERT_MESSAGE(dir, TEXT("The directory pointer is a null-pointer"));

	File file(dir, name, File::READ, 0);
	Buffer buf;
	file.Read(buf);
	const Codec<Bitmap> *codec = Codec<Bitmap>::FindDecoder(buf);
	codec->Decode(buf, *this);
}

Bitmap::Bitmap(const Array<uint8> &bitmap,int32 width,int32 height,ColorSpace space):
	m_width(width),
	m_height(height),
	m_color_space(space),
	m_buffer(bitmap)
{
	D_ASSERT_MESSAGE(width*height*BytesPerPixel(space) == m_buffer.Length(),TEXT("Size on array does not match width and height."));
}

Bitmap::Bitmap(const uint8 *bitmap,int32 width,int32 height,ColorSpace space):
	m_width(width),
	m_height(height),
	m_color_space(space),
	m_buffer(bitmap,width*height*BytesPerPixel(space))
{
}

Bitmap::~Bitmap()
{
}

/*!
	\brief resizes the bitmap destructively
	\param width the new width of the bitmap
	\param height the new height of the bitmap
	\param space the colorspace you want the bitmap to have
	\warning This method will destroy the contents of the bitmap

*/
void Bitmap::SetSize(int32 width, int32 height, Bitmap::ColorSpace space)
{
	m_color_space = space;
	m_width = width;
	m_height = height;
	m_buffer.SetLength(Width() * Height() * BytesPerPixel(), false);
}

/*!
	\brief resizes the bitmap
	\param width the new width of the bitmap
	\param height the new height of the bitmap
	\param mode tells the method how the bitmap will be handled during the resize (destroyed, cropped or rescaled)
	\todo SCALE is not yet implemented
*/
void Bitmap::SetSize(int32 width, int32 height, Bitmap::ResizeMode mode)
{
	switch (mode)
	{

	case IGNORE:
		m_width = width;
		m_height = height;
		m_buffer.SetLength(Width() * Height() * BytesPerPixel(), false);
		break;
	case CROP:
		{
			Array<uint8> new_buffer(BytesPerPixel() * width * height);

			for (int32 y = 0; y < height; y++)
			{
				// if we passed the height of the current bitmap, fill the line with zeroes
				if (y >= Height())
				{
					for (int32 x = 0; x < width * BytesPerPixel(); x++)
						new_buffer[y*width*BytesPerPixel() + x] = 0;
				}
				else // copy the data from the current bitmap
				{
					for (int32 x = 0; x < width * BytesPerPixel(); x++)
					{
						if (x >= Width() * BytesPerPixel()) new_buffer[y*width*BytesPerPixel() + x] = 0;
						else new_buffer[y*width*BytesPerPixel() + x] = m_buffer[y*Width() + x];
					}
				}
			}
			m_buffer = new_buffer;
			m_width = width;
			m_height = height;
		}
		break;
	case SCALE:
		{
			Array<uint8> new_buffer(BytesPerPixel() * width * height);

			float x_ratio = (float)Width() / (float)width;
			float y_ratio = (float)Height() / (float)height;

			float orig_x = 0, orig_y = 0; // the position in the original bitmap

			for (int32 y = 0; y < height; y++)
			{
				for (int32 x = 0; x < width; x++)
				{
					

					orig_x += x_ratio;
				}
				orig_y += y_ratio;
			}
		
		}
		break;
	}
}

uint8 Bitmap::BytesPerPixel(Bitmap::ColorSpace space) const
{
	static uint8 bytes[] = {1, 4, 1, 1, 1, 1, 3, 4, 1, 2, 3, 4};
	return bytes[space];
}


/*!
	\brief Changes the color space of the bitmap
	\param space the color space you want the bitmap to have
	\param convert if set to true the actual bitmap is converted into the new format, if set 
	to false the bitmap data will not be touched (but it may be cropped at the end)
*/
void Bitmap::SetSpace(Bitmap::ColorSpace space, bool convert)
{
	uint8 old_bytes = BytesPerPixel();
	ColorSpace old_space = ColorSpace();
	m_color_space = space;

	// if the colordepth has changed, reallocate the bitmap memory buffer
	if (old_bytes != BytesPerPixel() && !convert) m_buffer.SetLength(m_width * m_height * BytesPerPixel());
	else
	{
		if (old_bytes >= 3 && BytesPerPixel() == 1)
		{
			for(int32 i = 0; i < Width() * Height() * BytesPerPixel(); i++)
				m_buffer[i] = (m_buffer[i*old_bytes] + m_buffer[i*old_bytes + 1] + m_buffer[i*old_bytes + 2]) / 3;
			m_buffer.SetLength(m_width * m_height * BytesPerPixel());
		}
		else if (old_bytes == 1 && BytesPerPixel() == 3)
		{
			m_buffer.SetLength(m_width * m_height * BytesPerPixel());
			for(int32 i = Width() * Height() * old_bytes - 1; i >= 0; i--)
			{
				register uint8 val = m_buffer[i];
				m_buffer[i*3] = val;
				m_buffer[i*3 + 1] = val;
				m_buffer[i*3 + 2] = val;
			}
		}
		else if (old_bytes == 1 && BytesPerPixel() == 4)
		{
			m_buffer.SetLength(m_width * m_height * BytesPerPixel());
			for(int32 i = Width() * Height() * old_bytes - 1; i >= 0; i--)
			{
				register uint8 val = m_buffer[i];
				m_buffer[i*4] = val;
				m_buffer[i*4 + 1] = val;
				m_buffer[i*4 + 2] = val;
				m_buffer[i*4 + 3] = 255;
			}
		}
		else if (old_space != ColorSpace() && old_bytes == BytesPerPixel() && old_bytes >= 3)
		{
			for(int32 i = Width() * Height() * old_bytes - 1; i >= 0; i--)
			{
				register uint8 temp = m_buffer[i*old_bytes];
				m_buffer[i*old_bytes] = m_buffer[i*old_bytes + 2];
				m_buffer[i*old_bytes + 2] = temp;
			}
		}
		else m_buffer.SetLength(m_width * m_height * BytesPerPixel());
	}
}

void Bitmap::Restore(InStream& buffer)
{
	buffer >> m_width >> m_height >> (uint8&)m_color_space;

	if (m_buffer.Length() < m_width * m_height * BytesPerPixel())
		m_buffer.SetLength(m_width * m_height * BytesPerPixel());

	for (int32 i = 0; i < m_width * m_height * BytesPerPixel(); i++)
		buffer >> m_buffer[i];
}

void Bitmap::Serialize(OutStream& buffer) const
{
	buffer << m_width << m_height << (uint8)m_color_space;
	for (int32 i = 0; i < m_width * m_height * BytesPerPixel(); i++)
		buffer << m_buffer[i];
}

