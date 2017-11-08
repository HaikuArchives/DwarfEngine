#ifndef D_WINDOWBASE_H_
#define D_WINDOWBASE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "List.h"
#include "Vertex.h"

namespace dwarf
{

	class InputReceiver;


	class Rect;
	class Face;
	class InputReceiver;
	class View;
	class InputServer;

	class WindowBase
	{
	public:

		virtual void SetInputServer(InputServer *server) = 0;

		virtual void SetTitle(const String &title) = 0;
		virtual const String &Title() = 0;

		virtual void SetMousePosition(const Point &position) = 0;
		virtual void Tick() = 0;

		virtual void MoveTo(const Point &position) = 0;
		virtual Point Position() = 0;
		virtual void Resize(float width, float height) = 0;

		virtual int32 Width() = 0;
		virtual int32 Height() = 0;

		virtual void Show(bool visible = true) = 0;
		virtual bool IsVisible() = 0;
		virtual void Minimize(bool minimize) = 0;

		virtual void ShowCursor(bool show) = 0;
		virtual bool IsCursorVisible() = 0;

		virtual void SetRelativeMouseMode(bool mode) = 0;
	};


} // dwarf

#endif // D_WINDOWBASE_H_
