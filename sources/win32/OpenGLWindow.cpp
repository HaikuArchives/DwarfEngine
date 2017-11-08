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
#ifdef _DEBUG
#include "mmgr.h"
#else
#include "nommgr.h"
#endif

#include "Window.h"
#include "WindowBase.h"
#include "InputServerBase.h"
#include "RenderingDevice.h"
#include "OpenGLWindow.h"
#include "OpenGLDevice.h"
#include "List.h"
#include "Directory.h"

using namespace dwarf;

#if !defined(D_STATIC_RENDERER) || defined(_USRDLL)


Directory *log_dir = 0;
File *log_file = 0;

#if !defined (D_STATIC_RENDERER)
	HINSTANCE g_hInstance = 0;
#else
	extern HINSTANCE g_hInstance;
#endif

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD, LPVOID) { g_hInstance = hInstance; return TRUE; }

#else

extern HINSTANCE g_hInstance;

#endif // D_STATIC_RENDERER

OpenGLWindow::OpenGLWindow(OpenGLDevice *device, Window *window, const Rect &frame, uint32 flags, uint32 options) : 
	m_device(device),
	m_window(window),
	m_hwnd(),
	m_cursor_visible(true),
	m_running(true),
	m_is_active(true),
	m_delay(1.0f / 120.0f)
{
	D_ASSERT(device);
	D_ASSERT(window);

#if !defined(D_STATIC_RENDERER) || defined(_USRDLL)
	

	FileSystem::SetBasePath();

	log_dir = new Directory();
	log_file = new File(log_dir, TEXT("gl.log"), File::WRITE, File::CREATE | File::TEXT);
	logger.PushOutput(log_file);

#endif // D_STATIC_RENDERER

	m_mode.w = frame.width;
	m_mode.h = frame.height;

	logger(Log::LOW, TEXT("Creating OpenGL Window..."));
	HDC temp = GetDC(HWND_DESKTOP);
	if(temp)
	{
		m_mode.d = GetDeviceCaps(temp, BITSPIXEL);
		logger(TEXT("Using desktop bitdepth (%d)"), m_mode.d);
	}
	else
	{
		m_mode.d = 16;
		logger(TEXT("Desktop bitdepth unknown. Set to 16 bits."));
	}
	ReleaseDC(HWND_DESKTOP, temp);

	logger(TEXT("Opening Window..."));
	OpenWindow(TEXT("hrm"), flags, frame);
}



OpenGLWindow::~OpenGLWindow()
{
	logger(Log::LOW, TEXT("Closing OpenGL Window..."));
	ShowCursor(true);
	CloseWindow();

#if !defined(D_STATIC_RENDERER) || defined(_USRDLL)
	

	logger.PopOutput();
	delete log_file;
	delete log_dir;

#endif
}



