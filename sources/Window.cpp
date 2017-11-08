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

#include "Window.h"
#include "WindowDefs.h"
#include "View.h"
#include "Rect.h"
#include "Log.h"
#include "RenderingDevice.h"

using namespace dwarf;


/*!
	\brief Window constructor.
	\param frame the frame you want the window to have initially. x and y is the upper left corner.
	\param title sets the window's title. Can be changed with SetTitle()
	\param flags the window flags, see Window::WindowFlags
*/
Window::Window(const Rect &frame, const String &title, uint32 flags):
	m_timer(),
	m_device(0),
	m_window_base(0),
	m_viewlist(),
	m_flags(flags),
	m_input_server(0),
	m_running(false),
	m_min_frametime(0)
{
	m_device = create_default_device(this, frame, flags, 0);
	m_window_base = m_device->RenderWindow();
	m_window_base->SetTitle(title);
}



/*!
	\brief Window destroctor.
*/
Window::~Window()
{
	remove_default_device(m_device);
}


/*!
	\brief an implementation of the MessageReceived hookfunction defined in MessageReceiver::MessageReceived().
	This is a hook-function that is called when the window
	gets a message.
	\warning The baseclass version is NOT empty and
	should be called with any messages that your Window doesn't
	understand.
	\param message the Message the is sent to you
	\sa MessageReceiver::MessageReceived()
*/
void Window::MessageReceived(Message *message)
{
	switch (message->id)
	{
		case D_QUIT:
			Close();
			break;
		case D_HIDE:
			Show(false);
			break;
		case D_SHOW:
			Show(true);
			break;
		case D_REMOVE:
			View *view;
			(*message) >> (int32&)view;
			RemoveChild(view);
			break;
		default:
			MessageReceiver::MessageReceived(message);
	}
}


/*!
	\brief Resizes the window.
	\param width the new width of the window
	\param height the new ehight of the window
*/
void Window::Resize(float width, float height)
{
	D_ASSERT_MESSAGE(m_window_base != 0, TEXT("You used an uninitialized window."));
	m_window_base->Resize(width, height);
}


/*!
	\brief Called once per frame.
	If Window is inherited and Tick overloaded, the baseclass' Tick
	must be called in order for message handling to work.
	\param frame_time Time in seconds the previous frame took to draw
*/
void Window::Tick(float frame_time)
{
	MessageReceiver::Tick(frame_time);
}



/*!
	\brief A hookfunction that's called before any of the views are drawn.
	a hook-function that is called before the views are drawn
	a perfect place to clear the frame buffer, but if you are using a
	skydome, you don't need to clear the framebuffer. Note that
	this method is const, that means that you can't change any members of the
	window within this method.
*/
void Window::Draw(RenderingDevice &device) const
{
}

/*!
	\brief Adds a child view to the window.
	adds a view to the window, this view will then be Tick()'ed
	Draw()'n and receive window events (such as OnWindowAdd())
	Tick() will always be called before Draw() in the view. The views
	will be drawn in the same order that they are added to the window
	(i.e. the view added first will be drawn first, every frame)
	\param child the uView to add as a child to the window
	\warning note that you still owns the responsibility to delete the view, but
	you may not delete it while it's still added to a view (or a window), use RemoveChild()
	to remove it from the window
	\sa RemoveChild()
*/
void Window::AddChild(View *child)
{
	D_ASSERT_MESSAGE(child != 0, TEXT("You are trying to attach a child that is a null-pointer."));
	D_ASSERT_MESSAGE(child->Win() == 0, TEXT("You are trying to attach one view to more than one window."));

	m_viewlist.push_back(child);
	child->SetParent(this, Bounds());

	// lock the device just to be safe (the view's do have access to the deive through the Device() method)
	m_device->Lock();
	child->SetWindow(this);

	// and never forget to unlock the device
	m_device->Unlock();
}

