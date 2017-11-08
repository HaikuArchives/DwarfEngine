#ifndef D_INPUTHANDLERS_H_
#define D_INPUTHANDLERS_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

// This file contains a couple of utility
// classes useful when dealing with input.

namespace dwarf
{
	enum IR_CallOnEvent
	{
		IR_KEYCHAR,
		IR_KEYDOWN,
		IR_KEYUP,
		IR_MOUSEMOVE,
		IR_MOUSEDOWN,
		IR_MOUSEUP,
		IR_ALLKEY,
		IR_ALLMOUSE,
		IR_ALL
	};

	// InputRedirector
	// Instantiated for a certain type of object,
	// it takes a pointer to such an object
	// and also a method pointer.
	// It then calls this method whenever a certain
	// event is generated.
	template <class T, IR_CallOnEvent event = IR_ALL> class InputRedirector : public InputReceiver
	{
	public:
		InputRedirector(T* object, void (T::*fun)()) : m_fun(fun), m_obj(object) {}

		void KeyDown(dchar c, uint32 k, uint32 m)
		{
			if (event == IR_KEYDOWN)
			{
				(m_obj->*m_fun)();
			}
		}

	private:
		T* m_obj;
		void (T::*m_fun)();
	};

}

#endif // D_INPUTHANDLERS_H_