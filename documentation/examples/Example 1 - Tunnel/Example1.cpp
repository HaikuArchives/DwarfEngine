/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 *
 *
 * Example 1
 * Intro to Windows & Views
 */

// First of all, we need to include the basic
// DwarfEngine classes by including the file
// "Dwarf.h".
// All this file does is include a bunch of 
// other files that are needed for the engine 
// to work, such as Window.h for the Window class,
// and Vertex.h for the Vertex class.
//
// Then we also include the files DrawFPS.h and
// TunnelView.h. These are what we call support
// classes. They aren't really part of the engine,
// but takes the more general classes of the engine,
// and extends them with more detailed implementations.
//
// For example, the DrawFPS class inherits the engine
// class View, and can thus be used just like a normal
// view. What it does is it draws the current FPS
// in it's drawing area.
#include <Dwarf.h>
#include <ClearView.h>
#include <TunnelView.h>
#include <DrawFPS.h>

// Now we need to include the DwarfEngine classes
// into the global namespace.
// All of the DwarfEngine classes have been put in
// a unique namespace called dwarf to prevent 
// name collisions with other libraries like MFC.
// You could remove these lines, but then you would
// have to refer to all of the DwarfEngine classes
// with a dwarf:: prefix. For example, Window
// would become dwarf::Window.
//
// If you don't know what a namespace is, just ignore
// these lines for now.
using namespace dwarf;

// Next up is the DwarfMain method. This is just like
// main() in any other program, or WinMain if it's a
// Windows program, with the difference that DwarfMain
// takes a StringArray parameter, and also that the engine
// does some initializing and cleaning up before and after
// it is called.
// You yourself does not have to create a main() or WinMain(),
// the engine takes care of that.
// For now we'll ignore the cmdline parameter.
int32 DwarfMain(StringArray &cmdline)
{
	// First of all, we'll create a window.
	// At the moment the window automatically loads the
	// OpenGL renderer, but we are going to change this 
	// so that you'll be able to choose from the available 
	// renderers when constructing the window.
	//
	// The first parameter is simply the boundaries of the window.
	// We want a window that is 640 x 480 in size, so we'll pass on
	// a rect with a width of 640 and a height of 480. Since we give
	// the window the flag CENTER, we can pass anything in the first
	// two slots of the Rect. The window will always be centered on 
	// screen.
	// The second parameter is the window title. If the window isn't
	// fullscreen, you'll be able to see this text at the top of it.
	//
	// The last parameter are the window flags. There are a couple of
	// different flags available, like for example Window::FULLSCREEN
	// if you want to make a fullscreen window. For now, we'll just
	// give it the CENTERED flag so that the window will be centered on
	// screen.
	Window win(Rect(0, 0, 640, 480), TEXT("DwarfEngine 3D - Example 1"), Window::CENTERED);
	
	// Next, we'll create a Directory. The Directory class is simply
	// a front end to a normal directory on disk. By default, it opens
	// the directory the program file lies in, but we'll redirect it to
	// the subdirectory called data. Now we can load files from this 
	// directory using the Directory class.
	Directory directory;
	directory.OpenFileSystem(TEXT("../data"));

	// All a ClearView does is clear the front and back buffers on the
	// card. Thus it clears beyond it's frame and so should always be
	// placed at the end of the draw list.
	ClearView clear(win.Bounds(), Color(0,0,0));

	// Now we'll create the TunnelView.
	// The TunnelView class takes two textures as parameters,
	// and displays a tunnel using those two textures.
	// It's main use is debugging, but it is a good exercise
	// in how the View system works.
	//
	// We load the textures we want at the same time, by 
	// creating temporary Bitmap objects that we pass to the 
	// CreateTexture method of the device.
	TunnelView tunnel(win.Bounds(), View::KEEP_PROPORTIONS);
	tunnel.SetTexture(Texture(Bitmap(&directory, TEXT("tex1.jpg")), win));
	tunnel.SetTransparentTexture(Texture(Bitmap(&directory, TEXT("tex3.jpg")), win));

	// The DrawFPS view is constructed much in the same way,
	// but the parameters it takes are a bit more obscure.
	// First of all, we tell it the size it should have on screen.
	// The text won't resize along with the rest of the window,
	// but will stay put in the lower left corner.
	// Next, we give it the texture to use.
	// Then we tell the view how big each character is in the 
	// texture, and finally that it should follow the lower left corner
	// of the window when it is resized.
	DrawFPS fps(
		Rect(10,12,200,80),		Texture(Bitmap(&directory, TEXT("text2.tga")), win), 
		Point(9, 16),			View::FOLLOW_BOTTOM_LEFT);

	// Now we have to add the two views as children to the Window,
	// so that they will be drawn each frame. The order in which
	// we add them is important. By adding the tunnel first, we
	// make sure it's methods are called before the ones in the fps
	// view, and thus the fps will be drawn on top of the tunnel.
	win.AddChild(&clear);
	win.AddChild(&tunnel);
	win.AddChild(&fps);

	// Finally, we'll make the window visible by calling Show()
	// and then we let the Window take control with the method Run().
	win.Show();
	win.Run();

	return 0;
}
