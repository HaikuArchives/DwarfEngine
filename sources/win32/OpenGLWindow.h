#ifndef D_OPENGLWINDOW_H_
#define D_OPENGLWINDOW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "List.h"
#include "InputReceiver.h"
#include "Vertex.h"

namespace dwarf
{


		class Rect;
		class Face;
		class InputReceiver;
		class View;
		class OpenGLDevice;

		class OpenGLWindow : public WindowBase
		{
			public:
			

			OpenGLWindow(OpenGLDevice* device, Window* window, const Rect &frame, uint32 flags, uint32 options);
			virtual ~OpenGLWindow();


			virtual void SetRelativeMouseMode(bool mode);
//			virtual bool RelativeMouseMode();

			virtual void SetTitle(const String &title);
			virtual const String &Title();

			virtual void SetMousePosition(const Point &position);

			virtual void Tick();

			virtual void MoveTo(const Point &position);
			virtual Point Position();
			virtual void Resize(float width, float height);

			virtual int32 Width();
			virtual int32 Height();
	
			virtual void Show(bool visible = true);
			virtual bool IsVisible();
			virtual void Minimize(bool minimize);

			virtual void ShowCursor(bool show);
			virtual bool IsCursorVisible();

			virtual void SetInputServer(InputServer *server);

			class GfxMode
			{
			public:
				int32 w, h, d; // width, height, bitdepth of window
			};
			
			HWND WindowHandle() { return m_hwnd; }
			GfxMode Mode() { return m_mode; }
			void SetMode(int32 w, int32 h, int32 d) { m_mode.w = w; m_mode.h = h; m_mode.d = d; }
			Rect GetCanvasSize(const Rect& in_r) const;

			protected:	
					
				OpenGLDevice *m_device;
				Window *m_window;
				HWND m_hwnd;
				bool m_cursor_visible;
				bool m_running;
				bool m_is_active;
				float m_delay;
				GfxMode m_mode;
				String m_title;
				
				
				void OpenWindow(const String &title, uint32 flags, const Rect &frame);
				void CloseWindow();
				
				int SystemMessage(WindowMessage wm);

				static LRESULT WINAPI sWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		};

} // dwarf

#endif // D_OPENGLWINDOW_H_
