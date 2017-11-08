/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <Message.h>
#include <SupportDefs.h>
#include <OS.h>
#include <String.h>
#include <UTF8.h>
#include <Window.h>
#include <View.h>
#include <GameKit.h>
#include <InterfaceKit.h>
#include <Rect.h>

#include "Defs.h"
#include "InputServer.h"
#include "Window.h"
#include "InputReceiver.h"
#include "InputServerBase.h"


int32 scancode2dwarf(int32 scan)
{
	switch (scan)
	{
	case 0x1e: return dwarf::InputReceiver::SC_BACKSPACE;
	case 0x26: return dwarf::InputReceiver::SC_TAB;
	case 0x47: return dwarf::InputReceiver::SC_ENTER;
	case 0x01: return dwarf::InputReceiver::SC_ESCAPE;

	case 0x4b: return dwarf::InputReceiver::SC_LSHIFT;
	case 0x56: return dwarf::InputReceiver::SC_RSHIFT;
	case 0x5c: return dwarf::InputReceiver::SC_LCTRL;
	case 0x60: return dwarf::InputReceiver::SC_RCTRL;
	case 0x5d: return dwarf::InputReceiver::SC_LALT;
	case 0x5f: return dwarf::InputReceiver::SC_RALT;
//	case VK_LWIN: return InputReceiver::SC_LWINDOWS;
//	case VK_RWIN: return InputReceiver::SC_RWINDOWS;
//	case VK_APPS: return InputReceiver::SC_APPS; // applications key, left of the right control key on windows keyboards
	case 0x5e: return dwarf::InputReceiver::SC_SPACE;
//	case VK_PRIOR: return InputReceiver::SC_PAGEUP;
//	case VK_NEXT: return InputReceiver::SC_PAGEDOWN;
//	case VK_INSERT: return InputReceiver::SC_INSERT;
//	case VK_DELETE: return InputReceiver::SC_DELETE;
//	case VK_HOME: return InputReceiver::SC_HOME;
//	case VK_END: 	return InputReceiver::SC_END;
//	case VK_PAUSE: return InputReceiver::SC_PAUSE;
	case 0x3b: 	return dwarf::InputReceiver::SC_CAPS_LOCK;
//	case VK_SCROLL: return InputReceiver::SC_SCROLL_LOCK;
//	case VK_NUMLOCK: return InputReceiver::SC_NUM_LOCK;
//	case VK_SNAPSHOT: return InputReceiver::SC_PRINT_SCREEN;
//	case VK_LEFT: return InputReceiver::SC_LEFT;
//	case VK_RIGHT: return InputReceiver::SC_RIGHT;
//	case VK_UP: return InputReceiver::SC_UP;
//	case VK_DOWN: return InputReceiver::SC_DOWN;
	case 0xdc: return dwarf::InputReceiver::SC_CONSOLE;
	case 0x02: return dwarf::InputReceiver::SC_F1;
	case 0x03: return dwarf::InputReceiver::SC_F2;
	case 0x04: return dwarf::InputReceiver::SC_F3;
	case 0x05: return dwarf::InputReceiver::SC_F4;
	case 0x06: return dwarf::InputReceiver::SC_F5;
	case 0x07: return dwarf::InputReceiver::SC_F6;
	case 0x08: return dwarf::InputReceiver::SC_F7;
	case 0x09: return dwarf::InputReceiver::SC_F8;
	case 0x0a: return dwarf::InputReceiver::SC_F9;
	case 0x0b: return dwarf::InputReceiver::SC_F10;
	case 0x0c: return dwarf::InputReceiver::SC_F11;
	case 0x0d: return dwarf::InputReceiver::SC_F12;
//	case VK_NUMPAD0: return InputReceiver::SC_NUMPAD_0;
//	case VK_NUMPAD1: return InputReceiver::SC_NUMPAD_1;
//	case VK_NUMPAD2: return InputReceiver::SC_NUMPAD_2;
//	case VK_NUMPAD3: return InputReceiver::SC_NUMPAD_3;
//	case VK_NUMPAD4: return InputReceiver::SC_NUMPAD_4;
//	case VK_NUMPAD5: return InputReceiver::SC_NUMPAD_5;
//	case VK_NUMPAD6: return InputReceiver::SC_NUMPAD_6;
//	case VK_NUMPAD7: return InputReceiver::SC_NUMPAD_7;
//	case VK_NUMPAD8: return InputReceiver::SC_NUMPAD_8;
//	case VK_NUMPAD9: return InputReceiver::SC_NUMPAD_9;
	
//	case VK_DECIMAL: return InputReceiver::SC_NUMPAD_DECIMAL;
//	case VK_DIVIDE: return InputReceiver::SC_NUMPAD_DIVIDE;
//	case VK_MULTIPLY:return InputReceiver::SC_NUMPAD_MULTIPLY;
//	case VK_SUBTRACT:return InputReceiver::SC_NUMPAD_SUBTRACT;
//	case VK_ADD:	return InputReceiver::SC_NUMPAD_ADD;
	default: return scan;
	};
	return 0;
}


