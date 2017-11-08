#ifndef TYPES__JNG
#define TYPES__JNG

#ifdef __BEOS__
#include <SupportDefs.h>
#else

typedef	signed char			int8;
typedef unsigned char		uint8;

typedef	signed short		int16;
typedef unsigned short		uint16;

typedef	signed int			int32;
typedef unsigned int		uint32;

typedef long				status_t;

#endif

const	int JNG_OK = 0;
const	int JNG_ERROR = -1;
const	int JNG_ERROR_VERSION = -2;

#endif
