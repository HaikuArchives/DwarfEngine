#ifndef D_BEOS_H_
#define D_BEOS_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <stdio.h>
#include <wchar_t.h>
#include <ctype.h>

#include <DataIO.h>
#include <File.h>
#include <image.h>
#include <MediaFile.h>
#include <MediaTrack.h>
#include <MediaDefs.h>
#include <memory.h>
#include <OS.h>
#include <SupportDefs.h>
#include <SoundPlayer.h>
#include <Application.h>
#include <File.h>
#include <Entry.h>
#include <Path.h>
#include <Directory.h>
#include <Roster.h>
#include <UTF8.h>


#if BYTE_ORDER == __LITTLE_ENDIAN
#	define D_LITTLE_ENDIAN
#else
#	define D_BIG_ENDIAN
#endif

#define _DEBUG // comment this line away when building release mode
#define D_UNICODE

	#ifdef D_UNICODE
		typedef uint16 dchar;
	#else
		typedef char dchar;
	#endif

	#define D_CLINKAGE  __cdecl

//	operator = (wchar_t x)
//	{ return static_cast<uint16>(x); }

	#ifdef TEXT
		#undef TEXT
	#endif
	#ifdef CHAR
		#undef CHAR
	#endif
	#ifdef D_UNICODE
		#define TEXT(string) ((const dchar*)L##string)
		#define CHAR(char) ((const dchar)L##char)
	#else
		#define TEXT(string) string
		#define CHAR(char) char
	#endif

	#define D_PLATFORM TEXT("BeOS")
	#define D_VERSION_MAJOR 0
	#define D_VERSION_MINOR 1
	#define D_VERSION_STRING TEXT("##D_VERSION_MAJOR##.##D_VERSION_MINOR##")
	#define D_VERSION D_VERSION_MAJOR##.##D_VERSION_MINOR

#endif // D_BEOS_H_
