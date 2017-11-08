/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Texture.h"
#include "Buffer.h"
#include "RenderingDevice.h"
#include "Window.h"
#include "Codec.h"
#include "File.h"

using namespace dwarf;

FileSystem* Texture::m_fs = 0;

void Texture::SetFileSystem(FileSystem *fs)
{
	m_fs = fs;
}

FileSystem* Texture::GetFileSystem()
{
	return m_fs;
}

HTexture Texture::Load(String const& name, TexMipmapMode mode)
{
	D_ASSERT_MESSAGE(m_fs != 0, TEXT("No texture file system set!"));
	
	return HTexture(new Texture(m_fs, name, mode));
}


HTexture Texture::Load(FileSystem *dir, String const& name, TexMipmapMode mode)
{
	return HTexture(new Texture(dir, name, mode));
}

Texture::Texture(const Bitmap &bitmap, Texture::TexMipmapMode mode) 
	: RefCnt(),
	m_device(0),
	m_index(-1),
	m_width(bitmap.Width()),
	m_height(bitmap.Height()),
	m_color_space(bitmap.Space()),
	m_file(0),
	m_bitmap(bitmap),
	m_blend_color(Color(0,0,0,0)),
	m_correction(Texture::PERSPECTIVE),
	m_apply(Texture::MODULATE),
	m_wrap(Texture::WRAP_S_WRAP_T),
	m_filter(Texture::LINEAR),
	m_mipmap(mode)
{
}

/*!
	\bief copy constructor.
*/
Texture::Texture(const Texture &texture) 
	: RefCnt(),
	m_device(texture.m_device),
	m_index(texture.m_index),
	m_width(texture.m_width),
	m_height(texture.m_height),
	m_color_space(texture.m_color_space),
	m_file(0),
	m_bitmap(texture.m_bitmap),
	m_blend_color(texture.m_blend_color),
	m_correction(texture.m_correction),
	m_apply(texture.m_apply),
	m_wrap(texture.m_wrap),
	m_filter(texture.m_filter),
	m_mipmap(texture.m_mipmap)
{
	if (texture.IsResident() && m_device)
	{
		// Make sure we have our own copy of the texture on the card as well
		m_index = -1;
		
		HTexture h(this);

		m_device->Lock();
		m_device->UploadTexture(h);
		m_device->Unlock();
		
		h.Invalidate(false); // don't let the handle delete us when we get zero refs
	}

	if (texture.m_file)
	{
		m_file = new File(*texture.m_file);
	}
}

Texture::Texture(FileSystem *dir, String const& name, Texture::TexMipmapMode mode) 
	: RefCnt(),
	m_device(0),
	m_index(-1),
	m_width(-1),
	m_height(-1),
	m_color_space(Bitmap::GRAYSCALE),
	m_file(new File(dir, name)),
	m_bitmap(),
	m_blend_color(Color(0,0,0,0)),
	m_correction(Texture::PERSPECTIVE),
	m_apply(Texture::MODULATE),
	m_wrap(Texture::WRAP_S_WRAP_T),
	m_filter(Texture::LINEAR),
	m_mipmap(mode)
{
	Buffer image_data;
	m_file->Read(image_data);
	Codec<Bitmap> const* codec = Codec<Bitmap>::FindDecoder(image_data);
	codec->Decode(image_data, m_bitmap);
	m_width = m_bitmap.Width();
	m_height = m_bitmap.Height();
	m_color_space = m_bitmap.Space();
}

/*!
	\brief destructor.
*/
Texture::~Texture()
{
	logger(TEXT("Destroyed texture: %d (%d,%d)"), m_index, m_width, m_height);

	if (m_device)
	{
		HTexture h(this);
		
		m_device->Lock();
		m_device->RemoveTexture(h);
		m_device->Unlock();
		
		h.Invalidate(false);
	}

	if (m_file)
	{
		delete m_file;
	}
}

/*!
	\brief assignment operator.
*/
const Texture &Texture::operator=(const Texture &texture)
{
	m_device = texture.m_device;
	m_width = texture.m_width;
	m_height = texture.m_height;
	m_color_space = texture.m_color_space;

	if (texture.m_file)
		m_file = new File(*texture.m_file);

	m_bitmap = texture.m_bitmap;
	m_blend_color = texture.m_blend_color;
	m_correction = texture.m_correction;
	m_apply = texture.m_apply;
	m_wrap = texture.m_wrap;
	m_filter = texture.m_filter;
	m_mipmap = texture.m_mipmap;

	if (texture.IsResident() && m_device)
	{
		// Make sure we have our own copy of the texture on the card as well
		m_index = -1;
		
		HTexture h(this);
		
		m_device->Lock();
		m_device->UploadTexture(h);
		m_device->Unlock();
		
		h.Invalidate(false);	// don't let the handle delete us
	}

	return *this;
}

/*!
	\brief prints this texture's properties to the log stream.
*/
void Texture::PrintToLog() const
{
	const dchar *convert[] =
	{
		TEXT("STENCIL_INDEX"),
		TEXT("DEPTH_COMPONENT"),
		TEXT("RED"),
		TEXT("GREEN"),
		TEXT("BLUE"),
		TEXT("ALPHA"),
		TEXT("RGB"),
		TEXT("RGBA"),
		TEXT("GRAYSCALE"),
		TEXT("GRAYSCALE_ALPHA"),
		TEXT("BGR"),
		TEXT("BGRA")
	};
	logger(TEXT("Texture (width: %d height: %d colorspace: %s)"), m_width, m_height, convert[m_color_space]);
}

void Texture::SetBitmap(Bitmap const& bitmap)
{
	bool was_resident = IsResident();

	if (was_resident)
	{
		D_ASSERT_MESSAGE(m_device, TEXT("Texture uploaded to gfx card had no device pointer!"));
		
		// remove us from the card
		HTexture h(this);
	
		m_device->Lock();
		m_device->RemoveTexture(h);
		m_device->Unlock();
		
		h.Invalidate(false);	// don't let the handle delete us
	}

	m_bitmap = bitmap;
	m_width = bitmap.Width();
	m_height = bitmap.Height();
	m_color_space = bitmap.Space();

	if (was_resident)
	{
		HTexture h(this);

		m_device->Lock();
		m_device->RemoveTexture(h);
		m_device->Unlock();

		h.Invalidate(false);
	}
}

/*! At the moment, all textures store a bitmap copy of the texture in
 *	system memory. Obviously, this is unnecessary most of the time, and
 *	so this must be fixed. Fixing it involves several things:
 *	1) The texture must be able to reload itself from disk
 *	2) The constructors should take a flag whether the texture should
 *		be uploaded directly (so the bitmap can be removed), or if
 *		it should wait.
 *	3) a bitmap must be able to be Invalid, that is exist but don't take
 *		space.
 */
Bitmap* Texture::GetBitmap()
{
	return &m_bitmap;
}