dwarf::InputServerBase::InputServerBase(dwarf::InputServer *is):
	m_is(is),
	m_wnd(0),
	m_view(is)
{
}

dwarf::InputServerBase::~InputServerBase()
{
	if (m_wnd && m_wnd->HasInputServer() && m_wnd->GetInputServer() == m_is)
		m_wnd->SetInputServer(0);
}

void dwarf::InputServerBase::AttachedToWindow(Window *wnd)
{
	m_wnd = wnd;
}







dwarf::InputServerBase::BeInputView::BeInputView(dwarf::InputServer *server):
	BView(BRect(0,0,10,10), "InputView", B_FOLLOW_ALL_SIDES, 0),
	m_parent(0),
	m_server(server),
	m_mouse_pos(BPoint(0.0, 0.0)),
	m_orig_mouse_pos(BPoint(0.0,0.0)),
	m_mouse_buttons(0),
	m_focus(false)
{
	SetEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY);
}

void dwarf::InputServerBase::BeInputView::SetWindow(dwarf::Window *win)
{
	m_parent = win;
}

dwarf::Point dwarf::InputServerBase::BeInputView::MousePosition()
{
	return Point(m_mouse_pos.x, Bounds().bottom - m_mouse_pos.y);
}

void dwarf::InputServerBase::BeInputView::KeyDown(const char *key_string, int32 numBytes)
{
	int32 key;
	int32 modifier_mask;
	wchar_t dst_char;
	int32 dummy;
		
	// convert the incoming char to another encoding
	int32 src_len = 3;
	int32 dst_len = 2;

	// retreive the information about the pressed key			
	Window()->CurrentMessage()->FindInt32("key", &key);
	Window()->CurrentMessage()->FindInt32("modifiers",&modifier_mask);

	convert_from_utf8(B_UNICODE_CONVERSION,
							key_string,
							&src_len,
			  				(char*)&dst_char,
			   				&dst_len,
							&dummy);

	#ifdef D_LITTLE_ENDIAN

	dst_char = swap_byteorder(dst_char);

	#endif

	m_server->KeyChar(dst_char, key, modifier_mask);

	// if this key was not repeated (if the key was actually pressed instead of just been hold down to generate a repeat)
	if (Window()->CurrentMessage()->FindInt32("be:key_repeat",&dummy) != B_OK)
	{
		m_server->KeyDown(dst_char, scancode2dwarf(key), modifier_mask);
	}
}

