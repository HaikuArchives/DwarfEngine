#ifndef D_ENTITY_H
#define D_ENTITY_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Object.h"
#include "RenderingDevice.h"
#include "Frustum.h"
#include "Matrix4x4.h"

namespace dwarf
{

	class Camera;
	
	/*!
		\brief This class has not been fully documented yet.
		\todo document Entity.
	*/
	class Entity : public Object, public RefCnt
	{
	friend class Scene;
	friend class Sector;
	public:
		Entity();
		Entity(const Object &object);
		virtual ~Entity();
					
		void Invalidate(RenderingDevice &device, const Camera &frustum/*, uList<uLight*> *lightsources*/) const;
	protected:
		virtual void Draw(RenderingDevice &device, const Camera &frustum/*, uList<uLight*> *lightsources*/) const;
		virtual void Tick(float frame_time);
		virtual void OnTimer();
		virtual void OnSectorAdd();
		virtual void OnSectorRemove();
		
		void SetTimer(float time);
		float NextTimerEvent();
	private:
		float m_timer_event;
	};
	
	// inline definitions
	
	inline void Entity::Invalidate(RenderingDevice &device, const Camera &frustum) const
	{
		device.PushMatrix();
		device.Translate(Position());
		device.Rotate(Rotation());

		Draw(device, frustum);
		D_ASSERT_MESSAGE(device.GetTextureMatrix() == IdentityMatrix(), TEXT("Your entity did not reset the texture matrix to identity, you have to do that"));
	
		device.PopMatrix();
	}

	inline float Entity::NextTimerEvent()
	{
		return m_timer_event;
	}

	struct entity_instantiator
	{
		virtual Entity *operator()(const Object &obj) = 0;
	};		

} // dwarf



#define REGISTER_ENTITY(x) \
namespace name\
{\
	struct __##x##_instantiator : public dwarf::entity_instantiator \
	{ \
		__##x##_instantiator::__##x##_instantiator() { Scene::register_entity(this, TEXT(##"##x##"##)); } \
		Entity *operator()(const Object &obj) { return new x(obj); } \
	}; \
	static __##x##_instantiator x;\
}


#endif // D_ENTITY_H
