#ifndef D_SECTOR_H
#define D_SECTOR_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <map>

#include "Defs.h"

#include "Scene.h"
#include "Entity.h"
#include "Portal.h"
//#include "SkyBox.h"


namespace dwarf
{

	class Sector
	{
	friend class Entity;
	friend class Camera;
	public:
		Sector(Scene &scene);
		Sector(Scene &scene,const String &filename);
		virtual ~Sector();

		Entity *spawn(const String &name, const Vertex<3> &position = Vertex<3>(0,0,0), const Quaternion &rotation = Quaternion())
		{
			Entity *entity = (*Scene::m_entity_register[name])(Object(position, rotation, this));
			int32 id = m_scene.AddEntity(entity);
			AddEntity(id);
			return entity;
		}

		Entity *spawn(entity_instantiator &instantiator, const Vertex<3> &position = Vertex<3>(0,0,0), const Quaternion &rotation = Quaternion())
		{
			Entity *entity = instantiator(Object(position, rotation, this));
			int32 id = m_scene.AddEntity(entity);
			AddEntity(id);
			return entity;
		}

//	protected:

		virtual void Tick(float frame_time);

		// used only by Entity
		void AddEntity(int32 entity_id);
		void RemoveEntity(int32 entity_id);

		// used only by camera
		void AddCamera(int32 camera_id);
		void RemoveCamera(int32 camera_id);
		virtual void DrawSector(RenderingDevice &device, const Camera &frustum, int32 depth) const;

		void AddPortal(Portal *portal) {m_portals.push_back(portal); }

		// array access methods

		const std::list<int32> &Entities() const { return m_entities; }
		std::list<int32> &Entities() { return m_entities; }
		const std::list<Portal*> &Portals() const { return m_portals; }
		std::list<Portal*> &Portals() { return m_portals; }
		
		// load/unload, used by scene
		virtual void Load(float time) {};
		virtual void Unload() {};

		Scene &GetScene() const { return m_scene; }

	private:

		Scene&				m_scene;		// the scene this sector belongs to
		std::list<int32>	m_entities;		// entities within this sector
		std::list<int32>	m_cameras;		// a list of cameras
		std::list<Portal*>	m_portals;		// a list of all the portals in this sector
//		std::list<Light*>	m_lights;		// lights within this sector
//		SkyBox			m_skybox;		// the skybox for this sector
	};

} // namespace dwarf

#endif // D_SECTOR_H