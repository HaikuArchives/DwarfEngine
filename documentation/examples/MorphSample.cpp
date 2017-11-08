#include "Dwarf.h"

using namespace dwarf;

int32 DwarfMain(Array<String> &commandline)
{
	Window win(Rect(20, 20, 640, 480), TEXT("morph")/*, Window::UNRESIZABLE*/);

// create views
	ClearView clear(win.Bounds(), Color(0, 0, 0));

	Directory dir(TEXT("../data"));

	HTexture from_tex(&dir, TEXT("bear.png"));
	HTexture to_tex(&dir, TEXT("to.jpg"));

	File file(&dir, TEXT("mesh"));
	Buffer buffer;
	file.Read(buffer);

	int32 count;
	// read the number of points
	buffer >> count;
	logger(TEXT("Point: %d\n"), count);

	Array<Point> from_points(count);
	Array<Point> to_points(count);

	for (int32 i = 0; i < count; i++)
	{
		(InStream&)buffer >> from_points[i];
		(InStream&)buffer >> to_points[i];
	}

	// read the number of triangles
	buffer >> count;
	logger(TEXT("Triangles: %d\n"), count);

	Array<MorphView::Triangle> mesh(count);

	for (int32 j = 0; j < count; j++)
	{
		buffer >> mesh[j].point[0];
		buffer >> mesh[j].point[1];
		buffer >> mesh[j].point[2];
	}

	MorphView morph(win.Bounds(), from_tex, to_tex, mesh, from_points, to_points, 2);

// add views
	win.AddChild(&clear);
	win.AddChild(&morph);

	win.Show();
	win.Run();
	return 0;

}
