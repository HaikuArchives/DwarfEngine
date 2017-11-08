#ifndef D_DEFS_H_
#define D_DEFS_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>

#ifdef __BEOS__
#	include "beos/config.h"
#elif WIN32
#	include "win32/config.h"
#endif

// the mmgr can be excluded from the project by defining NO_MMGR
#ifndef NO_MMGR
#	ifdef _DEBUG
#		include "mmgr.h"
#	else
#		include "nommgr.h"
#	endif
#endif

namespace dwarf
{
		class RenderingDevice;
		class Window;
		class Rect;
		class String;

		RenderingDevice *create_default_device(Window* window, const Rect &frame, uint32 flags, uint32 options);
		void remove_default_device(RenderingDevice *device);
		void sleep(float seconds);
		/*!
			\brief error handler, terminates and displays message
		*/
		void Error(const dchar* msg, ...);
		void MsgBox(const String &title, const String &body);

		/*!
			\brief enters the debugger
		*/
		void enter_debugger();
		void assert_message(const char *file, uint32 line, const dchar *condition, const dchar *message);

} // dwarf

#include "assert.h"

inline int16 swap_byteorder(int16 val)
{
	int16 ret;
	((int8*)&ret)[0] = ((int8*)&val)[1];
	((int8*)&ret)[1] = ((int8*)&val)[0];
	return ret;
}

inline uint16 swap_byteorder(uint16 val)
{
	uint16 ret;
	((int8*)&ret)[0] = ((int8*)&val)[1];
	((int8*)&ret)[1] = ((int8*)&val)[0];
	return ret;
}

inline int32 swap_byteorder(int32 val)
{
	int32 ret;
	((int8*)&ret)[0] = ((int8*)&val)[3];
	((int8*)&ret)[1] = ((int8*)&val)[2];
	((int8*)&ret)[2] = ((int8*)&val)[1];
	((int8*)&ret)[3] = ((int8*)&val)[0];
	return ret;
}

inline uint32 swap_byteorder(uint32 val)
{
	uint32 ret;
	((int8*)&ret)[0] = ((int8*)&val)[3];
	((int8*)&ret)[1] = ((int8*)&val)[2];
	((int8*)&ret)[2] = ((int8*)&val)[1];
	((int8*)&ret)[3] = ((int8*)&val)[0];
	return ret;
}

inline int64 swap_byteorder(int64 val)
{
	int64 ret;
	((int8*)&ret)[0] = ((int8*)&val)[7];
	((int8*)&ret)[1] = ((int8*)&val)[6];
	((int8*)&ret)[2] = ((int8*)&val)[5];
	((int8*)&ret)[3] = ((int8*)&val)[4];
	((int8*)&ret)[4] = ((int8*)&val)[3];
	((int8*)&ret)[5] = ((int8*)&val)[2];
	((int8*)&ret)[6] = ((int8*)&val)[1];
	((int8*)&ret)[7] = ((int8*)&val)[0];
	return ret;
}

inline uint64 swap_byteorder(uint64 val)
{
	uint64 ret;
	((int8*)&ret)[0] = ((int8*)&val)[7];
	((int8*)&ret)[1] = ((int8*)&val)[6];
	((int8*)&ret)[2] = ((int8*)&val)[5];
	((int8*)&ret)[3] = ((int8*)&val)[4];
	((int8*)&ret)[4] = ((int8*)&val)[3];
	((int8*)&ret)[5] = ((int8*)&val)[2];
	((int8*)&ret)[6] = ((int8*)&val)[1];
	((int8*)&ret)[7] = ((int8*)&val)[0];
	return ret;
}

inline float swap_byteorder(float val)
{
	float ret;
	((int8*)&ret)[0] = ((int8*)&val)[3];
	((int8*)&ret)[1] = ((int8*)&val)[2];
	((int8*)&ret)[2] = ((int8*)&val)[1];
	((int8*)&ret)[3] = ((int8*)&val)[0];
	return ret;
}

inline double swap_byteorder(double val)
{
	double ret;
	((int8*)&ret)[0] = ((int8*)&val)[7];
	((int8*)&ret)[1] = ((int8*)&val)[6];
	((int8*)&ret)[2] = ((int8*)&val)[5];
	((int8*)&ret)[3] = ((int8*)&val)[4];
	((int8*)&ret)[4] = ((int8*)&val)[3];
	((int8*)&ret)[5] = ((int8*)&val)[2];
	((int8*)&ret)[6] = ((int8*)&val)[1];
	((int8*)&ret)[7] = ((int8*)&val)[0];
	return ret;
}

#ifdef __GNUC__
inline wchar_t swap_byteorder(wchar_t val)
{
	return (wchar_t)swap_byteorder((uint16)val);
}
#endif

template <class T>
inline T FromLittleEndian(T val)
{
	#ifdef D_BIG_ENDIAN
		return swap_byteorder(val);
	#else
		return val;
	#endif
}

template <class T>
inline T FromBigEndian(T val)
{
	#ifdef D_LITTLE_ENDIAN
		return swap_byteorder(val);
	#else
		return val;
	#endif
}


template <class T>
inline T ToLittleEndian(T val)
{
	#ifdef D_BIG_ENDIAN
		return swap_byteorder(val);
	#else
		return val;
	#endif
}

template <class T>
inline T ToBigEndian(T val)
{
	#ifdef D_LITTLE_ENDIAN
		return swap_byteorder(val);
	#else
		return val;
	#endif
}


#endif // D_DEFS_H_