void dwarf::InputServerBase::BeInputView::KeyUp(const char *key_string, int32 numBytes)
{
	int32 dummy;
	int32 key;
	int32 modifier_mask;
	wchar_t dst_char;

	Window()->CurrentMessage()->FindInt32("key",&key);
	Window()->CurrentMessage()->FindInt32("modifiers",&modifier_mask);

	int32 src_len = numBytes;
	int32 dst_len = 10;

	convert_from_utf8(B_UNICODE_CONVERSION,
									key_string,
									&src_len,
			  						(char*)&dst_char,
			   						&dst_len,
									&dummy);

	#ifdef D_LITTLE_ENDIAN

	dst_char = swap_byteorder(dst_char);

	#endif

	m_server->KeyUp(dst_char, scancode2dwarf(key), modifier_mask);
}

void dwarf::InputServerBase::BeInputView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
	case B_MOUSE_WHEEL_CHANGED:
	{
		Point delta_wheel;
		int32 modifier_mask;
		
		message->FindFloat("be:wheel_delta_x",&delta_wheel.x());
		message->FindFloat("be:wheel_delta_y",&delta_wheel.y());
		message->FindInt32("modifiers",&modifier_mask);

		m_server->MouseWheel(delta_wheel, MousePosition(), modifier_mask);
	}break;
	case B_UNMAPPED_KEY_DOWN:
	{
		int32 key;
		int32 modifier_mask;

		int32 dummy;
		if (message->FindInt32("be:key_repeat",&dummy) != B_OK)
		{
			message->FindInt32("key",&key);
			message->FindInt32("modifiers",&modifier_mask);

			m_server->KeyDown(0, key, modifier_mask);
		}
	}break;
	case B_UNMAPPED_KEY_UP:
	{
		int32 key;
		int32 modifier_mask;
		message->FindInt32("key",&key);
		message->FindInt32("modifiers",&modifier_mask);

		m_server->KeyUp(0, key, modifier_mask);
	}break;
/*
	case B_KEY_DOWN:
	{
		int32 key;
		int32 modifier_mask;
		BString key_string;
		wchar_t dst_char;
		int32 dummy;
			
		// convert the incoming char to another encoding
		int32 src_len = 3;
		int32 dst_len = 2;

		// retreive the information about the pressed key			
		message->FindInt32("key",&key);
		message->FindString("bytes",&key_string);
		message->FindInt32("modifiers",&modifier_mask);

		convert_from_utf8(B_UNICODE_CONVERSION,
								key_string.String(),
								&src_len,
				  				(char*)&dst_char,
				   				&dst_len,
								&dummy);

		#ifdef D_LITTLE_ENDIAN

		dst_char = swap_byteorder(dst_char);

		#endif

		m_server->KeyChar(dst_char, key, modifier_mask);

		// if this key was not repeated (if the key was actually pressed instead of just been hold down to generate a repeat)
		if (message->FindInt32("be:key_repeat",&dummy) != B_OK)
		{
			m_server->KeyDown(dst_char, scancode2dwarf(key), modifier_mask);
		}
	}break;
	case B_KEY_UP:
	{
		int32 dummy;
		int32 key;
		int32 modifier_mask;
		wchar_t dst_char;
		BString key_string;

		message->FindInt32("key",&key);
		message->FindString("bytes",&key_string);
		message->FindInt32("modifiers",&modifier_mask);

		int32 src_len = key_string.Length();
		int32 dst_len = 10;

		convert_from_utf8(B_UNICODE_CONVERSION,
										key_string.String(),
										&src_len,
				  						(char*)&dst_char,
				   						&dst_len,
										&dummy);

		#ifdef D_LITTLE_ENDIAN

		dst_char = swap_byteorder(dst_char);

		#endif

		m_server->KeyUp(dst_char, scancode2dwarf(key), modifier_mask);
	}break;
*/
	}
}

