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
#include "InputServer.h"
#include "Window.h"
#include "InputReceiver.h"
#include "InputServerBase.h"

using namespace dwarf;

namespace
{
	int32 translate_vk2scancode(int32 scan)
	{
		switch (scan)
		{
		case VK_BACK: return InputReceiver::SC_BACKSPACE;
		case VK_TAB: return InputReceiver::SC_TAB;
		case VK_RETURN: return InputReceiver::SC_ENTER;
		case VK_ESCAPE: return InputReceiver::SC_ESCAPE;
		case VK_LSHIFT: return InputReceiver::SC_LSHIFT;
		case VK_RSHIFT: return InputReceiver::SC_RSHIFT;
		case VK_LCONTROL: return InputReceiver::SC_LCTRL;
		case VK_RCONTROL: return InputReceiver::SC_RCTRL;
		case VK_LMENU: return InputReceiver::SC_LALT;
		case VK_RMENU: return InputReceiver::SC_RALT;
		case VK_LWIN: return InputReceiver::SC_LWINDOWS;
		case VK_RWIN: return InputReceiver::SC_RWINDOWS;
		case VK_APPS: return InputReceiver::SC_APPS; // applications key, left of the right control key on windows keyboards
		case VK_SPACE: return InputReceiver::SC_SPACE;
		case VK_PRIOR: return InputReceiver::SC_PAGEUP;
		case VK_NEXT: return InputReceiver::SC_PAGEDOWN;
		case VK_INSERT: return InputReceiver::SC_INSERT;
		case VK_DELETE: return InputReceiver::SC_DELETE;
		case VK_HOME: return InputReceiver::SC_HOME;
		case VK_END: 	return InputReceiver::SC_END;
		case VK_PAUSE: return InputReceiver::SC_PAUSE;
		case VK_CAPITAL: 	return InputReceiver::SC_CAPS_LOCK;
		case VK_SCROLL: return InputReceiver::SC_SCROLL_LOCK;
		case VK_NUMLOCK: return InputReceiver::SC_NUM_LOCK;
		case VK_SNAPSHOT: return InputReceiver::SC_PRINT_SCREEN;
		case VK_LEFT: return InputReceiver::SC_LEFT;
		case VK_RIGHT: return InputReceiver::SC_RIGHT;
		case VK_UP: return InputReceiver::SC_UP;
		case VK_DOWN: return InputReceiver::SC_DOWN;
		case 0xdc: return InputReceiver::SC_CONSOLE;
		case VK_F1: return InputReceiver::SC_F1;
		case VK_F2: return InputReceiver::SC_F2;
		case VK_F3: return InputReceiver::SC_F3;
		case VK_F4: return InputReceiver::SC_F4;
		case VK_F5: return InputReceiver::SC_F5;
		case VK_F6: return InputReceiver::SC_F6;
		case VK_F7: return InputReceiver::SC_F7;
		case VK_F8: return InputReceiver::SC_F8;
		case VK_F9: return InputReceiver::SC_F9;
		case VK_F10: return InputReceiver::SC_F10;
		case VK_F11: return InputReceiver::SC_F11;
		case VK_F12: return InputReceiver::SC_F12;
		case VK_NUMPAD0: return InputReceiver::SC_NUMPAD_0;
		case VK_NUMPAD1: return InputReceiver::SC_NUMPAD_1;
		case VK_NUMPAD2: return InputReceiver::SC_NUMPAD_2;
		case VK_NUMPAD3: return InputReceiver::SC_NUMPAD_3;
		case VK_NUMPAD4: return InputReceiver::SC_NUMPAD_4;
		case VK_NUMPAD5: return InputReceiver::SC_NUMPAD_5;
		case VK_NUMPAD6: return InputReceiver::SC_NUMPAD_6;
		case VK_NUMPAD7: return InputReceiver::SC_NUMPAD_7;
		case VK_NUMPAD8: return InputReceiver::SC_NUMPAD_8;
		case VK_NUMPAD9: return InputReceiver::SC_NUMPAD_9;
		
		case VK_DECIMAL: return InputReceiver::SC_NUMPAD_DECIMAL;
		case VK_DIVIDE: return InputReceiver::SC_NUMPAD_DIVIDE;
		case VK_MULTIPLY:return InputReceiver::SC_NUMPAD_MULTIPLY;
		case VK_SUBTRACT:return InputReceiver::SC_NUMPAD_SUBTRACT;
		case VK_ADD:	return InputReceiver::SC_NUMPAD_ADD;

		default: return scan;
		};
//		return 0;
	}

