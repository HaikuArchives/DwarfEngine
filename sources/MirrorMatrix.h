#ifndef	D_MIRROR_MATRIX_H_
#define	D_MIRROR_MATRIX_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Plane.h"
#include "Matrix4x4.h"
#include "Vertex.h"

namespace dwarf
{
	
	class MirrorMatrix : public Matrix4x4
	{
	public:
		MirrorMatrix(const Plane<3> &plane)
		{
			Vertex<3> normal = plane.Normal();
			Vertex<3> point = plane.Normal() * (2 * -plane.D());

 
			m_elements[0]  = 1 - 2 * normal[0]	* normal[0];
			m_elements[1]  =   - 2 * normal[1]	* normal[0];
			m_elements[2]  =   - 2 * normal[2]	* normal[0];
			m_elements[3]  = 0;

			m_elements[4]  =   - 2 * normal[0]	* normal[1];
			m_elements[5]  = 1 - 2 * normal[1]	* normal[1];
			m_elements[6]  =   - 2 * normal[2]	* normal[1];
			m_elements[7]  = 0;

			m_elements[8]  =   - 2 * normal[0]	* normal[2];
			m_elements[9]  =   - 2 * normal[1]	* normal[2];
			m_elements[10] = 1 - 2 * normal[2]	* normal[2];
			m_elements[11] = 0;

			m_elements[12] = point[0];
			m_elements[13] = point[1];
			m_elements[14] = point[2];
			m_elements[15] = 1;

		}

		MirrorMatrix(const Plane<3> &plane, const Matrix4x4 &transformation)
		{
			Vertex<3> t = transformation.Column(3);
			
			Vertex<3> n2 = plane.Normal() * -2;
			Vertex<3> mt = t + n2 * (t * plane.Normal() - plane.D());

			Vertex<3> x = transformation.Column(0) + t;
			x += n2 * (x * plane.Normal()) - mt;
			Vertex<3> y = transformation.Column(1) + t;
			y += n2 * (y * plane.Normal()) - mt;
			Vertex<3> z = transformation.Column(2) + t;
			z += n2 * (z * plane.Normal()) - mt;
			

			m_elements[0]  = x.x();
			m_elements[1]  = x.y();
			m_elements[2]  = x.z();
			m_elements[3]  = 0;
			
			m_elements[4]  = y.x();
			m_elements[5]  = y.y();
			m_elements[6]  = y.z();
			m_elements[7]  = 0;
			
			m_elements[8]  = z.x();
			m_elements[9]  = z.y();
			m_elements[10] = z.z();
			m_elements[11] = 0;
			
			m_elements[12] = t.x();
			m_elements[13] = t.y();
			m_elements[14] = t.z();
			m_elements[15] = 1;
			
		}
		
	};

}



#endif // D_MIRROR_MATRIX_H_