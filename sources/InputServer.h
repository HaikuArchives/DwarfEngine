#ifndef D_INPUTSERVER_H_
#define D_INPUTSERVER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <list>
#include <algorithm>
#include "mmgr.h"


#include "InputReceiver.h"
//#include "InputServerBase.h"

namespace dwarf
{

	class Window;

	/*!
		The input server is responsible for sending all forms
		of input to registered input receivers.
		\brief Platform dependent interface to the input server.
		\todo move all comments to source file
	*/
	class InputServer
	{
	friend class Window;
	friend class InputServerBase;
	friend class InputReceiver;
	public:
		InputServer();

		/*
			\brief the inputserver automajikally sets itself as server of the given window
		*/
		InputServer(Window* wnd);

		virtual ~InputServer();

		void SetRelativeMouseMode(bool mode);
		bool RelativeMouseMode() { return m_relative_mouse_mode; }
		
		void Add(InputReceiver *i); 
		void Remove(InputReceiver *i); 
		void PushToFront(InputReceiver *i);
		void PushToBack(InputReceiver *i);
		
		inline void SetExclusive(InputReceiver *i);
		void SetNonExclusive(InputReceiver *i);

		inline bool IsTopLevel(InputReceiver *i);
		inline bool IsExclusive(InputReceiver *i);

		inline InputServerBase &GetBase();

		virtual void KeyDown(dchar character, uint32 scancode, uint32 modifiers);
		virtual void KeyChar(dchar character, uint32 scancode, uint32 modifiers);
		virtual void KeyUp(dchar character, uint32 scancode, uint32 modifiers);
		virtual void MouseMove(const Point &position);
		virtual void RelativeMouseMove(const Point &position);
		virtual void MouseDown(const Point &position, uint8 button_index, uint32 modifiers);
		virtual void MouseUp(const Point &position, uint8 button_index, uint32 modifiers);
		virtual void MouseWheel(const Point &delta_wheel, const Point &mouse_position, uint32 modifiers);

	private:

		void AttachedToWindow(Window *wnd);

		int32 m_exclusive; // number of receivers with exclusive access (only the top level one gets it)
		std::list<InputReceiver*> m_inlist;
		Window *m_wnd;
		InputServerBase *m_base;

		bool m_relative_mouse_mode;
	};

	/*			.inline			*/

	/*
		\brief adds i to the end of the input queue
	*/
	inline void InputServer::Add(InputReceiver *i)
	{
		PushToBack(i);
	}
			
	/*
		\brief removes i from the input queue
	*/
	inline void InputServer::Remove(InputReceiver *i)
	{
		m_inlist.remove(i);
	}

	/*
		\brief pushes i to the front of the queue
	*/
	inline void InputServer::PushToFront(InputReceiver *i)
	{
		Remove(i);
		m_inlist.push_front(i);
	}

	/*
		\brief adds i to the end of the queue
	*/
	inline void InputServer::PushToBack(InputReceiver *i)
	{
		Remove(i);
		m_inlist.push_back(i);
	}

	/*
		Moves the given input receiver to the front of the queue
		\brief gives the input receiver exclusive access to all input
	*/
	inline void InputServer::SetExclusive(InputReceiver *i)
	{
		std::list<InputReceiver*>::iterator k = std::find(m_inlist.begin(), m_inlist.end(), i);

		if (k != m_inlist.end())
		{
			m_exclusive++;
			PushToFront(i);
		}
	}

	/*
		\brief true if i is at front of the queue
	*/
	inline bool InputServer::IsTopLevel(InputReceiver *i)
	{
		return (m_inlist.size() != 0) && (m_inlist.front() == i);
	}

	/*
		\brief true if i is the only input receiver active
	*/
	inline bool InputServer::IsExclusive(InputReceiver *i)
	{
		return m_exclusive && IsTopLevel(i);
	}

	/*
		\brief returns pointer to base
	*/
	inline InputServerBase& InputServer::GetBase()
	{
		return *m_base;
	}
}

#endif // D_SERIALIZABLE_H_