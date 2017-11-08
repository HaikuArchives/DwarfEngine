#ifndef _SOFTWARE_WINDOW_H_
#define _SOFTWARE_WINDOW_H_

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

	class SoftwareWindow : public WindowBase
	{
	public:
		SoftwareWindow(Window *parent, RenderingDevice *device, const Rect &frame, uint32 flags, uint32 options, BView *view);
		virtual ~SoftwareWindow();

		virtual void SetInputServer(InputServer *server);
		
		void Lock();
		void Unlock();
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

		uint32 *BackBuffer() { m_window->BackBuffer(); }

	private:

		// internal window class
		class BeWindow : public BWindow
		{
		public:
			BeWindow(Window *parent, const Rect &frame, uint32 flags, uint32 options);
			virtual ~BeWindow();

			void *operator new(size_t size);
			void operator delete(void *ptr);

			void Lock();
			void Unlock();
			void SwapBuffers();

			void *BackBuffer() { return m_view.BackBuffer(); }

			virtual bool QuitRequested();
			virtual void Quit();

		private:

			class BitmapView : public BView
			{
			public:

				BitmapView(BRect frame):
					BView(frame, "bitmap view", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE),
					m_front_buffer(0)
				{
					SetViewColor(B_TRANSPARENT_COLOR);
					m_buffer[0] = new BBitmap(BRect(0,0, frame.Width(), frame.Height()), B_RGB32);
					m_buffer[1] = new BBitmap(BRect(0,0, frame.Width(), frame.Height()), B_RGB32);
				}
				
				~BitmapView()
				{
					Sync();
					delete m_buffer[0];
					delete m_buffer[1];
				}

				void SwapBuffers()
				{
					m_front_buffer++;
					m_front_buffer &= 1;
					Invalidate();
				}

				void Lock() { m_locker.Lock(); }
				void Unlock() { m_locker.Unlock(); }

				virtual void Draw(BRect)
				{
					Lock();
					DrawBitmapAsync(m_buffer[m_front_buffer], BPoint(0,0));
//					DrawBitmap(m_buffer[m_front_buffer], BPoint(0,0));
					Unlock();
				}

				void *BackBuffer() { return m_buffer[(m_front_buffer+1)&1]->Bits(); }

				virtual void FrameResized(float width, float height)
				{
					Lock();
					Sync();
					delete m_buffer[0];
					delete m_buffer[1];
					
					m_buffer[0] = new BBitmap(BRect(0,0, width, height), B_RGB32);
					m_buffer[1] = new BBitmap(BRect(0,0, width, height), B_RGB32);

					Unlock();
				}

			private:

				BLocker m_locker;
				BBitmap *m_buffer[2];
				int32 m_front_buffer; // 0 or 1

			};

			BitmapView m_view; // the View that lives inside the window
			Window *m_parent; // the window that should receive all input events
		};

		BeWindow *m_window;
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

	inline void SoftwareWindow::Lock()
	{
		m_window->Lock();
	}
	
	inline void SoftwareWindow::Unlock()
	{
		m_window->Unlock();
	}
	
	inline void SoftwareWindow::SwapBuffers()
	{
		m_window->SwapBuffers();
	}
	
	inline void SoftwareWindow::SetFullScreen(bool set, int32 width, int32 height)
	{
	}
	
	inline bool SoftwareWindow::IsFullScreen()
	{
		return false;
	}
	
	inline void SoftwareWindow::BeWindow::Lock()
	{
		m_view.Lock();
		BWindow::Lock();
	}
	
	inline void SoftwareWindow::BeWindow::Unlock()
	{
		m_view.Unlock();
		BWindow::Unlock();
	}
	
	inline void SoftwareWindow::BeWindow::SwapBuffers()
	{
		m_view.SwapBuffers();
	}

} // dwarf

#endif // _SOFTWARE_WINDOW_H_
