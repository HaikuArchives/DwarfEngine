/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <queue>
#include <list>

#include "../Defs.h"

#include "../Dwarf.h"

#include "../String.h"
#include "../Exception.h"
#include "../RenderingDevice.h"
#include "../Window.h"
#include "../Rect.h"
#include "../FileSystem.h"
#include "Directory.h"
#include "JPEGCodec.h"
#include "TGACodec.h"
#include "PNGCodec.h"
#include "../Codec3DS.h"
#include "../Texture.h"
#include "../LwobCodec.h"
#include "../MD2Codec.h"

#ifdef D_STATIC_RENDERER
#include "OpenGLDevice.h"
#endif

using namespace std;


#ifndef D_UNICODE
#define WCSRCHR strrchr
#define WCSCPY strcpy
#else
#define WCSRCHR wcsrchr
#define WCSCPY wcscpy
#endif

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"fmodvc.lib")


//#ifdef _DEBUG
//#pragma comment(lib, "zlibd.lib")
//#else
#pragma comment(lib, "zlib.lib")
//#endif


namespace dwarf
{
	void PrintWindowsInfo();
}

using namespace dwarf;


String FileSystem::m_Win32BasePath("");

#ifndef D_STATIC_RENDERER

static HINSTANCE g_hInstance;

#else

HINSTANCE g_hInstance;

#endif

void FileSystem::SetBasePath()
{
	dchar pathBuf[512];

	int32 retval = GetModuleFileName(g_hInstance, pathBuf, 512);

	if (!retval)
	{
		retval = GetModuleFileName(0, pathBuf, 512);
	}

	if (!retval)
	{
		logger(TEXT("GetModuleFileName() failed."));
		WCSCPY(pathBuf, String(__argv[0]));
	}


	dchar *bop = pathBuf;
	dchar *ptr;
	
	// Scan for "'s and remove them if found
	if(pathBuf[0] == TEXT('"'))
	{
		bop = &pathBuf[1];
		ptr = bop;
		while(*ptr != TEXT('"')) ptr++;
		ptr = TEXT('\0');
	}

	ptr = WCSRCHR(bop, TEXT('\\'));
	if (ptr)
	{
		ptr[1] = TEXT('\0');
	}
	else
	{
		ptr = WCSRCHR(bop, TEXT(':'));
		if (ptr)
		{
			ptr[1] = TEXT('\\');
			ptr[2] = TEXT('\0');
		}
		else
		{
			bop[0] = TEXT('\0');
		}
	}
	m_Win32BasePath = String(bop);
}

String FileSystem::BasePath()
{
	return m_Win32BasePath;
}

namespace dwarf
{

	void PrintWindowsInfo()
	{
		DWORD dwVersion = GetVersion();
 
		// Get the Windows version.

		DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
		DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

		// Get the build number for Windows NT/Windows 2000 or Win32s.
		
		DWORD dwBuild = 0;
		if (dwVersion < 0x80000000)                // Windows NT/2000
		{
			dwBuild = (DWORD)(HIWORD(dwVersion));
			logger(TEXT("* OS Version: Windows NT/2000 or higher, version %d.%d, build %d"), dwWindowsMajorVersion, dwWindowsMinorVersion, dwBuild);
		}
		else if (dwWindowsMajorVersion < 4)        // Win32s
		{
			dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
			logger(TEXT("* OS Version: Win32s, version %d.%d, build %d"), dwWindowsMajorVersion, dwWindowsMinorVersion, dwBuild);
		}
		else                  // Windows 95/98 -- No build number
		{
			logger(TEXT("* OS Version: Windows 95/98 or higher, version %d.%d"), dwWindowsMajorVersion, dwWindowsMinorVersion);
			dwBuild =  0;
		}

	}

