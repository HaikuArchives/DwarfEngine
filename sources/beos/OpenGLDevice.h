#ifndef D_OPENGLDEVICE_H_
#define D_OPENGLDEVICE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <GLView.h>
#include <stdlib.h>
#include <new>

#include "RenderingDevice.h"
#include "Vertex.h"
#include "Rect.h"
#include "Window.h"
#include "State.h"
#include "OpenGLWindow.h"

#ifdef _DEBUG
#include "nommgr.h"
#endif

namespace dwarf
{

	/*!
		\todo in beos, the window that is opened do not like Paul Nettle's memory manager, and crashes when closed.
	*/
	class OpenGLDevice : public RenderingDevice
	{
	public:
	
		OpenGLDevice(Window *window, const Rect &frame, uint32 flags, uint32 options, BView *view);
		virtual ~OpenGLDevice();
		
		// include the common members
		#include "../OpenGLDevice/OpenGLDevice_common.h"

		OpenGLWindow m_window;
		HTexture m_null_texture;
	};
	
	#ifdef _DEBUG
	#include "mmgr.h"
	#endif

} // dwarf

#endif // D_OPENGLDEVICE_H_
