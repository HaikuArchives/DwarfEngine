/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Scene.h"
#include "Entity.h"
#include "Sector.h"

using namespace dwarf;


std::map<String, entity_instantiator*> Scene::m_entity_register;

Scene::Scene():
	m_time(0.0f)
{
}

Scene::~Scene()
{
	for (ListArray<Entity*>::Iterator i = m_entities.Begin(); i.IsValid(); i++)
	{
		delete m_entities.RemoveItemAt(i);
	}
}

void Scene::Tick(float frame_time)
{
	float old_time = m_time;
	m_time += frame_time;

	for (ListArray<Entity*>::Iterator i = m_entities.Begin(); i.IsValid(); i++)
	{
		(*i)->Tick(frame_time);
	}

	for (ListArray<Sector*>::Iterator i = m_sectors.Begin(); i.IsValid(); i++)
		(*i)->Tick(frame_time);

	for (ListArray<Entity*>::Iterator i = m_entities.Begin(); i.IsValid(); i++)
	{
		Entity *ent = *i;

		if((ent->NextTimerEvent() >= old_time) && (ent->NextTimerEvent() <= m_time))
			ent->OnTimer();
	}
}

int32 Scene::AddSector(Sector* sector)
{
	return m_sectors.AddItem(sector);
}

int32 Scene::AddEntity(Entity* entity)
{
	return m_entities.AddItem(entity);
}

void Scene::RemoveEntity(int32 id)
{
	Entity *entity = m_entities.RemoveItemAt(id);
	entity->OnSectorRemove();
	entity->m_sector = 0;
	entity->m_scene = 0;
	delete entity;
}

int32 Scene::AddCamera(Camera* camera)
{
	return m_cameras.AddItem(camera);
}

