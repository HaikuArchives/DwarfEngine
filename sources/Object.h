#ifndef D_OBJECT_H_
#define D_OBJECT_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Quaternion.h"
#include "Vertex.h"
#include "Serializable.h"
#include "ListArray.h"

namespace dwarf
{
		class Buffer;
		class Scene;
		class Sector;
		class Object;

		/*!
			\brief The base class for a geometric object (something that has a position and a rotation).
		*/
		class Object : public Serializable
		{
		friend class Sector;
		public:
	
			Object();
			Object(const Vertex<3> &position, const Quaternion &rotation = Quaternion());
			Object(const Vertex<3> &position, const Quaternion &rotation, Sector *sector);
			virtual ~Object();
	

			// translation
			const Vertex<3> &Position() const; // returns the position of the object
	
			virtual void MoveTo(const Vertex<3> &pos);
			void MoveTo(const Point &pos);
			virtual void MoveBy(const Vertex<3> &delta);
			void MoveBy(const Point &delta);

			void MoveForward(float distance);
			void MoveRight(float distance);
			void MoveUpward(float distance);


			// rotation

			void LookAt(const Vertex<3> &point);
			void LookAt(const Vertex<3> &lookat, const Vertex<3> &up_vector);

			const Quaternion &Rotation() const; // returns the object's rotation
	
			virtual void RotateTo(const Quaternion &rotation);
			virtual void RotateBy(const Quaternion &rotation);
	

			// serialize-related methods

			virtual void Restore(InStream &stream);
			virtual void Serialize(OutStream &stream) const;


			// parent / child handling methods

			Scene*	GetScene();
			Sector*	GetSector();
			Object*	GetParent();

			const Scene*	GetScene() const;
			const Sector*	GetSector() const;
			const Object*	GetParent() const;

			bool	HasScene() const;
			bool	HasSector() const;
			bool	HasParent() const;
			bool	HasChildren() const;

			int32	NumChildren() const;

		protected:
	
		
			Scene*				m_scene;	// owner scene
			Sector*				m_sector;	// owner sector (is this allowed to be 0? )
			Object*				m_parent;	// parent node (can be 0) (does this serve any purpose?)
			ListArray<Object*>	m_children;	// children nodes (does this sere a purpose?)

			Vertex<3>			m_position; // the current position
			Quaternion		m_rotation; // stores the object's rotation


		};



		// inline definitions
		
		inline const Vertex<3> &Object::Position() const
		{
			return m_position;
		}
		
		inline const Quaternion &Object::Rotation() const
		{
			return m_rotation;
		}
		
		inline void Object::MoveBy(const Point &delta)
		{
			MoveBy(Vertex<3>(delta.x(), delta.y(), 0.0));
		}
		
		inline void Object::MoveTo(const Point &pos)
		{
			MoveTo(Vertex<3>(pos.x(), pos.y(), 0.0));
		}
		
		inline Scene* Object::GetScene()
		{
			D_ASSERT_MESSAGE(m_scene, TEXT("GetScene() called on object not attached to a scene!"));
			return m_scene;
		}

		inline Sector* Object::GetSector()
		{
			D_ASSERT_MESSAGE(m_sector, TEXT("GetSector() called on object not attached to a sector!"));
			return m_sector;
		}

		inline Object* Object::GetParent()
		{
			D_ASSERT_MESSAGE(m_parent, TEXT("GetParent() called on parentless object!"));
			return m_parent;
		}

		inline const Scene* Object::GetScene() const
		{
			D_ASSERT_MESSAGE(m_scene, TEXT("GetScene() called on object not attached to a scene!"));
			return m_scene;
		}
		
		inline const Sector* Object::GetSector() const
		{
			D_ASSERT_MESSAGE(m_sector, TEXT("GetSector() called on object not attached to a sector!"));
			return m_sector;
		}
		
		inline const Object* Object::GetParent() const
		{
			D_ASSERT_MESSAGE(m_parent, TEXT("GetParent() called on parentless object!"));
			return m_parent;
		}

		inline int32 Object::NumChildren() const
		{
			return m_children.ItemCount();
		}

		inline bool	Object::HasScene() const
		{
			return m_scene != 0;
		}

		inline bool	Object::HasSector() const
		{
			return m_sector != 0;
		}

		inline bool	Object::HasParent() const
		{
			return m_parent != 0;
		}

		inline bool	Object::HasChildren() const
		{
			return NumChildren() != 0;
		}

} // dwarf

#endif // D_OBJECT_H_
