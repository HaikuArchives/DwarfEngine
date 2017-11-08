
/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <UTF8.h>
#include <String.h>
#include <Screen.h>
#include <View.h>
#include <GLView.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Rect.h>

#include "Plane.h"
#include "Vertex.h"
#include "Quaternion.h"
#include "Vertex.h"
#include "Rect.h"
#include "Color.h"
#include "Window.h"
#include "Array.h"
#include "InputServer.h"
#include "InputServerBase.h"
#include "Matrix4x4.h"
#include "OpenGLWindow.h"

using namespace dwarf;

OpenGLWindow::OpenGLWindow(Window *parent, RenderingDevice *device, const Rect &frame, uint32 flags, uint32 options, BView *view):
	m_window(new BeGLWindow(parent, frame, flags, options)),
	m_device(device),
	m_parent(parent),
	m_title(TEXT("DwarfEngine window")),
	m_running(true),
	m_min_frametime(1000000 / 80),	// initialize the minimal fps to 80
	m_cursor_visible(true),
	m_flags(flags)
{
	m_window->AddChild(view);
}

/*!
	\brief Window destroctor
*/
OpenGLWindow::~OpenGLWindow()
{
	m_window->Quit();

	logger(TEXT("[OpenGLWindow::~OpenGLWindow] closing"));
}

void OpenGLWindow::SetInputServer(InputServer *server)
{
	if (server == 0)
	{
		m_window->Lock();
		m_window->RemoveChild(m_input_view);
		m_window->Unlock();
		m_input = 0;
		m_input_view = 0;
	}
	else
	{
		m_input_view = server->GetBase().InputView();
		m_window->AddChild(m_input_view);
	}

	logger(TEXT("Setting input server\n"));
}

/*!
	\brief Shows or hides the window
	\param visible sets wether the window should be visible or not
*/
void OpenGLWindow::Show(bool visible)
{
	if (visible) m_window->Show();
	else m_window->Hide();
}

/*!
	This method tells only if the window is shown (with Show()), the window may be hidden by some other
	reason (it's on another workspace or another window lies in front of it) but still visible (in this sence).
	\brief Tells if the window is currently hidden or visible
	\return true if the window currently is visible and false if the window is hidden
*/
bool OpenGLWindow::IsVisible()
{
	m_window->Lock();
	bool visible = !m_window->IsHidden();
	m_window->Unlock();
	return visible;
}

void OpenGLWindow::SetRelativeMouseMode(bool mode)
{
	SetMousePosition(Point(Width()/2.0f, Height()/2.0f));
	if (mode) be_app->HideCursor();
	else be_app->ShowCursor();
}

void OpenGLWindow::Minimize(bool minimize)
{
	m_window->Minimize(minimize);
}

/*!
	\brief returns the window flags
	\return the window's flags
*/
uint32 OpenGLWindow::Flags()
{
	return m_flags;
}

/*!
	moves the mouse cursor to the given window-space position
	\brief Moves the mouse to the given position
	\param position the position the mouse will be moved to
*/
void OpenGLWindow::SetMousePosition(const Point &position)
{
	Point pos;
	pos = Point(position.x(), Height() - position.y());
	pos += Position();
	set_mouse_position(static_cast<int32>(pos.x()), static_cast<int32>(pos.y()));
}


void OpenGLWindow::Tick()
{
}

/*!
	sets the window's title
	\brief Changes the window's title
	\param title the new desired window title
	\sa Title()
*/
void OpenGLWindow::SetTitle(const String &title)
{
	char buffer[256];
	m_title = title;
	m_window->SetTitle(title.ToChar(buffer, 256));
}

/*!
	\brief Returns the window's title
	\return the window's title
	\sa SetTitle(), Window()
*/
const String &OpenGLWindow::Title()
{
	return m_title;
}

/*!
	\brief Shows or hides the cursor
	\param show sets the cursor visible or invisible
	\sa IsCursorVisible()
*/
void OpenGLWindow::ShowCursor(bool show)
{
	m_cursor_visible = show;
	if (show) be_app->ShowCursor();
	else be_app->HideCursor();
}

