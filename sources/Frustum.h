#ifndef D_FRUSTUM_H
#define D_FRUSTUM_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Serializable.h"
#include "Vertex.h"
#include "Plane.h"
#include "Object.h"
#include "Box.h"
#include "ListArray.h"

namespace dwarf
{

		class RenderingDevice;

		/*!
			\brief represents a frustum volume with a variable number of clip planes.
			\image html Frustum.gif
			\image html Frustum2.gif
		*/
		
		class Frustum : public Serializable
		{
		public:
			Frustum(const Vertex<3> &position = Vertex<3>(0,0,0));
			Frustum(const Frustum &frustum);
			virtual ~Frustum();

			int32 NumPlanes() const { return m_num_planes; }

			void SetNearClip(const Plane<3> &plane) { m_clip_plane[1] = plane; m_custom_near_plane = true; }
			void SetFarClip(const Plane<3> &plane) { m_clip_plane[0] = plane; }
			const Plane<3> &NearClip() const { return m_clip_plane[1]; }
			const Plane<3> &FarClip() const { return m_clip_plane[0]; }

			bool CustomNearPlane() const { return m_custom_near_plane; }

			Plane<3> &operator[](int32 index) { return m_plane[index]; }
			const Plane<3> &operator[](int32 index) const { return m_plane[index]; }

			int32 AddPlane(const Vertex<3> &first, const Vertex<3> &second);
			int32 AddPlane(const Vertex<3> &normal);
			void AddPlanes(const ListArray<Vertex<3> > &polygon);
			
			virtual void RotateBy(const Quaternion &rotation);
		
			virtual void MoveTo(const Vertex<3> &position);
			virtual void MoveBy(const Vertex<3> &delta);

			void Mirror(const Plane<3> &plane);
			const Vertex<3> &Position() const;

			const Array<Vertex<3> > PortalPoly() const { return m_portal; }
			int32 PortalPoints() const { return m_portal_points; }

		//	bool Contains(const uSphere &s);
			bool Contains(const Box &b) const;
			bool Contains(const Vertex<3> &v) const;
		//	bool Contains(const uTriangle &t);

			void SetTo(const Vertex<3> &position,const Quaternion &direction,float vertical_fov,float horizontal_fov, float near_clip, float far_clip);
		
			virtual void Restore(InStream& buffer) throw ();
			virtual void Serialize(OutStream& buffer) const;

		private:
			// position
			Vertex<3> m_position;
			Array<Plane<3> > m_plane;
			int32 m_num_planes;

			Plane<3> m_clip_plane[2]; // the far and near clip-planes. Far: 0 near: 1

			bool m_custom_near_plane; // true if the near clip plane was set using the SetNearChlip() instead of SetTo()

			Array<Vertex<3> > m_portal; // this is the polygon that builds the portal which this frustum was build from
			int32 m_portal_points; // this is the number of points in the array, if the frustum was not build from a portal-polygon, this is 0
		};
		
		// inline definitions
		
		/*!
			\brief tells you the position of the eye in the frustum.

			\return the position of the eye in the frustum.
		*/
		inline const Vertex<3> &Frustum::Position() const
		{
			return m_position;
		}

		/*!
			\brief destructor.
		*/
		inline Frustum::~Frustum()
		{
		}

		/*!
			\brief Adds a plane to the frustum.

			These two points will together with the eye's position form a plane that is added
			to the frustum. This is useful if you want to build a frustum that should clip only
			a portion of the screen. This method is used when drawing portals, to clip against the
			portal polygon.
			It's important that the points are given (as paramaters) in a clockwise order,
			otherwise the plane will be facing out of the frustum (culling everything within it).
			\param first the first point point.
			\param second the second point.
		*/
		inline int32 Frustum::AddPlane(const Vertex<3> &first, const Vertex<3> &second)
		{
			if (m_plane.Length() <= m_num_planes) m_plane.SetLength(m_num_planes + 4);
			m_plane[m_num_planes] = Plane<3>(m_position, first, second);
			return m_num_planes++;
		}
		
		/*!
			\brief Adds a plane to the frustum.

			The normal together with the eye's position will form a plane that is added
			to the frustum. Remember that the normal should point into the frustum volume, if
			it points out of the frustum everything inside the frustum is culled. i.e. Everything
			on the opposite side of the normal is culled.
			\param normal the normal of the plane to add.

		*/
		inline int32 Frustum::AddPlane(const Vertex<3> &normal)
		{
			if(m_plane.Length() < m_num_planes) m_plane.SetLength(m_num_planes + 4);
			m_plane[m_num_planes] = Plane<3>(m_position, normal);
			return m_num_planes++;
		}
		
		/*!
			\brief Checks if any point of a box lies within the frustum.

			\param box the box to check against the frustum.
			\return true if any point of the box is within the frustum.
			\warning note that parts of the box may intersect the frustum without any points within the frustum.
			\warning if the frustum contains no planes, thes method will always return true if the point is within the
			near and far planes.
		*/
		inline bool Frustum::Contains(const Box &box) const
		{
			// checking against the first four planes (left,right,top,bottom)
			for (int32 i=0; i < m_num_planes;i++)
			{
				const Plane<3> &plane = m_plane[i];
				if (plane.IsInside( box.origin) ) continue;
				if (plane.IsInside( box.origin + Vertex<3>(0, box.height, 0) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(0, box.height, box.depth) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(0, 0, box.depth) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, 0, 0) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, box.height, 0) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, box.height, box.depth) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, 0, box.depth) )) continue;
				return false;
			}
			// check against near and far
			for (int32 i=0; i < 2; i++)
			{
				const Plane<3> &plane = m_clip_plane[i];
				if (plane.IsInside( box.origin) ) continue;
				if (plane.IsInside( box.origin + Vertex<3>(0, box.height, 0) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(0, box.height, box.depth) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(0, 0, box.depth) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, 0, 0) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, box.height, 0) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, box.height, box.depth) )) continue;
				if (plane.IsInside( box.origin + Vertex<3>(box.width, 0, box.depth) )) continue;
				return false;
			}
			return true;
		}
		
		/*!
			\brief checks if a point is inside the frustum.

			\param Vertex<3> the point to check against the frustum.
			\return true if the point is inside the frustum and false if it's not.
			\warning if the frustum contains no planes, thes method will always return true if the point is within the
			near and far planes.
		*/
		inline bool Frustum::Contains(const Vertex<3> &point) const
		{
			// check if the point is on the outside of each plane, if it's on the outside, the point is outside the frustum,
			// and false is returned
			for (int32 i = 0; i < m_num_planes; i++)
				if (!m_plane[i].IsInside(point)) return false;

			// check near & far z-clip
			for (int32 i = 0; i < 2; i++)
				if (!m_clip_plane[i].IsInside(point)) return false;

			return true;
		}


} // dwarf

#endif // D_FRUSTUM_H
