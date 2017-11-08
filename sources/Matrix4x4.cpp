/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "Matrix4x4.h"
#include "InStream.h"
#include "OutStream.h"
#include "Plane.h"

using namespace dwarf;

// initializes the columns
Matrix4x4::Matrix4x4(const Vertex<3> &a, const Vertex<3> &b, const Vertex<3> &c)
{
	m_elements[0] = a.x();
	m_elements[1] = a.y();
	m_elements[2] = a.z();
	m_elements[3] = 0.0f;

	m_elements[4] = b.x();
	m_elements[5] = b.y();
	m_elements[6] = b.z();
	m_elements[7] = 0.0f;

	m_elements[8] = c.x();
	m_elements[9] = c.y();
	m_elements[10] = c.z();
	m_elements[11]=0.0f;

	m_elements[12]=0.0f;
	m_elements[13]=0.0f;
	m_elements[14]=0.0f;
	m_elements[15]=1.0f;
}

void Matrix4x4::Identity()
{
	m_elements[0]=1.0f;
	m_elements[1]=0.0f;
	m_elements[2]=0.0f;
	m_elements[3]=0.0f;

	m_elements[4]=0.0f;
	m_elements[5]=1.0f;
	m_elements[6]=0.0f;
	m_elements[7]=0.0f;

	m_elements[8]=0.0f;
	m_elements[9]=0.0f;
	m_elements[10]=1.0f;
	m_elements[11]=0.0f;

	m_elements[12]=0.0f;
	m_elements[13]=0.0f;
	m_elements[14]=0.0f;
	m_elements[15]=1.0f;
}

// angle in radians
void Matrix4x4::Generate(float angle, Vertex<3> axis)
{
	float mag = axis.Length();
	if(fabs(mag) < D_EPSILON2)
	{
		Identity();
		return;
	}

	axis *= -mag;

	float s = sin(angle);
	float c = cos(angle);
	float one_minus_c = 1.0f - c;

	m_elements[ 0] = (one_minus_c * axis.x() * axis.x()) + c;
	m_elements[ 1] = (one_minus_c * axis.x() * axis.y()) - (axis.z() * s);
	m_elements[ 2] = (one_minus_c * axis.z() * axis.x()) + (axis.y() * s);
	m_elements[ 3] = 0.0f;

	m_elements[ 4] = (one_minus_c * axis.x() * axis.y()) + (axis.z() * s);
	m_elements[ 5] = (one_minus_c * axis.y() * axis.y()) + c;
	m_elements[ 6] = (one_minus_c * axis.y() * axis.z()) - (axis.x() * s);
	m_elements[ 7] = 0.0f;

	m_elements[ 8] = (one_minus_c * axis.z() * axis.x()) - (axis.y() * s);
	m_elements[ 9] = (one_minus_c * axis.y() * axis.z()) + (axis.x() * s);
	m_elements[10] = (one_minus_c * axis.z() * axis.z()) + c;
	m_elements[11] = 0.0f;

	m_elements[12] = 0.0f;
	m_elements[13] = 0.0f;
	m_elements[14] = 0.0f;
	m_elements[15] = 1.0f;
}

Vertex<3> Matrix4x4::operator*(const Vertex<3> &vector) const
{
	return Vertex<3>(
		m_elements[0] * vector.x() + m_elements[4] * vector.y() + m_elements[8] *vector.z() + m_elements[12], 
		m_elements[1] * vector.x() + m_elements[5] * vector.y() + m_elements[9] *vector.z() + m_elements[13],
		m_elements[2] * vector.x() + m_elements[6] * vector.y() + m_elements[10] *vector.z() + m_elements[14]);

}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &m2) const
{
	Matrix4x4 t;

	t.m_elements[0] = m_elements[0 ]*m2.m_elements[0 ] + m_elements[4 ]*m2.m_elements[1 ] + m_elements[8 ]*m2.m_elements[2 ] + m_elements[12]*m2.m_elements[3 ];
	t.m_elements[1] = m_elements[1 ]*m2.m_elements[0 ] + m_elements[5 ]*m2.m_elements[1 ] + m_elements[9 ]*m2.m_elements[2 ] + m_elements[13]*m2.m_elements[3 ];
	t.m_elements[2] = m_elements[2 ]*m2.m_elements[0 ] + m_elements[6 ]*m2.m_elements[1 ] + m_elements[10]*m2.m_elements[2 ] + m_elements[14]*m2.m_elements[3 ];
	t.m_elements[3] = m_elements[3 ]*m2.m_elements[0 ] + m_elements[7 ]*m2.m_elements[1 ] + m_elements[11]*m2.m_elements[2 ] + m_elements[15]*m2.m_elements[3 ];
	t.m_elements[4] = m_elements[0 ]*m2.m_elements[4 ] + m_elements[4 ]*m2.m_elements[5 ] + m_elements[8 ]*m2.m_elements[6 ] + m_elements[12]*m2.m_elements[7 ];
	t.m_elements[5] = m_elements[1 ]*m2.m_elements[4 ] + m_elements[5 ]*m2.m_elements[5 ] + m_elements[9 ]*m2.m_elements[6 ] + m_elements[13]*m2.m_elements[7 ];
	t.m_elements[6] = m_elements[2 ]*m2.m_elements[4 ] + m_elements[6 ]*m2.m_elements[5 ] + m_elements[10]*m2.m_elements[6 ] + m_elements[14]*m2.m_elements[7 ];
	t.m_elements[7] = m_elements[3 ]*m2.m_elements[4 ] + m_elements[7 ]*m2.m_elements[5 ] + m_elements[11]*m2.m_elements[6 ] + m_elements[15]*m2.m_elements[7 ];
	t.m_elements[8] = m_elements[0 ]*m2.m_elements[8 ] + m_elements[4 ]*m2.m_elements[9 ] + m_elements[8 ]*m2.m_elements[10] + m_elements[12]*m2.m_elements[11];
	t.m_elements[9] = m_elements[1 ]*m2.m_elements[8 ] + m_elements[5 ]*m2.m_elements[9 ] + m_elements[9 ]*m2.m_elements[10] + m_elements[13]*m2.m_elements[11];
	t.m_elements[10]= m_elements[2 ]*m2.m_elements[8 ] + m_elements[6 ]*m2.m_elements[9 ] + m_elements[10]*m2.m_elements[10] + m_elements[14]*m2.m_elements[11];
	t.m_elements[11]= m_elements[3 ]*m2.m_elements[8 ] + m_elements[7 ]*m2.m_elements[9 ] + m_elements[11]*m2.m_elements[10] + m_elements[15]*m2.m_elements[11];
	t.m_elements[12]= m_elements[0 ]*m2.m_elements[12] + m_elements[4 ]*m2.m_elements[13] + m_elements[8 ]*m2.m_elements[14] + m_elements[12]*m2.m_elements[15];
	t.m_elements[13]= m_elements[1 ]*m2.m_elements[12] + m_elements[5 ]*m2.m_elements[13] + m_elements[9 ]*m2.m_elements[14] + m_elements[13]*m2.m_elements[15];
	t.m_elements[14]= m_elements[2 ]*m2.m_elements[12] + m_elements[6 ]*m2.m_elements[13] + m_elements[10]*m2.m_elements[14] + m_elements[14]*m2.m_elements[15];
	t.m_elements[15]= m_elements[3 ]*m2.m_elements[12] + m_elements[7 ]*m2.m_elements[13] + m_elements[11]*m2.m_elements[14] + m_elements[15]*m2.m_elements[15];

	return t;
}