/*!
	\brief Tells if the cursor is currently visible
	\return true if the cursor is visible
	\sa ShowCursor()
*/
bool OpenGLWindow::IsCursorVisible()
{
	return !be_app->IsCursorHidden();
}

/*!
	\brief Returns the window's width
	\return the width of the drawing area of the window, no title bars or borders will be included in the width
*/
int32 OpenGLWindow::Width()
{
	return static_cast<int32>(m_window->Bounds().Width());
}

/*!
	\brief Returns the window's height
	\return the height of the drawing area of the window, no title bars or borders will be included in the height
*/
int32 OpenGLWindow::Height()
{
	return static_cast<int32>(m_window->Bounds().Height());
}

/*!
	resizes the window to the given width and height.
	\brief Resizes the window
	\param width the new window width
	\param height the new window height
*/
void OpenGLWindow::Resize(float width, float height)
{
	m_window->ResizeTo(width, height);
}

/*!
	moves the window to the given position in the screen
	coordinate space. The upper-left corner will
	be placed in the given position
	\brief Moves the window
	\param position the position the window's new upper left corner will be moved to
*/
void OpenGLWindow::MoveTo(const Point &position)
{
	m_window->MoveTo(position.x(), position.y());
}

/*!
	\brief Returns the window's current position
	\return the window's upper-left corder's current position in screen coordinate space
*/
Point OpenGLWindow::Position()
{
	BRect r = m_window->Frame();
	return Point(r.left, r.top);
}

/********************************************************************/
// BeGLWindow

OpenGLWindow::BeGLWindow::BeGLWindow(Window *parent, const Rect &frame, uint32 flags, uint32 options):
	BDirectWindow(BRect(frame.x, frame.y, frame.x + frame.width, frame.y + frame.height),
		"DwarfEngine window", (flags & Window::BORDERLESS)? B_NO_BORDER_WINDOW_LOOK : B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS),
	m_glview(BRect(0, 0, frame.width, frame.height), "OpenGL view", B_FOLLOW_ALL_SIDES, B_WILL_DRAW, BGL_RGB + BGL_DOUBLE + BGL_DEPTH),
	m_parent(parent)
{
	// look at the flags and do something about them

	uint32 b_flags = 0;
	if (flags & Window::UNRESIZABLE) b_flags |= B_NOT_RESIZABLE | B_NOT_ZOOMABLE;
	if (flags & Window::UNCLOSABLE) b_flags |= B_NOT_CLOSABLE;
	if (flags & Window::UNMOVABLE) b_flags |= B_NOT_MOVABLE;
	SetFlags(b_flags);
	
	if (flags & Window::FULLSCREEN) SetFullScreen(true);

	if (flags & Window::CENTERED)
	{
		BScreen scr(this);
		BRect scr_frame = scr.Frame();
		Rect frame = Rect(0, 0, Bounds().Width(), Bounds().Height());
		BPoint left_top = BPoint(scr_frame.right / 2.0 - frame.width / 2.0,scr_frame.bottom / 2.0 - frame.height / 2.0); 
		MoveTo(left_top);
	}

	AddChild(&m_glview);
}

OpenGLWindow::BeGLWindow::~BeGLWindow()
{
}

void *OpenGLWindow::BeGLWindow::operator new(size_t size)
{
	return malloc(size);
}

void OpenGLWindow::BeGLWindow::operator delete(void *ptr)
{
	free(ptr);
}

void OpenGLWindow::BeGLWindow::DirectConnected(direct_buffer_info *info)
{
	m_glview.DirectConnected(info);
	m_glview.EnableDirectMode(true);
}

bool OpenGLWindow::BeGLWindow::QuitRequested()
{
	// if the game is still running,
	// tell the window that the user wants to close it
	m_parent->PostMessage(D_QUIT);
	return false;
}

void OpenGLWindow::BeGLWindow::Quit()
{
	// the window is apperantly ready to be closed
	m_glview.EnableDirectMode(false);
	RemoveChild(&m_glview);
	Lock();
	BDirectWindow::Quit();
}

