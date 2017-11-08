/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Euler.h"
	
using namespace dwarf;


// SLERP - Spherical interpolation between two quaternions
const Quaternion &Quaternion::SLERP(Quaternion &to, Quaternion &from, float t)
{
	Quaternion to1;

	// calc cosine
	float cosom = from.m_v.DotProduct(to.m_v) + from.m_s * to.m_s;

	// adjust signs (if necessary)
	if(cosom < 0.0)
	{
		cosom = -cosom;
		to1.m_v = - to.m_v;
		to1.m_s = - to.m_s;
	}
	else
	{
		to1.m_v = to.m_v;
		to1.m_s = to.m_s;
	}

	// calculate coefficients
	float scale0, scale1;

	if ( (1.0 - cosom) > D_EPSILON )
	{
		// standard case (slerp)
		double omega = acos(cosom);
		double sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	}
	else
	{
		// "from" and "to" quaternions are very close
		//  ... so we can do a linear interpolation
		scale0 = 1.0 - t;
		scale1 = t;
	}
	// calculate final values
	m_v = from.m_v * scale0 + to1.m_v * scale1;
	m_s = from.m_s * scale0 + to1.m_s * scale1;

	return *this;
}

// LERP - Linear interpolation between two quaternions
const Quaternion &Quaternion::LERP(Quaternion &to, Quaternion &from, float t)
{
	Quaternion to1;

	// calc cosine
	float cosom = from.m_v.DotProduct(to.m_v) + from.m_s * to.m_s;

	// adjust signs (if necessary)
	if( cosom < 0.0 )
	{
		to1.m_v = - to.m_v;
		to1.m_s = - to.m_s;
	}
	else
	{
		to1.m_v = to.m_v;
		to1.m_s = to.m_s;
	}

	// interpolate linearly
	float scale0 = 1.0 - t;
	float scale1 = t;
 
	// calculate final values
	m_v = from.m_v * scale0 + to1.m_v * scale1;
	m_s = from.m_s * scale0 + to1.m_s * scale1;

	return *this;
}

// create quaternion from euler angles stored as radians in vector "rotation"
void Quaternion::FromEuler(const Euler &rotation)
{
	float ti = rotation.pitch * (float)0.5;
	float tj = rotation.heading * (float)0.5;
	float tk = rotation.bank * (float)0.5;

	Quaternion qx(cos(ti), Vertex<3>(sin(ti), 0, 0), true);
	Quaternion qy(cos(tj), Vertex<3>(0, sin(tj), 0), true);
	Quaternion qz(cos(tk), Vertex<3>(0, 0, sin(tk)), true);

	*this = qz * qx * qy;

	Normalize();
}

/*!
	\todo I believe this method reads the matrix transposed.
*/
void Quaternion::FromMatrix(const Matrix4x4 &m)
{
	float  q[4];
	int nxt[3] = {1, 2, 0};
	
	float tr = m[0] + m[5] + m[10];

	// check the diagonal
	if (tr > 0.0)
	{
		float s = sqrt (tr + 1.0);
		m_s = s / 2.0;
		s = 0.5 / s;
		m_v.x() = (m[9] - m[6]) * s;
		m_v.y() = (m[2] - m[8]) * s;
		m_v.z() = (m[4] - m[1]) * s;
	}
	else
	{
		// diagonal is negative
		int i = 0;
		if (m[5] > m[0]) i = 1;
		if (m[10] > m[i*5]) i = 2;
		int j = nxt[i];
		int k = nxt[j];

		float s = sqrt ((m[(i*4)+i] - (m[(j*4)+j] + m[(k*4)+k])) + 1.0);

		q[i] = s * 0.5;
		if (s != 0.0) s = 0.5 / s;

		q[3] = (m[(k*4)+j] - m[(j*4)+k]) * s;
		q[j] = (m[(j*4)+i] + m[(i*4)+j]) * s;
		q[k] = (m[(k*4)+i] + m[(i*4)+k]) * s;

		m_v.x() = q[0];
		m_v.y() = q[1];
		m_v.z() = q[2];
		m_s = q[3];
	}

}

Matrix Quaternion::ToMatrix() const
{
	Matrix r;

	// calculate coefficients
	float x2 = m_v.x() + m_v.x();
	float y2 = m_v.y() + m_v.y(); 
	float z2 = m_v.z() + m_v.z();
	float xx = m_v.x() * x2;
	float xy = m_v.x() * y2;
	float xz = m_v.x() * z2;
	float yy = m_v.y() * y2;
	float yz = m_v.y() * z2;
	float zz = m_v.z() * z2;
	float sx = m_s * x2;
	float sy = m_s * y2;
	float sz = m_s * z2;

	r[0] = 1.0 - (yy + zz);
	r[4] = xy - sz;
	r[8] = xz + sy;
	r[12] = 0.0;
 
	r[1] = xy + sz;
	r[5] = 1.0 - (xx + zz);
	r[9] = yz - sx;
	r[13] = 0.0;

	r[2] = xz - sy;
	r[6] = yz + sx;
	r[10] = 1.0 - (xx + yy);
	r[14] = 0.0;

	r[3] = 0;
	r[7] = 0;
	r[11] = 0;
	r[15] = 1;

	
	return r;
}

const Quaternion& Quaternion::Exp()
{
	float length = m_v.Length();

	float mul;
	if (length > D_EPSILON) mul = sin(length)/length;
	else mul = 1.0;

	m_s = cos(length);
	m_v *= mul;

	Normalize();
	return *this;
}

const Quaternion& Quaternion::Log()
{
	float length;

	length = atan(m_v.Length()/m_s);
	m_s = 0.0;
	m_v *= length;

	Normalize();
	return *this;
}

