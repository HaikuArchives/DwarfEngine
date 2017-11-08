#ifndef D_STATICMESH_H_
#define D_STATICMESH_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Material.h"

namespace dwarf
{

		/*!
			\brief A static mesh, probably stored as a display list or similar
		*/
		class StaticMesh
		{
		public:
			StaticMesh();
			~StaticMesh();

		private:
			HMaterial m_material;
		};

} // dwarf

#endif // D_STATICMESH_H_
