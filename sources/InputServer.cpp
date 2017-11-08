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
#include "InputReceiver.h"
#include "InputServer.h"

#include "InputServerBase.h" // platform dependent

using namespace dwarf;

InputServer::InputServer():
	m_exclusive(0),
	m_inlist(),
	m_wnd(0),
	m_relative_mouse_mode(false)
{
	m_base = new InputServerBase(this);
}

InputServer::InputServer(Window* wnd):
	m_exclusive(0),
	m_inlist(),
	m_wnd(wnd),
	m_relative_mouse_mode(false)
{
	m_base = new InputServerBase(this);
	wnd->SetInputServer(this);
}

InputServer::~InputServer()
{
	delete m_base;
}

void InputServer::SetRelativeMouseMode(bool mode)
{
	m_relative_mouse_mode = mode;
	m_wnd->SetRelativeMouseMode(mode);
}

void InputServer::AttachedToWindow(Window *wnd)
{
// this line is not needed, right?
//	m_wnd->SetInputServer(this);

	m_wnd = wnd;
	m_base->AttachedToWindow(wnd);
}

/*
	Moves the given input receiver to the end of the queue
	\brief stops the input receiver from having exclusive access to all input
*/
void InputServer::SetNonExclusive(InputReceiver *i)
{
	std::list<InputReceiver*>::iterator k = m_inlist.begin();
	bool found = false;
	int32 c = m_exclusive;

	while (c)
	{
		if (*k == i)
		{
			m_inlist.erase(k);
			found = true;
			break;
		}

		c--;
	}

	if (m_exclusive > 0)
		m_exclusive--;

	if (found)
	{
		m_inlist.push_back(i);
	}
}

void InputServer::KeyDown(dchar character, uint32 scancode, uint32 modifiers)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->KeyDown(character, scancode, modifiers);
	}
}

void InputServer::KeyChar(dchar character, uint32 scancode, uint32 modifiers)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->KeyChar(character, scancode, modifiers);
	}
}

void InputServer::KeyUp(dchar character, uint32 scancode, uint32 modifiers)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->KeyUp(character, scancode, modifiers);
	}
}

void InputServer::MouseMove(const Point &position)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->MouseMove(position);
	}
}

void InputServer::RelativeMouseMove(const Point &position)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->RelativeMouseMove(position);
	}
}

void InputServer::MouseDown(const Point &position, uint8 button_index, uint32 modifiers)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->MouseDown(position, button_index, modifiers);
	}
}

void InputServer::MouseUp(const Point &position, uint8 button_index, uint32 modifiers)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->MouseUp(position, button_index, modifiers);
	}
}

void InputServer::MouseWheel(const Point &delta_wheel, const Point &mouse_position, uint32 modifiers)
{
	for (std::list<InputReceiver*>::iterator i = m_inlist.begin(); i != m_inlist.end(); ++i)
	{
		(*i)->MouseWheel(delta_wheel, mouse_position, modifiers);
	}
}