	// MsgBox
	// Used to display an error message.
	void MsgBox(const String &title, const String &body)
	{
		HWND window = ::FindWindow(D_WINDOW_CLASS, 0);

		if (window)
		{
			::SetWindowPos(window, HWND_BOTTOM, 0,0,0,0, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
			::ChangeDisplaySettings(0, 0);
		}
		::MessageBox(window, *body, *title, MB_OK | MB_ICONEXCLAMATION );
	}

	void enter_debugger()
	{
		__asm { int 3 };
	}

	static HMODULE handle;

	RenderingDevice* create_default_device(Window* window, const Rect &frame, uint32 flags, uint32 options)
	{
#ifndef D_STATIC_RENDERER
		RenderingDevice::Constructor rdc;
		rdc.window = window;
		rdc.frame = frame;
		rdc.flags = flags;
		rdc.options = options;

		String devname = TEXT("dwarfopengl");

		handle = LoadLibrary(devname);
		typedef RenderingDevice* (*pfnLoad)(RenderingDevice::Constructor *);
		pfnLoad Load = (pfnLoad)GetProcAddress(handle, "LoadPlugin");
		String msg;
		msg.Format(TEXT("Failed to load 3d device, check that a dll named '%s.dll' exists in the path."), *devname);
		D_ASSERT_MESSAGE(Load, msg);
		return ((Load)(&rdc));
#else
		return new OpenGLDevice(window, frame, flags, options);
#endif
	}

	void remove_default_device(RenderingDevice* rd)
	{
#ifndef D_STATIC_RENDERER
		typedef void (*pfnUnload)(RenderingDevice*);
		pfnUnload Unload = (pfnUnload)GetProcAddress(handle, "UnloadPlugin");
		D_ASSERT_MESSAGE(Unload, TEXT("Failed to unload 3d device."));

		(Unload)(rd);
		FreeLibrary(handle);
#else
		OpenGLDevice* ogld = reinterpret_cast<OpenGLDevice*>(rd);
		D_ASSERT(ogld);
		delete ogld;
#endif
	}

	void sleep(float seconds)
	{
		Sleep(seconds * 1000.0f);
	}

	void assert_message(const char *file, uint32 line, const dchar *condition, const dchar *message)
	{
		dwarf::String msg;
#ifdef D_UNICODE
		msg.Format(TEXT("Assert Failed!\n\n\nIn file: %S:%d\nThe statement that failed was '%s': %s\n"), file, line, condition, message);
#else
		msg.Format(TEXT("Assert Failed!\n\n\nIn file: %s:%d\nThe statement that failed was '%s': %s\n"), file, line, condition, message);
#endif
		logger(msg);
		MsgBox(TEXT("DwarfEngine 3D Error message"), msg);
		enter_debugger();
	}

}

void DwarfLogTime()
{

   struct _timeb timebuffer;
   char *timeline;

   _ftime( &timebuffer );
   timeline = ctime( & ( timebuffer.time ) );

#ifdef D_UNICODE
   logger(Log::LOW, TEXT("* Timestamp: %.19S"), timeline);
#else
   logger(Log::LOW, TEXT("* Timestamp: %.19s"), timeline);
#endif

}

int32 DwarfMain(dwarf::Array<String> &commandline);

int DwarfInternalMain()
{
	FileSystem::SetBasePath();

#ifdef _DEBUG
	logger.SetLogLevel(Log::HIGH);
#else
	logger.SetLogLevel(Log::LOW);
#endif

	Directory defaultDir;

	Texture::SetFileSystem(&defaultDir);

	File log_file(&defaultDir, TEXT("dwarf.log"), File::WRITE, File::CREATE | File::TEXT);
	logger.PushOutput(&log_file);

	Array<String> command(__argc);
	for (int32 i = 0; i < __argc; i++)
		command[i] = __argv[i];

	PNGCodec pngcodec;
	JPEGCodec jpgcodec;
	TGACodec tgacodec;
	Codec3DS threedscodec;
	LwobCodec lwob_codec;
	MD2Codec md2_codec;
		
	logger(Log::LOW, TEXT("* Dwarf Engine 3D"));
	DwarfLogTime();
	logger(Log::LOW, TEXT("* Version ") D_PLATFORM TEXT("-") D_VERSION_STRING);
#ifdef _DEBUG
	logger(Log::LOW, TEXT("* Debug build"));
#else
	logger(Log::LOW, TEXT("* Release build"));
#endif
	logger(Log::LOW, TEXT("* Base path: %s"), *FileSystem::m_Win32BasePath);
	PrintWindowsInfo();

	int r = DwarfMain(command);
	
	if (r)	logger(Log::LOW, TEXT("* Engine shutdown with errorcode 0x02x."), r);
	else	logger(Log::LOW, TEXT("* Engine shutdown normal."), r);

	logger.PopOutput();

	return r;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	g_hInstance = hInstance;
	try
	{
		int ret = DwarfInternalMain();

#ifdef _DEBUG
		char buf[512];
		FileSystem::BasePath().ToChar(buf, 512);
		strcat(buf, "\\memreport.log");
		m_dumpMemoryReport(buf);
#endif
		return ret;
	}
	catch (const Exception &e)
	{
		e.OpenDialog();
		return 1;
	}
}
