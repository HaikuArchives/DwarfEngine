#ifndef D_COLOR_H_
#define D_COLOR_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Vertex.h"

namespace dwarf
{
		class Color;
	
		/*!
			contains red, green, blue and alpha components, stored as 4 floats where
			1.0 is max and 0.0 is min.
			\brief a color class
		*/
		class Color
		{
			public:
	
			Color(float red = 1.0, float green = 1.0, float blue = 1.0, float alpha = 1.0);
			Color(const Color &c);
			Color(const Vertex<3> &v);
	
			Color& operator=(const Color &c);
			Color operator*(float val) const;
			Color& operator=(const Vertex<3> &v);
			operator float*() const;

			void FromBytes(uint8 red, uint8 green, uint8 blue, uint8 alpha = 255);

			float r; //!< the red color component
			float g; //!< the green color component
			float b; //!< the blue color component
			float a; //!< the alpha color component
		};
	
		// inline definitions
		
		/*!
			\brief constructor
			\param red the red component (default is 1.0)
			\param green the green component (default is 1.0)
			\param blue the blue component (default is 1.0)
			\param alpha the alpha component (default is 1.0)
		*/
		inline Color::Color(float red, float green, float blue, float alpha) :
		r(red), g(green), b(blue), a(alpha)
		{
		}
		
		/*!
			\brief copy constructor
			\param c the color to be copied
		*/
		inline Color::Color(const Color &c) :
		r(c.r), g(c.g), b(c.b), a(c.a)
		{
		}

		inline Color::Color(const Vertex<3> &v) :
		r(v.x()), g(v.y()), b(v.z()), a(1.0)
		{
		}
		
		inline Color Color::operator*(float val) const
		{
			return Color(r * val, g * val, b * val, a * val);
		}
		
		/*!
			\brief copy operator
			\param c the color to be copied
			\return a reference to this color object
		*/
		inline Color& Color::operator=(const Color &c)
		{
			r = c.r; g = c.g; b = c.b; a = c.a;
			return *this;
		}
		
		/*!
			copies the content of the Vertex<3>. (red = x, green = y, blue = z)
			\brief assignment operator
			\param v the Vertex<3> to read values from
			\return a reference to this color object
		*/
		inline Color& Color::operator=(const Vertex<3> &v)
		{
			r = v.x(); g = v.y(); b = v.z();
			return *this;
		}
		
		/*!
			returns a float pointer that points to an array with the four components
			in the following order (red, green, blue, alpha)
			\brief converts the color to a float pointer
			\return a float pointer
		*/
		inline Color::operator float*() const
		{
			return reinterpret_cast<float*>(const_cast<Color *>(this));
		}

		inline void Color::FromBytes(uint8 red, uint8 green, uint8 blue, uint8 alpha)
		{
			r = static_cast<float>(red)/255.0f;
			g = static_cast<float>(green)/255.0f;
			b = static_cast<float>(blue )/255.0f;
			a = static_cast<float>(alpha)/255.0f;
		}


} // dwarf

#endif // D_COLOR_H_