void Matrix4x4::Transpose()
{
	Matrix4x4 n(*this);

//	m_elements[0 ] = n.m_elements[0 ];
	m_elements[1 ] = n.m_elements[4 ];
	m_elements[2 ] = n.m_elements[8 ];
	m_elements[3 ] = n.m_elements[12];
	
	m_elements[4 ] = n.m_elements[1 ];
//	m_elements[5 ] = n.m_elements[5 ];
	m_elements[6 ] = n.m_elements[9 ];
	m_elements[7 ] = n.m_elements[13];

	m_elements[8 ] = n.m_elements[2 ];
	m_elements[9 ] = n.m_elements[6 ];
//	m_elements[10] = n.m_elements[10];
	m_elements[11] = n.m_elements[14];

	m_elements[12] = n.m_elements[3 ];
	m_elements[13] = n.m_elements[7 ];
	m_elements[14] = n.m_elements[11];
//	m_elements[15] = n.m_elements[15];
}

void Matrix4x4::Normalize()
{
	float inv_length;

	// first column
	inv_length = 1.0f/sqrt(m_elements[0]*m_elements[0] + m_elements[1]*m_elements[1] + m_elements[2]*m_elements[2] + m_elements[3]*m_elements[3]);
	m_elements[0]*=inv_length;
	m_elements[1]*=inv_length;
	m_elements[2]*=inv_length;
	m_elements[3]*=inv_length;
	// second column
	inv_length = 1.0f/sqrt(m_elements[4]*m_elements[4] + m_elements[5]*m_elements[5] + m_elements[6]*m_elements[6] + m_elements[7]*m_elements[7]);
	m_elements[4]*=inv_length;
	m_elements[5]*=inv_length;
	m_elements[6]*=inv_length;
	m_elements[7]*=inv_length;
	// third column
	inv_length = 1.0f/sqrt(m_elements[8]*m_elements[8] + m_elements[9]*m_elements[9] + m_elements[10]*m_elements[10] + m_elements[11]*m_elements[11]);
	m_elements[8]*=inv_length;
	m_elements[9]*=inv_length;
	m_elements[10]*=inv_length;
	m_elements[11]*=inv_length;
}

Vertex<3> Matrix4x4::Column(int index) const
{
	return Vertex<3>(&m_elements[index*4]);
}


Vertex<3> Matrix4x4::RightVector() const
{
	return Vertex<3>(&m_elements[0]);
}


Vertex<3> Matrix4x4::UpVector() const
{
	return Vertex<3>(&m_elements[4]);
}


Vertex<3> Matrix4x4::LookVector() const
{
	return Vertex<3>(&m_elements[8]);
}

void Matrix4x4::Restore(InStream &stream)
{
	for (int32 i = 0; i < 16; i++) stream >> m_elements[i];
}

void Matrix4x4::Serialize(OutStream &stream) const
{
	for (int32 i = 0; i < 16; i++) stream << m_elements[i];
}

ScaleMatrix::ScaleMatrix(Vertex<3> scale)
{
	m_elements[0]=scale.x();
	m_elements[1]=0.0f;
	m_elements[2]=0.0f;
	m_elements[3]=0.0f;

	m_elements[4]=0.0f;
	m_elements[5]=scale.y();
	m_elements[6]=0.0f;
	m_elements[7]=0.0f;

	m_elements[8]=0.0f;
	m_elements[9]=0.0f;
	m_elements[10]=scale.z();
	m_elements[11]=0.0f;

	m_elements[12]=0.0f;
	m_elements[13]=0.0f;
	m_elements[14]=0.0f;
	m_elements[15]=1.0f;
}
