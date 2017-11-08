#ifndef D_SCENE_H
#define D_SCENE_H

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

#include "ListArray.h"
#include "Entity.h"
#include "String.h"

namespace dwarf
{

	// Forward declarations
	class Scene;
	class Sector;

	class Light;
	class Camera;
	
	class Node;
	class Entity;		// node with mesh, position, material, rotation etc
	class Dummy;		// dummy node, has position and rotation

	class Scene 
	{
	friend Sector;
	public:

		Scene();
		virtual ~Scene();

		void Tick(float frame_time);

		int32 AddSector(Sector* sector);
		int32 AddCamera(Camera* camera);

		Camera &GetCamera(int32 camera_id) { return *m_cameras[camera_id]; }
		Entity &GetEntity(int32 entity_id) { return *m_entities[entity_id]; }

		float Time() { return m_time; }


// DO NOT USE!
		int32 AddEntity(Entity* entity);
		void RemoveEntity(int32 id);

		typedef Entity *(entity_instantiation_function)(const Object &);

		static void register_entity(entity_instantiator *instantiator, const String &name)
		{
			char buffer[1000];
			name.ToChar(buffer, 1000);
			printf("registering entity: %s\n", buffer);
		
			m_entity_register[name] = instantiator;
		}
		
	private:


		ListArray<Sector*>	m_sectors;		// list of sectors in this scene
		ListArray<Entity*>	m_entities;		// list of all the entities in this scene
		ListArray<Camera*>	m_cameras;		// list of all cameras within this scene
		ListArray<Light*>	m_lights;		// list of all lights in this scene
		float				m_time;			// scene time, since first tick


		static std::map<String, entity_instantiator*> m_entity_register;
	};

} // dwarf

#endif // D_SCENE_H