	uint8 keyboard[256];

	uint32 GetModifiers()
	{
		uint32 mod = 0;

		if (!GetKeyboardState(keyboard))
			logger(TEXT("Failed to get keyboard state!"));
		if (keyboard[VK_LCONTROL] & 128) mod += InputReceiver::M_LEFT_CONTROL;
		if (keyboard[VK_LSHIFT] & 128) mod += InputReceiver::M_LEFT_SHIFT;
		if (keyboard[VK_LMENU] & 128) mod += InputReceiver::M_LEFT_ALT;
		if (keyboard[VK_RCONTROL] & 128) mod += InputReceiver::M_RIGHT_CONTROL;
		if (keyboard[VK_RSHIFT] & 128) mod += InputReceiver::M_RIGHT_SHIFT;
		if (keyboard[VK_RMENU] & 128) mod += InputReceiver::M_RIGHT_ALT;
		if (keyboard[VK_LWIN] & 128) mod += InputReceiver::M_LEFT_WINDOWS;
		if (keyboard[VK_RWIN] & 128) mod += InputReceiver::M_RIGHT_WINDOWS;
		if (keyboard[VK_APPS] & 128) mod += InputReceiver::M_MENU;
		return mod;
	}

	bool IsLeftPressed(uint32 key)
	{
		if (!GetKeyboardState(keyboard))
			logger(TEXT("Failed to get keyboard state!"));

		switch (key)
		{
		case InputReceiver::SC_LCTRL:
		case InputReceiver::SC_RCTRL:
			{
				if (keyboard[VK_LCONTROL] & 128)
					return true;
			} break;
		case InputReceiver::SC_LSHIFT:
		case InputReceiver::SC_RSHIFT:
			{
				if (keyboard[VK_LSHIFT] & 128)
					return true;
			} break;
		case InputReceiver::SC_LALT:
		case InputReceiver::SC_RALT:
			{
				if (keyboard[VK_LMENU] & 128)
					return true;
			} break;
		}
		return false;
	}
}

InputServerBase::InputServerBase(InputServer *is) : m_is(is), m_wnd(0)
{
}

InputServerBase::~InputServerBase()
{
	if (m_wnd && m_wnd->HasInputServer() && m_wnd->GetInputServer() == m_is)
	{
		m_wnd->SetInputServer(0);
	}
}

void InputServerBase::AttachedToWindow(Window *wnd)
{
	m_wnd = wnd;
}

