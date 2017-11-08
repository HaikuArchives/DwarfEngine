#ifndef	D_EULER_
#define	D_EULER_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

namespace dwarf
{
	/*!
		Represents an euler angle
	*/

	class Euler
	{
	public:
		Euler();
		Euler(const Euler &euler);
		Euler(float _pitch,float _heading,float _bank);

		//! component add
		Euler operator+(const Euler &e) const;
		//! component substract
		Euler operator-(const Euler &e) const;
		//! component multiplication
		Euler operator*(const Euler &e) const;
		//! component division
		Euler operator/(const Euler &e) const;
		//! scalar multiplication
		Euler operator*(float s) const;
		//! unary neg
		Euler operator-() const;

		//! component add assignment
		const Euler &operator+=(const Euler &e);
		//! component substract assignment
		const Euler &operator-=(const Euler &e);
		//! component multiplication assignment
		const Euler &operator*=(const Euler &e);
		//! component division assignment
		const Euler &operator/=(const Euler &e);
		//! scalar multiplication assignment
		const Euler &operator*=(float s);

		//! assignment
		const Euler &operator=(const Euler &e);

		float pitch;
		float heading;
		float bank;
	};

	inline Euler::Euler():
		pitch(0.0f),
		heading(0.0f),
		bank(0.0f)
	{
	}

	inline Euler::Euler(const Euler &euler):
		pitch(euler.pitch),
		heading(euler.heading),
		bank(euler.bank)
	{
	}

	inline Euler::Euler(float _pitch,float _heading,float _bank):
		pitch(_pitch),
		heading(_heading),
		bank(_bank)
	{
	}
		
	inline Euler Euler::operator+(const Euler &e) const
	{
		return Euler(pitch+e.pitch,heading+e.heading,bank+e.bank);
	}

	inline Euler Euler::operator-(const Euler &e) const
	{
		return Euler(pitch-e.pitch,heading-e.heading,bank-e.bank);
	}

	inline Euler Euler::operator*(const Euler &e) const
	{
		return Euler(pitch*e.pitch,heading*e.heading,bank*e.bank);
	}

	inline Euler Euler::operator/(const Euler &e) const
	{
		return Euler(pitch/e.pitch,heading/e.heading,bank/e.bank);
	}

	inline Euler Euler::operator*(float s) const
	{
		return Euler(pitch*s,heading*s,bank*s);
	}

	inline Euler Euler::operator-() const
	{
		return Euler(-pitch,-heading,-bank);
	}
	
	inline const Euler &Euler::operator=(const Euler &e)
	{
		pitch = e.pitch;
		heading = e.heading;
		bank = e.bank;

		return *this;
	}

	inline const Euler &Euler::operator+=(const Euler &e)
	{
		pitch += e.pitch;
		heading += e.heading;
		bank += e.bank;

		return *this;
	}

	inline const Euler &Euler::operator-=(const Euler &e)
	{
		pitch -= e.pitch;
		heading -= e.heading;
		bank -= e.bank;

		return *this;
	}

	inline const Euler &Euler::operator*=(const Euler &e)
	{
		pitch *= e.pitch;
		heading *= e.heading;
		bank *= e.bank;

		return *this;
	}


	inline const Euler &Euler::operator/=(const Euler &e)
	{
		pitch /= e.pitch;
		heading /= e.heading;
		bank /= e.bank;

		return *this;
	}


	inline const Euler &Euler::operator*=(float s)
	{
		pitch *= s;
		heading *= s;
		bank *= s;

		return *this;
	}

} // dwarf

#endif