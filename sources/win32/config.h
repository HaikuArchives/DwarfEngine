#ifndef D_CONFIG_H_
#define D_CONFIG_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 *
 *	This is the configuration header for the win32
 *	version of Dwarf. It can be used as a template for
 *	other future ports.
 */


	// to disable microsoft's "feature" that makes counter variables
	// declared within for-statements remain in scope after the for-statement

#define for if (false) {} else for

	#define D_STATIC_RENDERER // if true, use static renderer instead of loading it dynamically
	#define D_UNICODE
	#define D_LITTLE_ENDIAN

	#ifdef D_UNICODE
		#define UNICODE
		#define _UNICODE
	#endif


	#include <windows.h>

	#include <stdarg.h>
	#include <string.h>
	#include <sys/timeb.h>
	#include <time.h>

	
//	#pragma warning(disable: 4244 4231 4018 4290 4660 4251 4305)

	#pragma warning(disable: 4244) // Type conversions, 'possible loss of data'
	#pragma warning(disable: 4305) // truncation from 'const double' to 'float'
	#pragma warning(disable: 4290) // C++ Exception Specification ignored
	#pragma warning(disable: 4100) // unreferenced formal parameter
	#pragma warning(disable: 4512) // assignment operator could not be generated

	#pragma warning(disable: 4127) // conditional expression is constant
//	#pragma warning(disable: 4231) // nonstandard extension used : 'extern'
//	#pragma warning(disable: 4661) // dont know if its ok to disable
//	#pragma warning(disable: 4275) // non dll-interface class ... used as base for dll-interface class

	// STL Warnings

	#pragma warning(disable: 4189)
	#pragma warning(disable: 4018)
	#pragma warning(disable: 4245)
	#pragma warning(disable: 4100)
	#pragma warning(disable: 4786)
	#pragma warning(disable: 4244)
	#pragma warning(disable: 4245)

	// Some Typedefs

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned __int64 uint64;

	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
	typedef signed __int64 int64;

	#ifdef D_UNICODE
		typedef unsigned short dchar;
	#else
		typedef char dchar;
	#endif

	#define D_CLINKAGE  __cdecl
	#define D_IMPORT __declspec(dllimport)
	#define D_EXPORT __declspec(dllexport)

//	#ifdef _USRDLL
		#define D_API //__declspec(dllexport)
		#define D_TEMPLATE(x) //template class __declspec(dllexport) x
//	#else
//		#define D_API __declspec(dllimport)
//		#define D_TEMPLATE(x) extern template class __declspec(dllimport) x
//	#endif
	
	#ifdef TEXT
		#undef TEXT
	#endif
	#ifdef CHAR
		#undef CHAR
	#endif
	#ifdef D_UNICODE
		#define TEXT(x) L##x
		#define CHAR(x) L##x
	#else
		#define TEXT(x) x
		#define CHAR(x) x
	#endif

	#ifndef WM_MOUSEWHEEL
		#define WM_MOUSEWHEEL 0x020A
	#endif


	struct WindowMessage { HWND hwnd; UINT msg; WPARAM wparam; LPARAM lparam; };

	#define D_PLATFORM TEXT("win32")
	#define D_VERSION_MAJOR 0
	#define D_VERSION_MINOR 1
	#define D_VERSION_STRING TEXT("0.1")
	#define D_VERSION D_VERSION_MAJOR##.##D_VERSION_MINOR
	#define D_WINDOW_CLASS	TEXT("_DwarfEngine3DWindow")
	#define D_WINDOW_NAME	TEXT("Dwarf 3D")
	
#endif // D_CONFIG_H_
