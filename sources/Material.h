#ifndef D_MATERIAL_H_
#define D_MATERIAL_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Handle.h"
#include "Texture.h"

namespace dwarf
{

		class Material;
		typedef Handle<Material> HMaterial; //! Handle to a material

		/*!
			This describes a material for a certain surface.
			It contains information like texture, bumpmap,
			detail map etc.
			\brief a surface material
		*/
		class Material : public RefCnt
		{
			public:

			//! Surface flags
			enum MaterialFlags
			{	
				M_INVISIBLE		= 0x000001,	//! Surface is invisible
				M_ALPHATEST		= 0x000002,	//! Surface should be drawn with alpha test active
				M_TRANSPARENT	= 0x000004,	//! Surface is transparent
				M_SOLID			= 0x000008,	//! Surface is solid (for collision testing)
				// move envmap to sector (so that reflection depends on environment, duh)
				M_ENVMAP		= 0x000010,	//! Surface is environment mapped
				M_DETAILMAP		= 0x000020,	//!	Surface is detail mapped
				M_REFLECTIVE	= 0x000040,	//! Surface is reflective
				// move lightmap to per-face place
				M_LIGHTMAP		= 0x000080,	//! Surface is lightmapped
				M_TWOSIDED		= 0x000100,	//! Surface is two sided
				M_VERTEXLIT		= 0x000200,	//! Surface receives vertex lighting
				M_PORTAL		= 0x000400,	//! Surface is portal
				M_FOG			= 0x000800,	//! Surface has fog parameters
				M_EMITLIGHT		= 0x001000,	//! Surface emits light
			};

			friend class Handle<Material>;

			Material();
			Material(HTexture texture);
			Material(Material const& m);

			virtual ~Material();

			// Texture handling
			void SetTexture(HTexture texture);
			bool HasTexture()	const;
			HTexture	GetTexture();

			// Detail map
			void SetDetailMap(HTexture texture);
			bool HasDetailMap() const;
			HTexture GetDetailMap();

			// Environment map
			void SetEnvironmentMap(HTexture texture);
			bool HasEnvironmentMap() const;
			HTexture GetEnvironmentMap();

			// Light map
			void SetLightMap(HTexture texture);
			bool HasLightMap() const;
			HTexture GetLightMap();

			// Material flags
			uint32 GetFlags() const;
			void AddFlags(MaterialFlags f);
			void SetFlags(MaterialFlags f);

			// support for material chaining (for different materials on multiple rendering passes)
			void SetNextMaterial(HMaterial material);
			bool HasNextMaterial() const;
			HMaterial GetNextMaterial();

			Color& BlendColor();
			Color& Diffuse();
			Color& Luminance();
			Color& Specular();

		private:
			uint32 m_flags;

			Color m_diffuse;
			Color m_luminance;
			Color m_specular;
			Color m_blend;

			HTexture	m_texture;		// texture, if any
			HTexture	m_detail_map;	// detail map, if any
			HTexture	m_env_map;		// environment map, if any
			HTexture	m_light_map;	// light map, if any
			
			HMaterial	m_next;
		};

		// inlines

		inline
		void Material::SetTexture(HTexture texture)
		{
			m_texture = texture;
		}

		inline
		bool Material::HasTexture()	const
		{
			return m_texture.IsValid();
		}

		inline
		HTexture	Material::GetTexture()
		{
			D_ASSERT(m_texture.IsValid());
			return m_texture;
		}

		inline
		void Material::SetDetailMap(HTexture texture)
		{
			m_detail_map = texture;
		}

		inline
		bool Material::HasDetailMap()	const
		{
			return m_detail_map.IsValid();
		}

		inline
		HTexture	Material::GetDetailMap()
		{
			D_ASSERT(m_detail_map.IsValid());
			return m_detail_map;
		}

		inline
		void Material::SetEnvironmentMap(HTexture texture)
		{
			m_env_map = texture;
		}

		inline
		bool Material::HasEnvironmentMap()	const
		{
			return m_env_map.IsValid();
		}

		inline
		HTexture	Material::GetEnvironmentMap()
		{
			D_ASSERT(m_env_map.IsValid());
			return m_env_map;
		}

		inline
		void Material::SetLightMap(HTexture texture)
		{
			m_light_map = texture;
		}

		inline
		bool Material::HasLightMap()	const
		{
			return m_light_map.IsValid();
		}

		inline
		HTexture	Material::GetLightMap()
		{
			D_ASSERT(m_light_map.IsValid());
			return m_light_map;
		}
		// support for material chaining (for different materials on multiple rendering passes)
		inline
		void Material::SetNextMaterial(HMaterial material)
		{
			m_next = material;
		}

		inline
		bool Material::HasNextMaterial() const
		{
			return m_next.IsValid();
		}

		inline
		HMaterial Material::GetNextMaterial()
		{
			D_ASSERT(m_next.IsValid());
			return m_next;
		}

		inline
		Color& Material::Diffuse()
		{
			return m_diffuse;
		}

		inline
		Color& Material::Luminance()
		{
			return m_luminance;
		}

		inline
		Color& Material::Specular()
		{
			return m_specular;
		}

		inline
		Color& Material::BlendColor()
		{
			return m_blend;
		}

		inline
		uint32 Material::GetFlags() const
		{
			return m_flags;
		}

		inline
		void Material::AddFlags(MaterialFlags f)
		{
			m_flags = m_flags | f;
		}

		inline
		void Material::SetFlags(MaterialFlags f)
		{
			m_flags = f;
		}

} // dwarf

#endif // D_MATERIAL_H_
