#ifndef	D_QUATERNION_H_
#define	D_QUATERNION_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Vertex.h"
#include "Euler.h"

namespace dwarf
{
		class Matrix4x4;
		class Quaternion;

		/*!
			\brief Quaternion class, used to represent rotations.
			A Quaternion can be represented as a scalar and a vector,
			as a scalar for the coefficient of 1 and a vector for the 
			coefficients of the imaginary terms.
		*/
		class Quaternion
		{
		friend class OutStream;
		friend class InStream;
		public:
			Quaternion();
			explicit Quaternion(float angle, const Vertex<3> &axis = Vertex<3>(0, 0, 1.0));
			Quaternion(const Quaternion &q);
			Quaternion(const Euler &rotation);

			float Length() const;
			const Quaternion &Normalize();

			void FromEuler(const Euler &rotation);

			void FromMatrix(const Matrix4x4 &m);
			Matrix4x4 ToMatrix() const;

			const Quaternion& FromAxis(const float angle, Vertex<3> axis);
			void ToAxis(float *angle, Vertex<3> *axis) const;

			const Quaternion &SLERP(Quaternion &to, Quaternion &from, float t);
			const Quaternion &LERP(Quaternion &to, Quaternion &from, float t);

			const Quaternion &Exp();
			const Quaternion &Log();

			Quaternion operator~() const;
			Quaternion operator-() const;
			Quaternion operator*(const Quaternion &q) const;
			Quaternion operator*(float scale) const;
			Quaternion operator+(const Quaternion &q) const;
			Quaternion operator-(const Quaternion &q) const;
			const Quaternion &operator*=(float scale);
			const Quaternion &operator*=(const Quaternion &q);
			const Quaternion &operator+=(const Quaternion &q);
			const Quaternion &operator+=(float angle);

		private:
			Quaternion(float s, const Vertex<3> &v, bool);

			Vertex<3> m_v;
			float m_s;
		};

		inline Quaternion::Quaternion():
			m_v(0.0, 0.0, 0.0),
			m_s(1.0)
		{
			Normalize();
		}

		inline Quaternion::Quaternion(float angle, const Vertex<3> &axis):
			m_v(0.0, 0.0, 0.0),
			m_s(1.0)
		{
			FromAxis(angle, axis);
		}

		inline Quaternion::Quaternion(float s, const Vertex<3> &v, bool):
			m_v(v),
			m_s(s)
		{
			Normalize();
		}

		inline Quaternion::Quaternion(const Quaternion &q):
			m_v(q.m_v),
			m_s(q.m_s)
		{
		}

		// create quaternion from euler angles stored as radians in vector "rotation"
		inline Quaternion::Quaternion(const Euler &rotation):
			m_v(0.0, 0.0, 0.0),
			m_s(1.0)
		{
			FromEuler(rotation);
		}
	
} // dwarf

#endif // D_QUATERNION_H_