void OpenGLWindow::OpenWindow(const String &title, uint32 flags, const Rect &frame)
{
	int w = frame.width;
	int h = frame.height;

	WNDCLASSEX cl;
	HWND _hwnd;

	// Window class stuff
	cl.cbSize = sizeof(WNDCLASSEX);
	cl.hInstance = g_hInstance;
	cl.lpszClassName = D_WINDOW_CLASS;
	cl.lpfnWndProc = sWndProc;
	cl.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW; // OWNDC is important.
	cl.hIcon = LoadIcon(NULL, IDI_APPLICATION); //LoadIcon(g_hInstance, MAKEINTRESOURCE(101));
	cl.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //(HICON)LoadImage(g_hInstance, MAKEINTRESOURCE(101),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	cl.hCursor = LoadCursor(NULL, IDC_ARROW);
	cl.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	cl.cbClsExtra = 0;
	cl.cbWndExtra = 0;
	cl.lpszMenuName = NULL;

	RegisterClassEx(&cl);

	if(flags & Window::FULLSCREEN && m_device->GoFullScreen(w, h, m_mode.d))
	{
		_hwnd = CreateWindowEx
			(
			WS_EX_APPWINDOW | WS_EX_TOPMOST, // perhaps this fixes start button stuff showing up in NT
			D_WINDOW_CLASS,
			*title,
			WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			0,0,
			w,
			h,
			NULL,
			NULL,
			g_hInstance,
			NULL
			);
	}
	else
	{
		uint32 x, y, wndflags;
		if(flags & Window::CENTERED)
		{
			x = (GetSystemMetrics(SM_CXSCREEN) - w)>>1;
			y = (GetSystemMetrics(SM_CYSCREEN) - h)>>1;
		}
		else
		{
			x = frame.x;
			y = frame.y;
		}

		wndflags = WS_OVERLAPPED | WS_CAPTION;

		if(!(flags & Window::UNRESIZABLE))
		{
			wndflags |= WS_MAXIMIZEBOX;
			wndflags |= WS_MINIMIZEBOX;
			wndflags |= WS_SIZEBOX;
		}
		else
		{
			logger(TEXT("Window flag: UNRESIZABLE"));
			wndflags |= WS_BORDER;
		}

		if(!(flags & Window::UNCLOSABLE))
		{
			wndflags |= WS_SYSMENU;
		}
		else
			logger(TEXT("Window flag: UNCLOSABLE"));


		if((flags & Window::UNMOVABLE))
		{
			logger(TEXT("Window flag: UNMOVABLE, ignored"));
		}

		if((flags & Window::BORDERLESS))
		{
			logger(TEXT("Window flag: BORDERLESS, overriding.."));
			wndflags = WS_OVERLAPPED | WS_POPUP | WS_BORDER;
		}


		RECT r;
		r.left 	 = 0;
		r.top 	 = 0;
		r.right  = w;
		r.bottom = h;
		AdjustWindowRectEx( &r, wndflags, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );
		w = (r.right - r.left);
		h = (r.bottom - r.top);

		_hwnd = CreateWindowEx
			(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			D_WINDOW_CLASS,
			title,
			wndflags | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y,
			w, 
			h,
			NULL,
			NULL,
			g_hInstance,
			NULL
			);
	}


	if(!_hwnd)
		Error(TEXT("Failed to create window!"));

	m_hwnd = _hwnd;

	// store pointer to this in our window
	// so that we can identify ourselves in
	// window procedure
	::SetWindowLong (m_hwnd, GWL_USERDATA, reinterpret_cast<long> (this));

	m_title = title;
}





void OpenGLWindow::CloseWindow()
{
	Show(false);
	::SetWindowLong (m_hwnd, GWL_USERDATA, 0);
	if(m_window->Flags() & Window::FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// TODO: close device

	if(!DestroyWindow(m_hwnd))
	{
		logger(TEXT("Failed to close window."));
	}
	
	m_hwnd = NULL;
}





void OpenGLWindow::SetRelativeMouseMode(bool mode)
{
	if(!mode)
	{
		::ReleaseCapture();
	}
	else
	{
		::SetCapture(m_hwnd);
		SetMousePosition(Point(Width()/2.0, Height()/2.0));
	}

	ShowCursor(!mode);
}

/*
bool OpenGLWindow::RelativeMouseMode()
{
	return m_relative_mouse_mode;
}
*/

void OpenGLWindow::SetMousePosition(const Point &position)
{
//	RECT rect;
//	::GetWindowRect(m_hwnd, &rect);
//	::SetCursorPos(rect.left + position.x(), rect.bottom - position.y());

//	Point p = Point(position.x(), position.y()) + Position();
//	logger(TEXT("Position(): %f, %f"), Position().x(), Position().y());
//	::SetCursorPos(p.x(), p.y());

	Point p = position + Position() + Point(4, 23);
	::SetCursorPos(p.x(), p.y());
}





void OpenGLWindow::ShowCursor(bool show)
{
	// Workaround of windows refcounting cursor :P
	if(show != m_cursor_visible)
	{
		::ShowCursor(show);
		m_cursor_visible = !m_cursor_visible;
	}
}



bool OpenGLWindow::IsCursorVisible()
{

	return m_cursor_visible;

}




void OpenGLWindow::SetTitle(const String &title)
{

	m_title = title;
	SetWindowText(m_hwnd, *m_title);

}


const String& OpenGLWindow::Title()
{

	return m_title;

}

void	OpenGLWindow::MoveTo(const Point &position)
{
	
	SetWindowPos(m_hwnd, 0, position.x(), position.y(), 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);

}

/*!
	\todo this method does not work. It returns the window's border position
*/

Point	OpenGLWindow::Position()
{

	RECT r;
	::GetWindowRect(m_hwnd,&r);
	return Point(r.left, r.top);

}


Rect	OpenGLWindow::GetCanvasSize(const Rect &in_r) const
{
	//TODO: fix this function
	RECT canvas;
	GetClientRect(m_hwnd, &canvas);

	Rect ret;
	ret.x = in_r.x;
	logger(TEXT("OpenGLWindow::GetCanvasSize: %d %d %d"), canvas.bottom, canvas.top, (int)in_r.y);
	ret.y = canvas.bottom - canvas.top - in_r.y- 1;
	ret.width = in_r.width;
	ret.height = in_r.height;

	ret.PrintToLog();

	return ret;
}


void	OpenGLWindow::	Resize(float width, float height)
{
	uint32 wndflags = GetWindowLong(m_hwnd, GWL_STYLE);
	RECT r;
	r.left 	 = 0;
	r.top 	 = 0;
	r.right  = width;
	r.bottom = height;
	AdjustWindowRectEx( &r, wndflags, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );
//
	logger(TEXT("Resize to: %f, %f"), width, height);
	SetWindowPos(m_hwnd, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
	m_mode.w = width;
	m_mode.h = height;
}




int32	OpenGLWindow::	Width()
{
	RECT r;
	GetClientRect(m_hwnd, &r);
	return r.right - r.left;
}




int32	OpenGLWindow::	Height()
{
	RECT r;
	GetClientRect(m_hwnd, &r);
	return r.bottom - r.top;
}



void	OpenGLWindow::	Show(bool visible)
{
	if(visible)
	{
		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
	}
	else
	{
		logger(TEXT("Hid window..."));
		ShowWindow(m_hwnd, SW_HIDE);
	}
}




bool	OpenGLWindow::	IsVisible()
{
	return IsWindowVisible(m_hwnd) != 0;
}


void	OpenGLWindow::	Minimize(bool minimize)
{
	if(minimize)
	{
		ShowWindow(m_hwnd, SW_MINIMIZE);
	}
	else
	{
		ShowWindow(m_hwnd, SW_RESTORE);
		UpdateWindow(m_hwnd);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
	}
}

void OpenGLWindow::Tick()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		DispatchMessage(&msg);
	}
}

