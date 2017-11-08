#ifndef D_INPUTRECEIVER_H_
#define D_INPUTRECEIVER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Vertex.h"

namespace dwarf
{
	/*!
		\brief Interface that someone who wants to be able to receive input must inherit.

		When you've inherited this interface, you will be able to register with an input
		server (commonly a window), and the methods of this interface will be called by
		that input server.
	*/
	class InputReceiver
	{
	public:
		enum ScanCode
		{
			SC_BACKSPACE = 0x01,
			SC_TAB,
			SC_ENTER,
			SC_ESCAPE,
			SC_LSHIFT,
			SC_RSHIFT,
			SC_LCTRL,
			SC_RCTRL,
			SC_LALT,
			SC_RALT,
			SC_LWINDOWS,
			SC_RWINDOWS,
			SC_APPS, // applications key, left of the right control key on windows keyboards
			SC_SPACE = TEXT(' '),
			SC_PAGEUP = 0x100,
			SC_PAGEDOWN,
			SC_INSERT,
			SC_DELETE,
			SC_HOME,
			SC_END,
			SC_PAUSE,
			SC_CAPS_LOCK,
			SC_SCROLL_LOCK,
			SC_NUM_LOCK,
			SC_PRINT_SCREEN,
			SC_LEFT,
			SC_RIGHT,
			SC_UP,
			SC_DOWN,
			SC_CONSOLE, // key above tab, used for console in quake.
			SC_F1,
			SC_F2,
			SC_F3,
			SC_F4,
			SC_F5,
			SC_F6,
			SC_F7,
			SC_F8,
			SC_F9,
			SC_F10,
			SC_F11,
			SC_F12,
			SC_NUMPAD_0,
			SC_NUMPAD_1,
			SC_NUMPAD_2,
			SC_NUMPAD_3,
			SC_NUMPAD_4,
			SC_NUMPAD_5,
			SC_NUMPAD_6,
			SC_NUMPAD_7,
			SC_NUMPAD_8,
			SC_NUMPAD_9,
			
			SC_NUMPAD_DECIMAL,
			SC_NUMPAD_DIVIDE,
			SC_NUMPAD_MULTIPLY,
			SC_NUMPAD_SUBTRACT,
			SC_NUMPAD_ADD
		};


		enum Modifier
		{
			M_LEFT_SHIFT	= 0x001,
			M_RIGHT_SHIFT	= 0x002,
			M_LEFT_CONTROL	= 0x004,
			M_RIGHT_CONTROL	= 0x008,
			M_LEFT_WINDOWS	= 0x010,
			M_RIGHT_WINDOWS	= 0x020,
			M_LEFT_ALT		= 0x040,
			M_RIGHT_ALT		= 0x080,
			M_MENU			= 0x100,
			M_CAPS_LOCK		= 0x200
		};

		virtual ~InputReceiver() {}
	
		virtual void KeyDown(dchar character, uint32 scancode, uint32 modifiers) {}
		virtual void KeyChar(dchar character, uint32 scancode, uint32 modifiers) {}
		virtual void KeyUp(dchar character, uint32 scancode, uint32 modifiers) {}
	
		virtual void MouseMove(const Point &position) {}
		virtual void RelativeMouseMove(const Point &position) {}
		virtual void MouseDown(const Point &position, uint8 button_index, uint32 modifiers) {}
		virtual void MouseUp(const Point &position, uint8 button_index, uint32 modifiers) {}
		virtual void MouseWheel(const Point &delta_wheel, const Point &mouse_position, uint32 modifiers) {}
	};
	
} // dwarf

#endif // D_INPUTRECEIVER_H_