int InputServerBase::HandleMessage(WindowMessage wm)
{
	static Point mousePos(0,0);
	static uint16 the_scancode = 0;
	std::list<InputReceiver*>::iterator i;

	switch (wm.msg)
	{
		case WM_SYSCHAR:
		case WM_CHAR:
		{
			if (wm.wparam == TEXT('\r'))
			{
				wm.wparam = TEXT('\n');
			}
			else if (wm.wparam == VK_CONTROL)
			{
				wm.wparam = IsLeftPressed(InputReceiver::SC_LCTRL) ? VK_LCONTROL : VK_RCONTROL;
			}
			else if (wm.wparam == VK_SHIFT)
			{
				wm.wparam = IsLeftPressed(InputReceiver::SC_LSHIFT) ? VK_LSHIFT : VK_RSHIFT;
			}
			else if (wm.wparam == VK_MENU)
			{
				wm.wparam = IsLeftPressed(InputReceiver::SC_LALT) ? VK_LMENU : VK_RMENU;
			}

			m_is->KeyChar(wm.wparam, translate_vk2scancode(the_scancode), GetModifiers());
		}
		break;

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if(!(wm.lparam & 0x40000000)) // anti key-repeat
			{
				if (wm.wparam == VK_CONTROL)
				{
					wm.wparam = IsLeftPressed(InputReceiver::SC_LCTRL) ? VK_LCONTROL : VK_RCONTROL;
				}
				else if (wm.wparam == VK_SHIFT)
				{
					wm.wparam = IsLeftPressed(InputReceiver::SC_LSHIFT) ? VK_LSHIFT : VK_RSHIFT;
				}
				else if (wm.wparam == VK_MENU)
				{
					wm.wparam = IsLeftPressed(InputReceiver::SC_LALT) ? VK_LMENU : VK_RMENU;
				}
				m_is->KeyDown(tolower(LOWORD(MapVirtualKey(wm.wparam, 2))), translate_vk2scancode(wm.wparam), GetModifiers());
			}
			
			MSG _msg;
			
			_msg.hwnd = wm.hwnd;
			_msg.wParam = wm.wparam;
			_msg.lParam = wm.lparam;
			_msg.message = wm.msg;

			if(!TranslateMessage(&_msg))
			{
				m_is->KeyChar(0, translate_vk2scancode(the_scancode), GetModifiers());
			}
			the_scancode = wm.wparam;

		}
		break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			if (wm.wparam == VK_CONTROL)
			{
				wm.wparam = IsLeftPressed(InputReceiver::SC_LCTRL) ? VK_LCONTROL : VK_RCONTROL;
			}
			else if (wm.wparam == VK_SHIFT)
			{
				wm.wparam = IsLeftPressed(InputReceiver::SC_LSHIFT) ? VK_LSHIFT : VK_RSHIFT;
			}
			else if (wm.wparam == VK_MENU)
			{
				wm.wparam = IsLeftPressed(InputReceiver::SC_LALT) ? VK_LMENU : VK_RMENU;
			}
			m_is->KeyUp(
				tolower(LOWORD(MapVirtualKey(wm.wparam, 2))), 
				translate_vk2scancode(wm.wparam), 
				GetModifiers());
		}
		break;


		case WM_MOUSEMOVE:
		{
			mousePos.x() = LOWORD(wm.lparam);
			mousePos.y() = HIWORD(wm.lparam);
			mousePos.y() = m_wnd->Height() - mousePos.y();

			if(m_is->RelativeMouseMode())
			{
				if (mousePos == m_wnd->Bounds().Midpoint()) break;
				Point pnt(mousePos);
				pnt -= m_wnd->Bounds().Midpoint();
				if(!pnt.x() && !pnt.y()) return 0;

				m_is->RelativeMouseMove(pnt);
				m_wnd->SetMousePosition(m_wnd->Bounds().Midpoint());
			}
			else
			{
				m_is->MouseMove(mousePos);
			}
		}
		break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			Point mousePos;
			mousePos.x() = LOWORD(wm.lparam);
			mousePos.y() = HIWORD(wm.lparam);
			mousePos.y() = m_wnd->Height() - mousePos.y();

			int32 index;
			switch (wm.msg)
			{
				case WM_LBUTTONDOWN: index = 0; break;
				case WM_RBUTTONDOWN: index = 1; break;
				case WM_MBUTTONDOWN: index = 2; break;
				default: index = 0;
			}

			m_is->MouseDown(mousePos, index, GetModifiers());
		}
		break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			mousePos.x() = LOWORD(wm.lparam);
			mousePos.y() = HIWORD(wm.lparam);
			mousePos.y() = m_wnd->Height() - mousePos.y();

			int32 index;
			switch (wm.msg)
			{
				case WM_LBUTTONUP: index = 0; break;
				case WM_RBUTTONUP: index = 1; break;
				case WM_MBUTTONUP: index = 2; break;
				default: index = 0;
			}

			m_is->MouseUp(mousePos, index, GetModifiers());
		}
		break;
	
		case WM_MOUSEWHEEL:
		{
			mousePos.x() = LOWORD(wm.lparam);
			mousePos.y() = HIWORD(wm.lparam);
			mousePos.y() = m_wnd->Height() - mousePos.y();
			Point wheel;
			if((short) HIWORD(wm.wparam) > 0)
				wheel = Point(0.0, 1.0); // up
			else
				wheel = Point(0.0,-1.0); // down

			m_is->MouseWheel(wheel, mousePos, 0);
		}
		break;

		default:
			return DefWindowProc(wm.hwnd, wm.msg, wm.wparam, wm.lparam);
			break;
	}
	return 0;
}