void OpenGLWindow::SetInputServer(InputServer *server)
{
}


int OpenGLWindow::SystemMessage(WindowMessage wm)
{
	PAINTSTRUCT ps;

	switch(wm.msg)
	{
		case WM_PAINT:
		case WM_SIZING:
		case WM_MOVING:
		{
			BeginPaint(wm.hwnd, &ps);
			EndPaint(wm.hwnd, &ps);
			ValidateRect(wm.hwnd, NULL );
		}
		break;

		case WM_ACTIVATE:
		{
			bool activated = (LOWORD(wm.wparam) == WA_INACTIVE) ? false : true;
			bool minimized = (HIWORD(wm.wparam))				? true	: false;

			if(minimized && m_device->IsFullScreen())
			{
				logger(TEXT("Window was minimized from fullscreen."));
				ShowCursor(true);
				::ReleaseCapture();
			}
			else if(activated && !m_is_active && m_device->IsFullScreen())
			{
				logger(TEXT("Window was activated and was fullscreen."));
				ShowCursor(false);
				::SetCapture(wm.hwnd);
			}

			if((!activated && m_is_active) || (minimized))
			{
				logger(TEXT("Window lost focus or was minimized."));
			}

			m_is_active = activated;

		}
		break;

		case WM_SYSCOMMAND:
		{
			switch (wm.wparam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				{
					if(m_device->IsFullScreen())
					{
						logger(TEXT("Prevented screensaver from kicking in."));
						return 0;
					}
				}
				break;
					
				default:
				{
					return DefWindowProc(wm.hwnd, wm.msg, wm.wparam, wm.lparam);
				}
				break;
			}
		}
		break;

		case WM_SIZE:
		{
			m_window->OnResize(Width(), Height());
		}
		break;


		case WM_DESTROY:
		case WM_CLOSE:
		case WM_QUIT:
		{
			if(m_device->IsFullScreen())
			{
				::ReleaseCapture();
				::ShowCursor(TRUE);
			}
			m_window->Close();//PostMessage(D_QUIT);
		}
		break;

		case WM_SYSCHAR:
		case WM_CHAR:
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		{
			if (m_window->HasInputServer())
			{
				return m_window->GetInputServer()->GetBase().HandleMessage(wm);
			}
		}

		default:
		{
			return DefWindowProc(wm.hwnd, wm.msg, wm.wparam, wm.lparam);
		}
		break;
	}
	return 0;
}

LRESULT WINAPI OpenGLWindow::sWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WindowMessage wm = {hwnd, msg, wparam, lparam };
	OpenGLWindow *window = reinterpret_cast<OpenGLWindow*> (::GetWindowLong (hwnd, GWL_USERDATA));

	if(window)
		return window->SystemMessage(wm);
	
	return ::DefWindowProc(hwnd, msg, wparam, lparam);
}
