#ifndef D_WINDOW_H_
#define D_WINDOW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <list>
#include "mmgr.h"

#include "Defs.h"


#include "WindowBase.h"
#include "View.h"
#include "MessageReceiver.h"
#include "WindowDefs.h"
#include "InputReceiver.h"
#include "InputServer.h"
#include "View.h"
#include "RenderingDevice.h"
#include "Timer.h"
#include "Exception.h"

namespace dwarf
{

	class Rect;
	class Face;
	class String;
	class Window;

	/*!
		It automatically attaches a rendering device to the window
		\brief the window class represents a window.
		\todo add options to the rendering device (alpha channel, depth buffer)
		\todo an option to select rendering device
	*/

	class Window : public MessageReceiver, public InputReceiver
	{
	public:

		/*!
			This enum defines the flags that can be sent to the Window constructor
			\brief Window flags
		*/
		enum WindowFlags
		{
			FULLSCREEN		= 0x001,	//!< Initializes the window to fullscreen
			CENTERED		= 0x002,	//!< Places the window in the center of the screen.
			UNRESIZABLE		= 0x004,	//!< Makes the window unresizable.
			UNCLOSABLE		= 0x008,	//!< Removes the close button on the window.
			UNMOVABLE		= 0x010,	//!< Makes the window unmovable.
			BORDERLESS		= 0x020		//!< Makes the window borderless. Not needed in fullscreen. If you set this flag the window will still be closable, movable and resizable, but only by using keyboard shortcuts.
		};


		Window(const Rect &frame, const String &title = TEXT("DwarfEngine"), uint32 flags = 0);
		virtual ~Window();

		
		void Run();


		void Show(bool visible = true);
		bool IsVisible() const;
		void Minimize(bool minimize);
		void Close();

		void SetInputServer(InputServer *server);
		InputServer* GetInputServer();
		bool HasInputServer() const;

		void AddChild(View *child);
		void RemoveChild(View *view);
		std::list<View*> &Children();

		void SetTitle(const String &title);
		const String &Title() const;

		void LimitFPS(float max_fps);
		void SetMousePosition(const Point &position);

		void MoveTo(const Point &position);
		Point Position() const;
		void Resize(float width, float height);
		Rect Bounds() const;
		float Width() const;
		float Height() const;

		void ShowCursor(bool show);
		bool IsCursorVisible() const;

		uint32 Flags() const;


		RenderingDevice *Device();


		virtual void Tick(float frame_time);
		virtual void Draw(RenderingDevice &device) const;
		virtual void OnResize(float width, float height);

		Timer& SystemTimer();


		void SetWindowBase(WindowBase* wb) { D_ASSERT_MESSAGE(wb != 0, TEXT("You tried to set the window base to null")); logger(TEXT("Switching WindowBase.")); m_window_base = wb; }
		void SetRenderingDevice(RenderingDevice* rd) { D_ASSERT_MESSAGE(rd != 0, TEXT("You tried to set the renderingdevice to null")); logger(TEXT("Switching RenderingDevice.")); m_device = rd; }

		void SetRelativeMouseMode(bool mode) { m_window_base->SetRelativeMouseMode(mode); }

	protected:

		virtual void MessageReceived(Message *message);
			
	private:

		
		Timer m_timer;
		RenderingDevice *m_device;	// the rendering device
		WindowBase *m_window_base;	// the windowbase, that the device has created
		std::list<View*> m_viewlist;		// the list of child views
		uint32 m_flags;
		InputServer* m_input_server;// the input server
		bool m_running;				// true while the window is running
		float m_min_frametime;		// the minimum frame time allowed

	
	};


	// inline definitions

	/*!
		\brief returns the window flags
		\return the window flags
	*/
	inline uint32 Window::Flags() const
	{
		return m_flags;
	}

	/*!
		Gives you access to the timer, you can Pause(), and retrieve the current time.
		\brief returns the system timer
		\return the system timer
		\sa Timer
	*/
	inline Timer& Window::SystemTimer()
	{
		return m_timer;
	}

	inline void Window::SetMousePosition(const Point &position)
	{
		D_ASSERT_MESSAGE(m_window_base, TEXT("You used an uninitialized window"));
		m_window_base->SetMousePosition(position);
	}