void dwarf::InputServerBase::BeInputView::MouseMoved(BPoint point, uint32, const BMessage *)
{
	if (!m_focus) return;
	if (point == m_mouse_pos) return; // ignore mouse_moves without the mouse moves
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
	if (m_server->RelativeMouseMode())
	{
		// *** relative mode ***
		BRect frame = Bounds();
		BPoint mid_point = BPoint( static_cast<int>((frame.right + 1) / 2.0),static_cast<int>((frame.bottom + 1) / 2.0));

		BPoint left_top = Window()->Frame().LeftTop();
		set_mouse_position(static_cast<int32>(mid_point.x + left_top.x), static_cast<int32>(mid_point.y + left_top.y));
		m_mouse_pos = mid_point - BPoint(0,1);

		m_server->RelativeMouseMove(Point(point.x - m_mouse_pos.x, Bounds().bottom - point.y - m_mouse_pos.y));
	}
	else
	{
		// *** absolute (normal) mode ***
		m_server->MouseMove(Point(point.x, Bounds().bottom - point.y));
	}

	m_mouse_pos = point;
}

void dwarf::InputServerBase::BeInputView::MouseDown(BPoint point)
{
	int32 button;
	int32 modifier_mask;

	BMessage *bmessage = Window()->CurrentMessage();
	bmessage->FindInt32("buttons",&button);
	bmessage->FindInt32("modifiers",&modifier_mask);

	int32 tmp_button = button;

	// find out the changed button
	button ^= m_mouse_buttons;
	// find out the index of the mouse button
	uint8 index = 0;
	while((button & 1 == 0) && index < 6)
	{
		index++;
		button >>= 1;
	}

	if (index == 6) return;

	if (m_server->RelativeMouseMode())
		m_server->MouseDown(Point(0, 0), button, modifier_mask);
	else
		m_server->MouseDown(Point(point.x, Bounds().bottom - point.y), button, modifier_mask);

	m_mouse_buttons = tmp_button;
}

void dwarf::InputServerBase::BeInputView::MouseUp(BPoint point)
{
	int32 button;
	int32 modifier_mask;

	BMessage *bmessage = Window()->CurrentMessage();
	bmessage->FindInt32("buttons",&button);
	bmessage->FindInt32("modifiers",&modifier_mask);

	int32 tmp_button = button;

	// find out the changed button
	button ^= m_mouse_buttons;
	// find out the index of the mouse button
	uint8 index = 0;
	while(! (button&1) && index < 6)
	{
		index++;
		button >>= 1;
	}

	if (index == 6) return;

	if (m_server->RelativeMouseMode())
		m_server->MouseUp(Point(0, 0), button, modifier_mask);
	else
		m_server->MouseUp(Point(0, 0), button, modifier_mask);

	m_mouse_buttons = tmp_button;
}

void dwarf::InputServerBase::BeInputView::MakeFocus(bool focus)
{
	m_focus = focus;
	if (m_focus) logger(TEXT("InputServerBase::BeInputFilter::m_focus = true"));
	else logger(TEXT("InputServerBase::BeInputFilter::m_focus = false"));
	BView::MakeFocus(focus);
}
/*
void InputServerBase::BeInputView::SetRelativeMouseMode(bool mode)
{
	m_mouse_mode = mode;

	Window()->Lock();
	if (m_mouse_mode)
	{
		m_orig_mouse_pos = m_mouse_pos;
		BRect frame = Bounds();
		BPoint mid_point = BPoint(static_cast<int32>((frame.right + 1) / 2.0),static_cast<int32>((frame.bottom + 1) / 2.0));
		BPoint left_top = Window()->Frame().LeftTop();
		set_mouse_position(static_cast<int32>(mid_point.x + left_top.x), static_cast<int32>(mid_point.y + left_top.y));
		m_mouse_pos = mid_point;
	}
	else
	{
		BPoint left_top = Window()->Frame().LeftTop();
		set_mouse_position(static_cast<int32>(left_top.x + m_orig_mouse_pos.x), static_cast<int32>(left_top.y + m_orig_mouse_pos.y));
	}
	Window()->Unlock();
}
*/
void dwarf::InputServerBase::BeInputView::AttachedToWindow()
{
	ResizeTo(Window()->Bounds().Width(), Window()->Bounds().Height());
	MakeFocus(true);
}

