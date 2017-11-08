#ifndef _OPENGL_WINDOW_H_
#define _OPENGL_WINDOW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#ifdef _DEBUG
#include "nommgr.h"
#endif

namespace dwarf
{
	class RenderingDevice;
	class Window;

	class OpenGLWindow : public WindowBase
	{
	public:
		OpenGLWindow(Window *parent, RenderingDevice *device, const Rect &frame, uint32 flags, uint32 options, BView *view);
		virtual ~OpenGLWindow();

		virtual void SetInputServer(InputServer *server);
		
		void LockGL();
		void UnlockGL();
		void SwapBuffers();
		
		void SetFullScreen(bool set, int32 width, int32 height);
		bool IsFullScreen();

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

		virtual void ShowCursor(bool show);
		virtual bool IsCursorVisible();
		virtual void Minimize(bool minimize);

		virtual uint32 Flags();
		virtual void SetRelativeMouseMode(bool mode);

		private:

		// internal window class
		class BeGLWindow : public BDirectWindow
		{
		public:
			BeGLWindow(Window *parent, const Rect &frame, uint32 flags, uint32 options);
			virtual ~BeGLWindow();

			void *operator new(size_t size);
			void operator delete(void *ptr);

			void LockGL();
			void UnlockGL();
			void SwapBuffers();

			virtual void DirectConnected(direct_buffer_info *info);
			virtual bool QuitRequested();
			virtual void Quit();

		private:
	
			BGLView m_glview; // the GLView that lives inside the window
			Window *m_parent; // the window that should receive all input events
		};

		BeGLWindow *m_window;
		RenderingDevice *m_device; // the rendering device
		Window *m_parent; // the window that should receive all input events
		String m_title;					// the window's title

		BMessageFilter *m_input;	// the messagr filter that retreives all input messages
									// not owned by this class!
		BView *m_input_view;		// the view that retrieves all mouse input events
									// not owned by this class!

		volatile bool m_running;		// is true as long as the window is opened
		bigtime_t m_min_frametime;		// the minimal allowed frame time. used to free CPU
		bool m_cursor_visible;			// tells you waether the cursor is visible or not
		uint32 m_flags;					// contains the window flags

	};

	inline void OpenGLWindow::LockGL()
	{
		m_window->LockGL();
	}
	
	inline void OpenGLWindow::UnlockGL()
	{
		m_window->UnlockGL();
	}
	
	inline void OpenGLWindow::SwapBuffers()
	{
		m_window->SwapBuffers();
	}
	
	inline void OpenGLWindow::SetFullScreen(bool set, int32 width, int32 height)
	{
		m_window->SetFullScreen(set);
	}
	
	inline bool OpenGLWindow::IsFullScreen()
	{
		return m_window->IsFullScreen();
	}
	
	inline void OpenGLWindow::BeGLWindow::LockGL()
	{
		m_glview.LockGL();
	}
	
	inline void OpenGLWindow::BeGLWindow::UnlockGL()
	{
		m_glview.UnlockGL();
	}
	
	inline void OpenGLWindow::BeGLWindow::SwapBuffers()
	{
		m_glview.SwapBuffers();
	}

} // dwarf

#endif // _OPENGL_WINDOW_H_