	/*!
		\brief removes a previously added child-view from this view
		\param view a pointer to the view to remove
		\sa AddChild()
	*/
	inline void Window::RemoveChild(View *child)
	{
		m_viewlist.remove(child);
	}
	
	/*!
		\brief returns an iterarator to the first child
		\return an Iterator pointing at the first child view.
		\warning you do NOT own these views, and you should NEVER
		delete any of them.
	*/
	inline std::list<View*> &Window::Children()
	{
		return m_viewlist;
	}

	inline void Window::ShowCursor(bool visible)
	{
		D_ASSERT_MESSAGE(m_window_base, TEXT("You used an uninitialized window"));
		m_window_base->ShowCursor(visible);
	}

	inline void Window::Minimize(bool minimize)
	{
		D_ASSERT_MESSAGE(m_window_base, TEXT("You used an uninitialized window"));
		m_window_base->Minimize(minimize);
	}

	/*!
		When the window is hidden it won't be accessible by the user (i.e. it's not the same as minimized). To make
		the window visible (and accessible) to the user again, just call Show(true)
		\brief shows or hides the window.
		\param visible when true the window will be visible, and if false it will be hidden (the user won't see the difference
		of a closed window and a hidden window).
		\sa Minimize() Close()
	*/
	inline void Window::Show(bool visible)
	{
		D_ASSERT(m_window_base);
		m_window_base->Show(visible);
	}

	/*!
		\brief asks the window if it's visible
		\return true if the window is visible and false if it's not.
		\sa Show()
	*/
	inline bool Window::IsVisible() const
	{
		D_ASSERT(m_window_base);
		return m_window_base->IsVisible();
	}

	inline RenderingDevice *Window::Device()
	{
		D_ASSERT(m_device);
		return m_device;
	}

	inline float Window::Width() const
	{
		D_ASSERT(m_window_base);
		return static_cast<float>(m_window_base->Width());
	}
	
	/*!
		That means that x and y will allways be (0,0).
		\brief returns the bounds rectangle of the window, in it's own coordinate system
	*/
	inline Rect Window::Bounds() const
	{
		D_ASSERT(m_window_base);
		return Rect(0,0,static_cast<float>(m_window_base->Width()), static_cast<float>(m_window_base->Height()));
	}
	
	inline float Window::Height() const
	{
		D_ASSERT(m_window_base);
		return static_cast<float>(m_window_base->Height());
	}
	
	inline Point Window::Position() const
	{
		D_ASSERT(m_window_base);
		return m_window_base->Position();
	}

	/*!
		\brief changes the title of the window
		\param title the new title you want for the window
		\sa Title()
	*/
	inline void Window::SetTitle(const String &title)
	{
		D_ASSERT(m_window_base);
		m_window_base->SetTitle(title);
	}

	/*!
		\brief tells you the current title of the window
		\returns the current window title
		\sa SetTitle()
	*/
	inline const String& Window::Title() const
	{
		D_ASSERT(m_window_base);
		return m_window_base->Title();
	}

	/*!
		\todo document
	*/
	inline InputServer* Window::GetInputServer()
	{
		D_ASSERT_MESSAGE(m_input_server != 0, TEXT("The window has no input server attached to it."));
		return m_input_server;
	}

	inline bool Window::HasInputServer() const
	{
		return m_input_server != 0;
	}

} // dwarf




/*!
	\page example_window a simple window example

	This example shows some primitive usages of the Window class and how
	it works together with the views.
	First we need to include the "dwarf.h" include file and declare that we
	want to use the dwarf namespace.
	\code
	#include "dwarf.h"

	using namespace dwarf;
	\endcode

	The following line creates a window of the size 320x240 and positioned with it's top left corner at (20, 20).
	It will have "WindowSample" as it's title and it will not be resizable.
	\todo finish this documentation!
  
	\until Window
	
	Here we create two views, the same size of the window (we query the window's size through Bounds()). The ClearView
	also want's a color (the color it should clear with).
	\until noise
	
	The we add the views to the window, and remember that the order in wich we add the views does matter.
	We add the clear view first, because we want the window cleared before the next view draws anything.
	(you could try switch order and see that there will be no noise visible)
	\until noise
	
	Then we just have to make the widnow visible (with Show()) and start it. The Window's Run() method won't return until
	the window is closed. And during that time, it will call Tick() and Draw() on all it's children, making them draw stuff.
	
	\until }
*/

#endif // D_WINDOWBASE_H_
