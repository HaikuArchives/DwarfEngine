#ifndef D_COREMATH_H_
#define D_COREMATH_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include <math.h>

namespace dwarf
{

		const float D_EPSILON = 1e-3f;
		const float D_EPSILON2 = 1e-9f;
		const float D_PI = 3.1415926f;
		const float D_2PI = 6.28318530f;

		/*!
			\brief converts radians to degrees
			\param rad the number of radians
			\return the eqvivalent number of degrees
		*/
		inline float RadToDeg(float rad)
		{
			return rad * 57.295779513082321f; // 180/pi
		}
		
		/*!
			\brief converts degrees to radians
			\param deg the number of degrees
			\return the eqvivalent number of radians
		*/
		inline float DegToRad(float deg)
		{
			return deg * 0.0174532925199433f; // pi/180
		}
		
		/*!
			\brief makes sure the value stays within the given floor and cieling
			\param val the value
			\param floor the floor value (val may not be less than this value)
			\param cieling the cieling value (val may not be greater than this value)
		*/
		template <class T>
		inline T Clamp(T val, T floor, T cieling)
		{
			if (val < floor) return floor;
			if (val > cieling) return cieling;
			return val;
		}

#ifndef min

		/*!
			\brief selects the smallesr value of the two given, and returns it
			\param val1 one of the values
			\param val2 the other value
			\return the smallest one of val1 and val2
		*/
		template <class T>
		inline T min(T val1, T val2)
		{
			if (val1 < val2) return val1;
			return val2;
		}

#endif
#ifndef max

		/*!
			\brief selects the largest value of the two given, and returns it
			\param val1 one of the values
			\param val2 the other value
			\return the largest one of val1 and val2
		*/
		template <class T>
		inline T max(T val1, T val2)
		{
			if (val1 > val2) return val1;
			return val2;
		}

#endif
		
} // dwarf

// just to make sure the right functions are used
inline float sin(float x)				{return sinf(x);}
inline float cos(float x)				{return cosf(x);}
inline float asin(float x)				{return asinf(x);}
inline float acos(float x)				{return acosf(x);}
inline float tan(float x)				{return tanf(x);}
inline float pow(float x, float y)		{return powf(x, y);}
inline float log(float x)				{return logf(x);}
inline float sqrt(float x)				{return sqrtf(x);}
inline float atan(float x)				{return atanf(x);}
inline float atan2(float x, float y)	{return atan2f(x, y);}
//inline float fabs(float f)				{return (*(unsigned long *)&f) & 0x7fffffff;}


#endif // D_COREMATH_H_
