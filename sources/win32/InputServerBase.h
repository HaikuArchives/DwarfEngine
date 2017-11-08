#ifndef D_INPUTSERVERBASE_H_
#define D_INPUTSERVERBASE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

namespace dwarf
{
	class InputServer;

	/*
		\brief Plattform dependent implementation of InputServer (win32 version).
	*/
	class InputServerBase
	{
	public:
		InputServerBase(InputServer *is);
		~InputServerBase();

		void AttachedToWindow(Window *wnd);

		int HandleMessage(WindowMessage wm);

	protected:


		InputServer *m_is;
		Window *m_wnd;

	};
}

#endif // D_INPUTSERVERBASE_H_