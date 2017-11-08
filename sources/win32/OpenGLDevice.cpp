
/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#define NO_MMGR
#include "Defs.h"
#include <gl/gl.h>
#include <gl/glu.h>

#include <list>

#ifdef _DEBUG
#include "mmgr.h"
#else
#include "nommgr.h"
#endif

#include "OpenGLDevice.h"
#include "Window.h"
#include "WindowBase.h"

#include "Bitmap.h"
#include "Plane.h"
#include "Vertex.h"
#include "Quaternion.h"
#include "Rect.h"
#include "Color.h"
#include "Array.h"
#include "PakFile.h"
#include "Mesh.h"
#include "Material.h"
#include "Matrix4x4.h"
#include "VertexArray.h"

#include "OpenGLWindow.h"

using namespace dwarf;

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#if !defined(D_STATIC_RENDERER) || defined(_USRDLL)

extern "C" __declspec(dllexport) RenderingDevice* LoadPlugin(RenderingDevice::Constructor* rdc)
{
	D_ASSERT(rdc);
	
	return new OpenGLDevice(rdc->window, rdc->frame, rdc->flags, rdc->options);
}

extern "C" __declspec(dllexport) void UnloadPlugin(RenderingDevice *rd)
{
	D_ASSERT(rd);
	// type cast is essential!
	delete ((OpenGLDevice*)rd);
}

int32 DwarfMain(Array<String> &commandline)
{
	return 0;
}

#endif // D_STATIC_RENDERER

#include "../OpenGLDevice/OpenGLDevice_common_code.h"


OpenGLDevice::OpenGLDevice(Window *window, const Rect &frame, uint32 flags, uint32 options):
	m_time(0.0f)
{
	D_ASSERT(window);
	m_fullscreen = false;
	// Create OpenGLWindowBase
	m_window = window;
	m_windowbase = new OpenGLWindow(this, m_window, frame, flags, options);
	m_depth = m_windowbase->Mode().d;
	// Create OpenGLDevice
	SetPixelFormat(m_windowbase->Mode().w, m_windowbase->Mode().h, m_windowbase->Mode().d);

	Bitmap default_bmp;
	default_bmp.SetSize(8,8, Bitmap::RGB);
	memset(default_bmp.Data(), 0xff, 8*8*default_bmp.BytesPerPixel());
	m_default_texture = HTexture(default_bmp, Texture::NO_MIPMAPS);

	// Start timer
	m_timer.Start();

	#ifdef _DEBUG
	m_locked = false;
	m_in_begin = false;
	#endif

	Lock();

	static const int32 state_convert[] =
	{
		GL_ALPHA_TEST,
			GL_AUTO_NORMAL,
			GL_BLEND,
			
			GL_COLOR_MATERIAL,
			GL_CULL_FACE,
			GL_DEPTH_TEST,
			GL_DITHER,
			
			GL_FOG,
			
			GL_LIGHTING,
			GL_LINE_SMOOTH,
			GL_LINE_STIPPLE,
			GL_LOGIC_OP,
			
			GL_POINT_SMOOTH,
			GL_POLYGON_SMOOTH,
			GL_POLYGON_STIPPLE,
			GL_SCISSOR_TEST,
			GL_STENCIL_TEST,
			GL_TEXTURE_1D,
			GL_TEXTURE_2D,
	};
	
	static int32 blend_convert[] =
	{
		GL_ZERO,
			GL_ONE,
			GL_SRC_COLOR,
			GL_ONE_MINUS_SRC_COLOR,
			GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,
			GL_DST_ALPHA,
			GL_ONE_MINUS_DST_ALPHA,
			GL_DST_COLOR,
			GL_ONE_MINUS_DST_COLOR,
			GL_SRC_ALPHA_SATURATE
	};
	
	
	for (int32 i = 0; i <= TEXTURE_2D; i++)
	{
		const uint32 bit = 1 << i;
		if (m_current_state.CurrentStates() & bit) glEnable(state_convert[i]);
		else glDisable(state_convert[i]);
	}
	
	SetBlendMode(m_current_state.SourceBlend(), m_current_state.DestinationBlend());
	SetColor(m_current_state.GetColor());
	SetDepthFunc(m_current_state.GetDepthTestFunction());
	SetPolygonMode(m_current_state.GetFillmode());

	Unlock();
}

OpenGLDevice::~OpenGLDevice()
{
	ShutdownDevice();
}