// normalizes the quaternion
const Quaternion& Quaternion::Normalize()
{
	float square = m_s*m_s + m_v.DotProduct(m_v);
	
	if (square > 0.0)
	{
		float inv_dist = (float)1.0 / sqrt(square);
		m_v *= inv_dist;
		m_s *= inv_dist;
	}
	return *this;
}

// magnitude of quaternion
float Quaternion::Length() const
{
	return sqrt(m_s*m_s + m_v.x()*m_v.x() + m_v.y()*m_v.y() + m_v.z()*m_v.z());
}

// returns a rotation on the opposite direction (conjugate)
Quaternion Quaternion::operator~() const
{
	if (m_v == Vertex<3>(0,0,0)) return *this;
	
	float inv_norm = 1.0 / (m_s * m_s * m_v.DotProduct(m_v));
	return Quaternion(m_s * inv_norm, -m_v * inv_norm, true);
}

// unary minus
Quaternion Quaternion::operator-() const
{	// yes, this is the same as the conjugate
	// NOTE: that sentiment is only true if
	// the quaternion is an unit quaternion
	// (however, we know our quaternion to be one
	// since we renormalize it after every "dangerous"
	// operation).
	if (m_v == Vertex<3>(0,0,0)) return *this;

	float inv_norm = 1.0 / (m_s * m_s * m_v.DotProduct(m_v));
	return Quaternion(m_s * inv_norm, -m_v * inv_norm, true);
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
	Quaternion tmp;
	tmp.m_v.x() =	q.m_s * m_v.x() + q.m_v.x() * m_s + q.m_v.y() * m_v.z() - q.m_v.z() * m_v.y();
	tmp.m_v.y() =	q.m_s * m_v.y() + q.m_v.y() * m_s + q.m_v.z() * m_v.x() - q.m_v.x() * m_v.z();
	tmp.m_v.z() =	q.m_s * m_v.z() + q.m_v.z() * m_s + q.m_v.x() * m_v.y() - q.m_v.y() * m_v.x();
	tmp.m_s   =	q.m_s * m_s - q.m_v.x() * m_v.x() - q.m_v.y() * m_v.y() - q.m_v.z() * m_v.z();
	tmp.Normalize();

	return tmp;
//  A less optimized but more readable version: NOT WORKING for some reason (not normalized)
//	return Quaternion((m_s*q.m_s) - m_v.DotProduct(q.m_v), (q.m_v*m_s) + (m_v*q.m_s) + m_v.CrossProduct(q.m_v));
}

Quaternion Quaternion::operator*(float scale) const
{
	Quaternion ret;
	Vertex<3> axis;
	float angle;
	ToAxis(&angle, &axis);
	ret.FromAxis(angle * scale, axis);
	return ret;
}

Quaternion Quaternion::operator+(const Quaternion &q) const
{
	Quaternion ret(m_s + q.m_s, m_v + q.m_v, true);
	ret.Normalize();
	return ret;
}

Quaternion Quaternion::operator-(const Quaternion &q) const
{
//	Quaternion ret(*this);
//	ret.Normalize();
//	ret.m_v = -ret.m_v;
//	return ret;
	return *this + (-q);
}

const Quaternion &Quaternion::operator*=(float scale)
{
	Vertex<3> axis;
	float angle;
	ToAxis(&angle, &axis);
	FromAxis(angle * scale, axis);
	return *this;
}

const Quaternion &Quaternion::operator*=(const Quaternion &q1)
{
	Quaternion q2(*this);

	m_v.x() = q1.m_s * q2.m_v.x() + q1.m_v.x() * q2.m_s + q1.m_v.y() * q2.m_v.z() - q1.m_v.z() * q2.m_v.y();
	m_v.y() = q1.m_s * q2.m_v.y() + q1.m_v.y() * q2.m_s + q1.m_v.z() * q2.m_v.x() - q1.m_v.x() * q2.m_v.z();
	m_v.z() = q1.m_s * q2.m_v.z() + q1.m_v.z() * q2.m_s + q1.m_v.x() * q2.m_v.y() - q1.m_v.y() * q2.m_v.x();
	m_s = q1.m_s * q2.m_s - q1.m_v.x() * q2.m_v.x() - q1.m_v.y() * q2.m_v.y() - q1.m_v.z() * q2.m_v.z();

	Normalize();
	return *this;
/*
	float s = (m_s*q.m_s) - m_v.DotProduct(q.m_v);
	m_v = (q.m_v*m_s) + (m_v*q.m_s) + m_v.CrossProduct(q.m_v);
	m_s = s;
	Normalize();
	return *this;
*/
}

const Quaternion &Quaternion::operator+=(const Quaternion &q)
{
	m_s += q.m_s;
	m_v += q.m_v;
	Normalize();
	return *this;
}

const Quaternion &Quaternion::operator+=(float d_angle)
{
	Vertex<3> axis;
	float angle;
	ToAxis(&angle, &axis);
	FromAxis(angle + d_angle, axis);
	return *this;
}

const Quaternion& Quaternion::FromAxis(const float angle, Vertex<3> axis)
{
	axis.Normalize();
	float half_angle = angle * .5;
	m_v = axis * sin(half_angle);
	m_s = cos(half_angle);

	Normalize();
	return *this;
}

void Quaternion::ToAxis(float *angle, Vertex<3> *axis) const
{
	if (m_v.Length2() > D_EPSILON2)
	{
		*angle = (float)acos(m_s) * 2;
//		float scale = (float)sin(*angle / 2.0);
//		*axis = m_v / scale;

		*axis = m_v;
		(*axis).Normalize();
	}
	else
	{
		axis->x() =  0.0;
		axis->y() =  0.0;
		axis->z() = -1.0;
		*angle =   0.0;
	}
}



