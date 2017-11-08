#ifndef D_INPUTSERVERBASE_H_
#define D_INPUTSERVERBASE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <MessageFilter.h>
#include <View.h>
#include <Point.h>

#include "Defs.h"

namespace dwarf
{
	class InputServer;
	class Window;

	class InputServerBase
	{
	public:
		InputServerBase(InputServer *is);
		~InputServerBase();
		
		void AttachedToWindow(Window *wnd);

		BView *InputView() { return &m_view; }

	protected:
		
		InputServer *m_is;
		Window *m_wnd;
	private:
		// internal class used to receive input
		class BeInputView : public BView
		{
		public:
			BeInputView(InputServer *server);

			virtual void MouseMoved(BPoint point, uint32, const BMessage *);
			virtual void MouseUp(BPoint point);
			virtual void MouseDown(BPoint point);
			virtual void KeyDown(const char *bytes, int32 numBytes);
			virtual void KeyUp(const char *bytes, int32 numBytes);
			
			virtual	void MessageReceived(BMessage *msg);
			virtual void AttachedToWindow();
			virtual void MakeFocus(bool focus = true);

			Point MousePosition();

			void SetWindow(dwarf::Window *win);

		private:
			
			dwarf::Window *m_parent;
			InputServer *m_server;
			BPoint m_mouse_pos;
			BPoint m_orig_mouse_pos;

			uint32 m_mouse_buttons;
			bool m_focus; // true when the view is the focus
		};

		BeInputView m_view;
	};
}

#endif // D_INPUTSERVERBASE_H_