void OpenGLDevice::ShutdownDevice()
{
	m_window->Show(false);
	
	if(IsFullScreen())
		ChangeDisplaySettings(NULL, 0);

	logger(TEXT("Releasing opengl context..."));
	if(!wglMakeCurrent(m_hdc, NULL)) logger(TEXT("wglMakeCurrent(hdc, NULL) returned false."));
	if(!wglDeleteContext(m_hrc))     logger(TEXT("wglDeleteContext(hdc, NULL) returned false."));
	if(!ReleaseDC(m_windowbase->WindowHandle(), m_hdc))    logger(TEXT("Failed to release device context."));
	wglMakeCurrent( NULL, NULL );
	delete m_windowbase;
	m_windowbase = 0;
}

WindowBase* OpenGLDevice::RenderWindow()
{
	return m_windowbase;
}

void OpenGLDevice::Lock()
{
	#ifdef _DEBUG
	m_locked = true;
	#endif
}

void OpenGLDevice::Unlock()
{
	glFlush();
	#ifdef _DEBUG
	m_locked = false;
	#endif
}

void OpenGLDevice::SwapBuffers()
{
	static bool has_been_called = false;
	// Deal with SwapBuffers problem with NVidia drivers.
	int r = ::SwapBuffers(m_hdc);
	if(!r && !has_been_called)
	{
		// we report it but ignore it.
		logger(TEXT("WARNING! SwapBuffers returned false. Undefined behaviour may ensue."));
		has_been_called = true;
	}

	m_time = m_timer.Current();
}

HTexture OpenGLDevice::NullTexture() // returns the "none" texture
{
	return m_default_texture;
}

void OpenGLDevice::SetPixelFormat(int32 w, int32 h, int32 d)
{
	int 
		zbuffer_depth = 16,
		stencil_depth = 0;

	m_hdc = GetDC(m_windowbase->WindowHandle());
	if(!m_hdc) Error(TEXT("Failed to retrieve Device Context."));

	switch(d)
	{
	case 32:
		zbuffer_depth = 24;
		stencil_depth = 8;
		break;
	case 24:
		zbuffer_depth = 24;
		stencil_depth = 0;
		break;
	case 16:
		zbuffer_depth = 16;
		stencil_depth = 0;
		break;
	default:
		zbuffer_depth = 16;
		stencil_depth = 0;
		break;
	}
	
	// Setup opengl pixel format
	GLuint	PixelFormat;

	PIXELFORMATDESCRIPTOR pfd;
	 memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = d;
    pfd.cDepthBits   = zbuffer_depth; // for now at least
	pfd.cStencilBits = stencil_depth;

	PixelFormat = ChoosePixelFormat(m_hdc, &pfd);
	
	if(!PixelFormat)
		Error(TEXT("Can't find a suitable pixelformat."));

	if(!::SetPixelFormat(m_hdc, PixelFormat, &pfd))
		Error(TEXT("Can't set the PixelFormat."));

	m_hrc = wglCreateContext(m_hdc);
	if(!m_hrc)
		Error(TEXT("Can't create a GL Rendering Context."));

	if(!wglMakeCurrent(m_hdc, m_hrc))
		Error(TEXT("Can't activate GLRC."));

	String vendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	String renderer(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	String version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	String extensions(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));

	extensions.ReplaceAll(CHAR(' '), CHAR('\n'));

	logger(*(TEXT("GL_VENDOR: ") + vendor)); 
	logger(*(TEXT("GL_RENDERER: ") + renderer));
	logger(*(TEXT("GL_VERSION: ") + version));
	logger(*(TEXT("GL_EXTENSIONS:\n")
		TEXT("-----------------------------------\n") + extensions));
	logger(TEXT("-----------------------------------"));
}

bool OpenGLDevice::CanSetFullScreen()
{
	return false;
}

bool OpenGLDevice::IsFullScreen()
{
	return m_fullscreen;
}

void OpenGLDevice::SetFullScreen(bool fullscreen, int32 width, int32 height)
{
	//TODO: Fix the fullscreen code so that it is usable.
	// atm Window will be left with a bad windowbase-pointer
	// also all textures and displaylists will disappear
	D_ASSERT(!fullscreen);
	if(fullscreen)
	{
		if(m_windowbase)
		{
			ShutdownDevice();
		}
//		if(GoFullScreen(width, height, m_depth))
		{
			m_windowbase = new OpenGLWindow(this, m_window, Rect(0,0,width,height), Window::FULLSCREEN, 0);

			// Create OpenGLDevice
			SetPixelFormat(m_windowbase->Mode().w, m_windowbase->Mode().h, m_windowbase->Mode().d);
			m_fullscreen = true;

			m_window->SetWindowBase(m_windowbase);
			m_window->SetRenderingDevice(this);
		}
	}
	else
	{
		ChangeDisplaySettings(NULL, 0);
		m_fullscreen = false;
	}
}

