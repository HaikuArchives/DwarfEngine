/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"
#include "Material.h"

using namespace dwarf;

Material::Material() 
	: RefCnt(), 
	m_texture(), 
	m_next(),
	m_flags(0),
	m_diffuse(1,1,1),
	m_luminance(1,1,1),
	m_specular(1,1,1),
	m_blend(1,1,1,1),
	m_detail_map(),
	m_env_map(),
	m_light_map()
{
}

Material::Material(HTexture texture) 
	: RefCnt(), 
	m_texture(texture),
	m_next(),
	m_flags(0),
	m_diffuse(1,1,1),
	m_luminance(1,1,1),
	m_specular(1,1,1),
	m_blend(1,1,1,1),
	m_detail_map(),
	m_env_map(),
	m_light_map()
{
}

Material::Material(Material const& m) 
	: RefCnt(), 
	m_texture(m.m_texture), 
	m_next(m.m_next),
	m_diffuse(m.m_diffuse),
	m_luminance(m.m_luminance),
	m_specular(m.m_specular),
	m_blend(m.m_blend),
	m_detail_map(m.m_detail_map),
	m_env_map(m.m_env_map),
	m_light_map(m.m_light_map)
{
}

Material::~Material()
{
}