/*!
	\todo document
*/
void Window::SetInputServer(InputServer *server)
{
	D_ASSERT_MESSAGE(m_window_base, TEXT("You are trying to use a window that has not been initialized correctly."));
	D_ASSERT_MESSAGE(m_input_server == 0 || server == 0, TEXT("You are trying to attach more than one input server on the same window."));

	m_input_server = server;
	m_window_base->SetInputServer(server);

	if (m_input_server != 0) m_input_server->AttachedToWindow(this);
}

/*!
	\brief a method that is called when the window is resized.
	This method is not empty, it tells all the child views that their
	parent has resized. You should call this version of OnResize() if
	you reimplement it in a derived class.
*/
void Window::OnResize(float width, float height)
{
	std::list<View*> &views = Children();
	for(std::list<View*>::iterator i = views.begin(); i != views.end(); i++)
		(*i)->OnSize(Rect(0.0, 0.0, width + 1.0, height + 1.0));
}


/*!
	\brief starts the window's main-loop, doesn't return until the window is closed.
	In the window's main loop it keep Tick()'ing and Draw()'ing it's child views. The window main-loop
	also limits the speed in wich it will call Tick() and Draw() (let's call it a cycle or a frame). If one cycle
	is really short, naturally, the frames per seconds will be high. Now, the window will limit the FPS (frames
	per second) if it's too high (to save CPU for other applications). You can set the max allowed FPS through
	the method LimitFPS().
	\sa AddChild() RemoveChild() LimitFPS()
*/
void Window::Run()
{
	D_ASSERT_MESSAGE(m_window_base, TEXT("You called Run() on an uninitialized window"));

	m_timer.Start(0);

	logger(TEXT("[Window::Run] starting"));

	float frame_time = 0.1;
	float time = m_timer.Current();
	m_running = true;
	while (m_running)
	{
		// tick the windowbase (does some platform dependent stuff)
		m_window_base->Tick();

		// tick the window
		Tick(frame_time);

		// Tick all the child views
		std::list<View*> &views = Children();
		for(std::list<View*>::iterator i = views.begin(); i != views.end(); ++i)
			(*i)->Tick(frame_time);

		// Draw the window content if the Window is visible
		if (IsVisible())
		{
			m_device->Lock();

			// call the window Draw() hook
			m_device->Viewport(Rect(0.0, 0.0, Width(), Height()));
			Draw(*m_device);

			// Invalidate (Draw) all the child views
			std::list<View*> &views = Children();
			for(std::list<View*>::iterator i = views.begin(); i != views.end(); ++i)
				(*i)->Invalidate(*m_device);

			m_device->SwapBuffers();
			m_device->Unlock();
		}

		// limit the FPS to free some CPU cycles
		float t = m_timer.Current();
		if (t - time < m_min_frametime)
			sleep(m_min_frametime - (t - time));

		// calculate the frame time in seconds
		t = m_timer.Current();
		frame_time = t - time;
		time = t;
	}

	logger(TEXT("[Window::Run] exiting"));

	m_timer.Pause();
}

/*!
	\brief closes the window.
	Tells the window's main-loop that it's time to end, after the current frame, the
	main-loop will exit, and the Run() call will return.
	\sa Run()
*/
void Window::Close()
{
	logger(TEXT("[Window::Quit] running = false"));
	// setting this variable to false makes the main_thread to quit
	// when finished with the current cycle, when that thread quits
	// it also quits the window and the application
	m_running = false;
}

/*!
	\brief limits the frames per second, used to free CPU.
	this sets the maximum framerate, if the framerate
	gets any higher the main thread will snooze and
	make sure the frame reate doesn't get any higher
	the reason is to save CPU and be a nice application
	the default max fps is 80
	\param max_fps the maximum frames per second
*/
void Window::LimitFPS(float fps)
{
	D_ASSERT_MESSAGE(fps > 0, TEXT("You are trying to limit the FPS to a value less than or equal to 0."));

	m_min_frametime = 1.0f / fps;
}
