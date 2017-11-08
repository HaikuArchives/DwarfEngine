#ifndef D_CAMERA_H
#define D_CAMERA_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Frustum.h"
#include "Object.h"
#include "Vertex.h"

namespace dwarf
{

		class RenderingDevice;
		class Quaternion;

		/*!
			\brief A camera exists in a sector and can draw what it sees.

			It reqires a sector.
		*/
		class Camera : public Object
		{
		public:
			Camera();
			Camera(const Object &object);
			virtual ~Camera();
		
			virtual void MoveTo(const Vertex<3> &pos);
			virtual void MoveBy(const Vertex<3> &delta);
		
			virtual void RotateTo(const Quaternion &rotation);
			virtual void RotateBy(const Quaternion &rotation);
		
			virtual void Draw(RenderingDevice &device/*, uList<uLight*> *lightsources*/) const;
			virtual void Draw(RenderingDevice &device/*, uList<uLight*> *lightsources*/, int depth) const;
		
			void Mirror(const Plane<3> &plane);

			// sets the far z clip
			void SetFarClip(float far_clip);
			float FarClip() const;
		
			// sets the near z clip
			void SetNearClip(float near_clip);
			float NearClip() const;
		
			// sets the field of vision, vertical and horizontal.
			// if in frustum mode they are interpreted as two radian angles
			// if in orthogonal mode the are interpreted as width and height of the view-volume 
			void SetFov(float horizontal_fov, float vertical_fov);
			float HorizontalFov() const;
			float VerticalFov() const;
			
			const Frustum &ViewFrustum() const;
			void SetFrustum(const Frustum &frustum);
		
			bool OrthogonalMode();
			void SetOrthogonalMode(bool set);

			const Vertex<3> &VirtualPosition() const;
			void SetVirtualPosition(const Vertex<3> &position);
		
		private:
			Frustum m_frustum;
			float m_far_clip;
			float m_near_clip;
			float m_vfov;
			float m_hfov;

			Vertex<3> m_virtual_position;
		};
		
		// inline definitions
		
		inline void Camera::SetFarClip(float far_clip)
		{
			m_far_clip = far_clip;
			Plane<3> plane = m_frustum.FarClip();
			plane.MoveTo(plane.Normal() * far_clip + m_position);
			//! \todo potential bug! the frustum will think that this plane is a custom plane
			m_frustum.SetFarClip(plane);
		}
		
		inline float Camera::FarClip() const
		{
			return m_far_clip;
		}
		
		inline void Camera::SetNearClip(float near_clip)
		{
			m_near_clip = near_clip;
			Plane<3> plane = m_frustum.NearClip();
			plane.MoveTo(-plane.Normal() * near_clip + m_position);
			//! \todo potential bug! the frustum will think that this plane is a custom plane
			m_frustum.SetNearClip(plane);
		}
		
		inline float Camera::NearClip() const
		{
			return m_near_clip;
		}
		
		inline void Camera::SetFov(float horizontal_fov, float vertical_fov)
		{
			m_hfov = horizontal_fov;
			m_vfov = vertical_fov;
			m_frustum.SetTo(Position(), Rotation(), m_vfov, m_hfov, m_near_clip, m_far_clip);
		}
		
		inline float Camera::HorizontalFov() const
		{
			return m_hfov;
		}
		
		inline float Camera::VerticalFov() const
		{
			return m_vfov;
		}
		
		inline const Frustum &Camera::ViewFrustum() const
		{
			return m_frustum;
		}

		inline void Camera::SetFrustum(const Frustum &frustum)
		{
			m_frustum = frustum;
		}

		/*!
			If you implement a sector that uses the camera position
			cor culling, use the virtual position instead. Because
			if you sector is seen through a portal, the real camera
			position migth end up in the wrong pvs-leaf or entirely
			outside the sector. The virtual position is the camera's
			real position (in normal cases) or the portal target when
			looking through a portal.
			\sa SetVirtualPosition()
		*/
		inline const Vertex<3> &Camera::VirtualPosition() const
		{
			return m_virtual_position;
		}

		/*!
			This is used in quakesector to get the correct pvs leaf for the camera when
			looking through a portal. otherwise the camera might end up outside all
			leafs, and screw up pvs.
			\sa VirtualPosition()
		*/
		inline void Camera::SetVirtualPosition(const Vertex<3> &position)
		{
			m_virtual_position = position;
		}

} // dwarf

#endif // D_CAMERA_H
