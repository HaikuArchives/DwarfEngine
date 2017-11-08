#ifndef D_WIDGET_H_
#define D_WIDGET_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "View.h"
#include "String.h"

namespace dwarf
{

		/*!
			There will have to be some kind of UI topnode object, so that UI's can
			receive clicks and doubleclicks...
			\brief Base class for GUI stuff. Is a view (but a view is not a widget).
		*/

		class Widget : public View // actually, View should inherit Widget...
		{
		public:

			// user interaction

			virtual void OnClick		(const Point& position) {}
			virtual void OnDoubleClick	(const Point& position) {}

			virtual void OnResize		(const Rect& parent_size) {}
			virtual void OnMove			(const Point& parent_pos) {}
			
			virtual void OnClose		() {}
			
			// drag and drop stuff

			// override this one if you are making a unique type that you
			// want other items to be able to identify.
			// It should contain a ':'-separated list of interfaces your
			// widget supports.
			virtual String ID() { return TEXT("Widget"); }

			virtual bool IsDropable() { return false; }

			virtual void OnDrop(const DropArea& target) {}

		};


		// DRAG AND DROP

		// Drag and drop should be included into the interface by default.
		// Here's my idea:
		// Everything is drag-and-droppable by default, but the drag-and-drop
		// (henceforth DND) is disabled. You will have to explicitly enable
		// DND where you want it via the method MakeDragable.
		
		// Something you can drop things unto is another matter.. Here you'll have
		// to inherit the interface DropArea, and implement the method OnDrop which
		// is called when something is released on top of the area.

		class DropArea
		{
		public:
			virtual void OnDrop(Widget& item) = 0;
		};
}

#endif // D_WIDGET_H_