bool OpenGLDevice::GoFullScreen(int32 width, int32 height, int32 _depth)
{
	const int MAX_NUM_MODES = 300; // to prevent a stall
	DEVMODE devMode;
	uint32 modeExist, modeSwitch, closeMode = 0, i, depth = _depth;
	uint32 w = (uint32)width, h = (uint32)height;

	const int MAX_MATCHES = 2;

	Array<DEVMODE> modelist(MAX_MATCHES); // max matching modes
	int num_found = 0;

	logger(TEXT("Trying switch to %dx%dx%d"), w, h, depth);

	if(depth%8 && !(depth==15)) // 15 bits is a valid mode on some old cards
	{
		logger(TEXT("GoFullScreen called with illegal bitdepth value"));
		return false;
	}

	for(i = 0; i < MAX_NUM_MODES; i++)
	{
		modeExist = EnumDisplaySettings(NULL, i, &devMode);

		// Check if we ran out of modes
		if (!modeExist)
			break;
		
		// Check for a matching mode
		if ( (devMode.dmBitsPerPel == depth) 
		   &&(devMode.dmPelsWidth  == w) 
		   &&(devMode.dmPelsHeight == h) )
		{
			if (!closeMode)
				closeMode = i;

			if(num_found < MAX_MATCHES)
			{
				modelist[num_found++] = devMode;
				logger(TEXT("Found matching mode %dx%dx%dx%d."), devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
			}
		}
	}

	// ok, find the best and the next best modes and try to switch to them
	int mode = 0;
//	memset(&modelist[mode], 0, sizeof(DEVMODE));
	bool found_mode = false;
	for(int oj = 0; oj < num_found; oj++)
		if(modelist[oj].dmDisplayFrequency >= modelist[mode].dmDisplayFrequency)
		{
			mode = oj;
			found_mode = true;
		}

	if(found_mode)
	{
		logger(TEXT("Attempting switch to %dx%dx%dx%d..."), modelist[mode].dmPelsWidth, modelist[mode].dmPelsHeight, modelist[mode].dmBitsPerPel, modelist[mode].dmDisplayFrequency);
		modeSwitch = ChangeDisplaySettings(&modelist[mode], CDS_FULLSCREEN); 
		if (modeSwitch==DISP_CHANGE_SUCCESSFUL)
			return true;
	}

	//Might be running in Windows95, let's try without the hertz change 
	EnumDisplaySettings(NULL, closeMode, &devMode);
	devMode.dmBitsPerPel = depth; 
	devMode.dmPelsWidth  = w; 
	devMode.dmPelsHeight = h; 
	devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; 
	
	logger(TEXT("Attempting no-hertz change to %dx%dx%d"), devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel);
	modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN); 
	if(modeSwitch == DISP_CHANGE_SUCCESSFUL)
		return true;

	// try with separate changes to bitdepth and resolution
	devMode.dmFields = DM_BITSPERPEL; 
	logger(TEXT("Attempting multistaged change to %dx%dx%d"), devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel);

	modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	if(modeSwitch == DISP_CHANGE_SUCCESSFUL)
	{
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT; 
		modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN); 

		if(modeSwitch == DISP_CHANGE_SUCCESSFUL)
			return true;
		ChangeDisplaySettings(NULL, 0);
	}

	// Well nothing works now does it
	if(modeSwitch == DISP_CHANGE_RESTART)
		logger(TEXT("Unable to set fullscreen mode. You may need to change your video mode to %dx%dx%d and then restart your computer."), w, h, depth);

	else
	if(modeSwitch == DISP_CHANGE_BADMODE)
		logger(TEXT("The video mode %dx%dx%d is not supported on this computer."), w, h, depth);

	else
	if(modeSwitch == DISP_CHANGE_FAILED)
		logger(TEXT("Hardware failed to change to %dx%dx%d."), w, h, depth);

	else
		logger(TEXT("Failed to change to %dx%dx%d for an unknown reason."), w, h, depth);

	return false;
}

