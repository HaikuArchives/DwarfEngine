#ifndef D_MATRIX4X4_H_
#define D_MATRIX4X4_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Math.h"
#include "Serializable.h"
#include "Vertex.h"

namespace dwarf
{
	class Matrix4x4;

	typedef Matrix4x4 Matrix;

	/*!
		\brief A simple matrix class. Currently it's a full blown 4x4 matrix.

		We should make a 3x3 rotation matrix class as well.
	*/
	class Matrix4x4 : public Serializable
	{
	public:
		Matrix4x4() {}
		Matrix4x4(const Matrix4x4 &matrix);
		Matrix4x4(const Vertex<3> &a, const Vertex<3> &b, const Vertex<3> &c); // columns

		bool operator==(const Matrix4x4 &matrix)
		{
			for (int i = 0; i < 16; i++) if (m_elements[i] != matrix.m_elements[i]) return false;
			return true;
		}

		Matrix4x4 operator*(const Matrix4x4 &matrix) const;
		Vertex<3> operator*(const Vertex<3> &vector) const;

		// load the identity matrix
		void Identity();

		void Transpose();
		void Normalize();
		
		Vertex<3> Column(int index) const;
		Vertex<3> RightVector() const;
		Vertex<3> UpVector() const;
		Vertex<3> LookVector() const;

		// generate a rotation matrix from the vector and angle
		void Generate(float angle, Vertex<3> v);

		float &operator[](int32 index);
		float operator[](int32 index) const;
		const Matrix4x4 &operator*=(const Matrix4x4 &matrix);

		operator float* ();
		operator const float* () const;
		
		virtual void Restore(InStream &stream);
		virtual void Serialize(OutStream &stream) const;

	protected:
		// the matrix stored as 4 columns (0,1,2,3) is the first column
		float m_elements[16]; // da matrix
	};


	inline Matrix4x4::Matrix4x4(const Matrix4x4 &matrix)
	{
		memcpy(m_elements, matrix.m_elements, 16*sizeof(float));
	}
	
	inline float &Matrix4x4::operator[](int32 index)
	{
		D_ASSERT_MESSAGE(index < 16 && index >= 0, TEXT("matrix index out of range"));
		return m_elements[index];
	}


	inline float Matrix4x4::operator[](int32 index) const
	{
		D_ASSERT_MESSAGE(index < 16 && index >= 0, TEXT("matrix index out of range"));
		return m_elements[index];
	}


	inline const Matrix4x4 &Matrix4x4::operator*=(const Matrix4x4 &matrix)
	{
		return *this = *this * matrix;
	}
	

	inline Matrix4x4::operator float* ()
	{
		return m_elements;
	}


	inline Matrix4x4::operator const float* () const
	{
		return m_elements;
	}


	//! \brief identity

	class IdentityMatrix : public Matrix4x4
	{
	public:
		IdentityMatrix()
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
	};

	//! \brief scalematrix

	class ScaleMatrix : public Matrix4x4
	{
	public:
		ScaleMatrix(Vertex<3> scale);
	};

} // dwarf

#endif // D_MATRIX4X4_H_

