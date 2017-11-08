#ifndef D_OPENGL_DEVICE_H_
#define D_OPENGL_DEVICE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 *
 *	Win32 OpenGL device.
 *
 */



#include "../RenderingDevice.h"
#include "../Timer.h"
#include "../State.h"

namespace dwarf
{

		class OpenGLDevice;

		class OpenGLDevice : public RenderingDevice
		{

		public:
			OpenGLDevice(Window *window, const Rect &frame, uint32 flags, uint32 options);
			virtual ~OpenGLDevice();

			bool GoFullScreen(int32 width, int32 height, int32 depth);
			
			void SetPixelFormat(int32 w, int32 h, int32 d);
			void ShutdownDevice();
			
			int32 m_depth; // bitdepth of device
			HDC m_hdc; // device context
			HGLRC m_hrc; // opengl rendering context
			bool m_fullscreen;
			HTexture m_default_texture;
			class OpenGLWindow *m_windowbase; // the window itself
			class Window *m_window; // the public interface to the window


			#include "../OpenGLDevice/OpenGLDevice_common.h"

			
		};
	
} // dwarf

#endif // D_OPENGL_DEVICE_H_
