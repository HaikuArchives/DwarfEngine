#ifndef D_TEXTURE_H_
#define D_TEXTURE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Vertex.h"
#include "Log.h"
#include "Bitmap.h"
#include "Handle.h"
#include "Color.h"

namespace dwarf
{

	/*!
		This handle is reference-counting. When you don't have any
		handles to a texture anymore, that texture will be removed
		from the rendering device.
		\brief a texture handle
	*/
	
	class Texture;
	class HTexture;

	class Texture : public RefCnt
	{
	friend class HTexture;
	public:
		

		enum TexCorrectionMode
		{
			AFFINE,
			PERSPECTIVE
		};

		enum TexApplyMode
		{
			REPLACE,
			DECAL,
			MODULATE,
			BLEND
		};

		enum TexWrapMode
		{
			CLAMP_S_CLAMP_T,
			CLAMP_S_WRAP_T,
			WRAP_S_CLAMP_T,
			WRAP_S_WRAP_T
		};

		enum TexFilterMode
		{
			NEAREST,
			LINEAR
		};

		enum TexMipmapMode
		{
			NO_MIPMAPS,
			NEAREST_NEAREST,
			NEAREST_LINEAR,
			LINEAR_NEAREST,
			LINEAR_LINEAR
		};

		static void SetFileSystem(FileSystem *fs);
		static FileSystem* GetFileSystem();

		~Texture();
	
		int32 Width() const;
		int32 Height() const;
		Bitmap::ColorSpace const& ColorSpace() const;
		TexCorrectionMode CorrectionMode() const;
		void SetCorrectionMode(TexCorrectionMode mode);
		TexApplyMode ApplyMode() const;
		void SetApplyMode(TexApplyMode mode);	// implement
		TexWrapMode WrapMode() const;
		void SetWrapMode(TexWrapMode mode);	// implement
		TexMipmapMode MipmapMode() const;
		void SetMipmapMode(TexMipmapMode mode);	// implement
		TexFilterMode FilterMode() const;
		void SetFilterMode(TexFilterMode mode);	// implement
		Color BlendColor() const;
		void SetBlendColor(const Color &c);
		
		void SetBitmap(Bitmap const& bitmap);
		Bitmap* GetBitmap();

		void PrintToLog() const;

		int32 const& Index() const;
		void SetIndex(int32 index);

		bool IsResident() const;

	private:
		Texture(FileSystem *dir, String const& name, TexMipmapMode mode = LINEAR_NEAREST);
		Texture(const Bitmap &bitmap, TexMipmapMode mode = LINEAR_NEAREST);
		Texture(Texture const& texture);
		Texture const& operator=(Texture const& texture);

		static FileSystem *m_fs;
		
		int32 m_index;
		int32 m_width;
		int32 m_height;
		Bitmap::ColorSpace m_color_space;

		RenderingDevice *m_device;	// should be removed
		File* m_file;				// should be replaced with filename
		Bitmap m_bitmap;

		Color m_blend_color;
		TexCorrectionMode m_correction;
		TexApplyMode m_apply;
		TexWrapMode m_wrap;
		TexMipmapMode m_mipmap;
		TexFilterMode m_filter;

		// Internal list handling functions
		// The create methods roam the list for the given texture,
		// and if found returns it. If not, a new texture is created
		// and inserted into the list.
		// The list should probably be implemented as a map as soon
		// as i've created the map class (soon).

		// This will also maintain the list, swapping entries in and
		// out of memory as needed (but never removing an entry)

		static HTexture Load(FileSystem *dir, String const& name, TexMipmapMode mode = LINEAR_NEAREST);
		static HTexture Load(String const& name, TexMipmapMode mode = LINEAR_NEAREST);
		
//			static ListArray<HTexture> ms_textures;
//			static FileSystem* ms_filesystem;
	};

	class HTexture : public Handle<Texture>
	{
	public:
		HTexture(): Handle<Texture>() {}
		HTexture(HTexture const& h): Handle<Texture>(h) {}
		HTexture(Texture* t): Handle<Texture>(t) {}

		HTexture(String const& name, Texture::TexMipmapMode mode = Texture::LINEAR_NEAREST)
		{
			*this = Texture::Load(name, mode);
		}
		HTexture(FileSystem *dir, String const& name, Texture::TexMipmapMode mode = Texture::LINEAR_NEAREST):
			Handle<Texture>()
		{
			*this = Texture::Load(dir, name, mode);
		}
		HTexture(const Bitmap &bitmap, Texture::TexMipmapMode mode = Texture::LINEAR_NEAREST):
			Handle<Texture>(new Texture(bitmap, mode))
		{}
		
		~HTexture() {}
	};

	/*!
		\brief queries the width of the texture
		\return the texture's width
	*/
	inline int32 Texture::Width() const
	{
		D_ASSERT_MESSAGE(m_width > 0, TEXT("Texture has invalid width."));
		return m_width;
	}
	
	/*!
		\brief queries the height of the texture
		\return the texture's height
	*/
	inline int32 Texture::Height() const
	{
		D_ASSERT_MESSAGE(m_height > 0, TEXT("Texture has invalid height."));
		return m_height;
	}
	
	/*!
		\brief queries the colorspace of the texture
		\return the texture's colorspace
		\sa Bitmap::ColorSpace
	*/
	inline Bitmap::ColorSpace const& Texture::ColorSpace() const
	{
		return m_color_space;
	}

	inline Texture::TexCorrectionMode Texture::CorrectionMode() const
	{
		return m_correction;
	}

	inline void Texture::SetCorrectionMode(TexCorrectionMode mode)
	{
		m_correction = mode;
	}

	inline Texture::TexApplyMode Texture::ApplyMode() const
	{
		return m_apply;
	}

	inline void Texture::SetApplyMode(TexApplyMode mode)
	{
		m_apply	= mode;
	}

	inline Texture::TexWrapMode Texture::WrapMode() const
	{
		return m_wrap;
	}

	inline void Texture::SetWrapMode(TexWrapMode mode)
	{
		m_wrap = mode;
	}

	inline Texture::TexMipmapMode Texture::MipmapMode() const
	{
		return m_mipmap;
	}

	inline void Texture::SetMipmapMode(TexMipmapMode mode)
	{
		m_mipmap = mode;
	}

	inline Texture::TexFilterMode Texture::FilterMode() const
	{
		return m_filter;
	}

	inline void Texture::SetFilterMode(TexFilterMode mode)
	{
		m_filter = mode;
	}

	inline Color Texture::BlendColor() const
	{
		return m_blend_color;
	}

	inline void Texture::SetBlendColor(const Color &c)
	{
		m_blend_color = c;
	}
	
	/*!
		The internal index number can be anything, it is not defined and only
		used by the rendering device.
		\brief queries the texture's internal index number.
		\return the texture's internal index number
		\warning dont not use this method unless you are implementing a rendering device. And
		if you are, only use the method from within the rendering device.
	*/
	inline int32 const& Texture::Index() const
	{
		D_ASSERT_MESSAGE(m_index >= 0, TEXT("You are trying to use an uninitialized texture."));
		return m_index;
	}

	inline void Texture::SetIndex(int32 index)
	{
		m_index = index;
	}

	inline bool Texture::IsResident() const
	{
		return m_index >= 0;
	}

} // dwarf

#endif // D_TEXTURE_H_
