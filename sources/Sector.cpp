/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <list>
#include "mmgr.h"

#include "Defs.h"

#include "Sector.h"
#include "Camera.h"
#include "Entity.h"

using namespace dwarf;

Sector::Sector(Scene &scene):
	m_scene(scene)
{
}

Sector::Sector(Scene &scene,const String &filename):
	m_scene(scene)
{
}

Sector::~Sector()
{}

void Sector::AddEntity(int32 entity_id)
{
	// if the entity already belongs to a sector, remove it from that sector

	Entity &entity = m_scene.GetEntity(entity_id);
//	if (entity.HasSector()) entity.GetSector()->RemoveEntity(entity);
	entity.m_sector = this;
	entity.m_scene = &m_scene;
	entity.OnSectorAdd();
	m_entities.push_back(entity_id);
}

void Sector::RemoveEntity(int32 entity_id)
{
	m_entities.remove(entity_id);
	m_scene.RemoveEntity(entity_id);
}


void Sector::AddCamera(int32 camera_id)
{
	Camera &camera = m_scene.GetCamera(camera_id);
	camera.m_sector = this;
	camera.m_scene = &m_scene;
	m_cameras.push_back(camera_id);
}

void Sector::RemoveCamera(int32 camera_id)
{
	m_cameras.remove(camera_id);
	Camera &camera = m_scene.GetCamera(camera_id);
	camera.m_sector = 0;
	camera.m_scene = 0;
}

/*!
	\brief does nothing but draw all entities in the sector.

	There's no need to call this method when you implement your own, in a
	derived class.
*/
void Sector::DrawSector(RenderingDevice &device, const Camera &camera, int32 depth) const
{
	for (std::list<Portal*>::const_iterator i = m_portals.begin(); i != m_portals.end(); i++)
	{
		const Portal *portal = *i;
		portal->Draw(device, camera, depth);
	}
	
	for (std::list<int32>::const_iterator i = m_entities.begin(); i != m_entities.end(); i++)
	{
		const Entity &entity = m_scene.GetEntity(*i);
		entity.Invalidate(device, camera);
	}
}

void Sector::Tick(float frame_time)
{
}
