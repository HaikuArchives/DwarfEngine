#include "Dwarf.h"

using namespace dwarf;

class EditorView : public View
{
public:

	struct Triangle
	{
		// the indices of the points in the point list
		int32 point[3];
		Color color;
	};

	struct Node
	{
		Node(const Point &p): point(p), selected(false) {}
		Node() {}
		Point point;
		bool selected;
	};

	EditorView(const Rect &frame,
				ListArray<Triangle> &mesh,
				ListArray<Node> &points,
				ListArray<Node> &sibling_points):
		View(frame, View::KEEP_PROPORTIONS, View::SPACE_2D_RELATIVE),
		m_mesh(mesh),
		m_points(points),
		m_sibling_points(sibling_points),
		m_moving(-1)
	{
		D_ASSERT(m_points.ItemCount() == m_sibling_points.ItemCount());
	}

	virtual void Tick(float frame_time)
	{
		float a = (sin(fmodf(Win()->SystemTimer().Current() * 8.0f, D_2PI)) * .5f) + .5f;
		m_wire_color = Color(a, a, a);

		View::Tick(frame_time);
	}

	virtual void Draw(RenderingDevice &device) const
	{
		D_ASSERT(m_points.ItemCount() == m_sibling_points.ItemCount());

		device.LoadIdentity();

		device.DisableState(RenderingDevice::TEXTURE_2D);
		device.DisableState(RenderingDevice::DEPTH_TEST);

		// antialias lines
		device.EnableState(RenderingDevice::LINE_SMOOTH);

		device.EnableState(RenderingDevice::BLEND);
		device.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);

		// draws the colors of the mesh
		device.Begin(RenderingDevice::TRIANGLES);
		for(int32 i = 0; i < m_mesh.ItemCount(); i++)
		{
			device.SetColor(m_mesh[i].color);
			device.AddVertex(m_points[m_mesh[i].point[0]].point);
			device.AddVertex(m_points[m_mesh[i].point[1]].point);
			device.AddVertex(m_points[m_mesh[i].point[2]].point);
		}
		device.End();


		// draws the mesh as wireframe
		device.SetColor(m_wire_color);
		for(int32 j = 0; j < m_mesh.ItemCount(); j++)
		{
			device.Begin(RenderingDevice::LINE_LOOP);
			device.AddVertex(m_points[m_mesh[j].point[0]].point);
			device.AddVertex(m_points[m_mesh[j].point[1]].point);
			device.AddVertex(m_points[m_mesh[j].point[2]].point);
			device.End();
		}

		// draws the points
		device.Begin(RenderingDevice::QUADS);
		for(int32 k = 4; k < m_points.ItemCount(); k++)
		{
			const Point &point = m_points[k].point;
			if (m_points[k].selected)
				device.SetColor(Color(1, 1, 1, 1));
			else
				device.SetColor(Color(.9, .3, .4, .7));


			device.AddVertex(point + Point(.01, .01));
			device.AddVertex(point + Point(-.01, .01));
			device.AddVertex(point + Point(-.01, -.01));
			device.AddVertex(point + Point(.01, -.01));
		}

		device.AddVertex(m_pos + Point(.01, .01));
		device.AddVertex(m_pos + Point(-.01, .01));
		device.AddVertex(m_pos + Point(-.01, -.01));
		device.AddVertex(m_pos + Point(.01, -.01));

		device.End();
		
		device.DisableState(RenderingDevice::BLEND);
	}

	virtual void OnWindowAdd(RenderingDevice &device)
	{
		// make this view receive input events
		Win()->GetInputServer()->Add(this);
	}

	virtual void MouseDown(const Point &position, uint8 button_index, uint32 modifiers)
	{
		// adjust the mouse position to the views coordinate space
		Point pos = position.ConvertSpace( Frame(), Bounds());

		if (button_index == 0)
		{
			logger(TEXT("Left Mouse Down!\n"));
			// are we trying to move a point?
			// start at 4 because the 4 first points are not movable!
			for(int32 k = 4; k < m_points.ItemCount(); k++)
			{
				const Point &point = m_points[k].point;
				Rect rect(point + Point(-.01, -.01), .2, .2);

				if (rect.Contains(pos))
				{
					// yes we are!
					m_moving = k;
					m_points[k].selected = true;
					m_sibling_points[k].selected = true;
					return;
				}

			}
		}
		else if (button_index == 1)
		{
			logger(TEXT("Right Mouse Down!\n"));
			// are we trying to add a point?		
			for(int32 i = 0; i < m_mesh.ItemCount(); i++)
			{
				const Triangle &tri = m_mesh[i];
				for (int32 j = 0; j < 3; j++)
				{
					if (tri.point[j] < 4 && tri.point[(j+1)%3] < 4) continue;
					Point &p1 = m_points[tri.point[j]].point;
					Point &p2 = m_points[tri.point[(j+1)%3]].point;

					Point normal = p1 - p2;
					normal.RotateBy(D_PI / 2);

					Plane<2> line(p1, normal);

					if (fabs(line.DistanceTo(pos)) < .01)
					{
						if (pos.x() > min(p1.x(), p2.x())
							&& pos.x() < max(p1.x(), p2.x())
							&& pos.y() > min(p1.y(), p2.y())
							&& pos.y() < max(p1.y(), p2.y()))
						{
							// yes we are!
							// now, it takes quite a bit of code to add a
							// point, so I've put it in a subroutine
							AddPoint(tri.point[j%3], tri.point[(j+1)%3]);

							// it's very important to return here, otherwise we
							// will continue to split the new triangles forever
							return;
						}
					}
				}
			}
		}
		logger(TEXT("~MouseDown()\n"));
	}

	void AddPoint(int32 index1, int32 index2)
	{
		Node node;
		node.selected = false;
		node.point = (m_points[index1].point + m_points[index2].point) / 2.0f;

		int32 new_index = m_points.AddItem(node);

		node.point = (m_sibling_points[index1].point + m_sibling_points[index2].point) / 2.0f;
		int32 new_index2 = m_sibling_points.AddItem(node);

		D_ASSERT(new_index == new_index2);

		// for each triangle that contains these points
		// split that triangle
		int32 itemcount = m_mesh.ItemCount();
		for(int32 i = 0; i < itemcount; i++)
		{
			Triangle &tri = m_mesh[i];
			int32 point1 = -1;
			int32 point2 = -1;

			for (int32 j = 0; j < 3; j++)
				if (tri.point[j] == index1) point1 = j;

			for (int32 k = 0; k < 3; k++)
				if (tri.point[k] == index2) point2 = k;

			if (point1 != -1 && point2 != -1)
			{
				// the triangle contain both points!
				// now we just have to split it

				// first, identify the third point in the triangle
				int32 point3 = 0;
				if (point1 == 0 || point2 == 0)
				{
					point3 = 1;
					if (point1 == 1 || point2 == 1) point3 = 2;
				}
				
				Triangle t;
				t.point[0] = new_index;
				t.point[1] = tri.point[point1];
				t.point[2] = tri.point[point3];
				t.color = Color(0,0,0,0);
				m_mesh.AddItem(t);

				tri.point[point1] = new_index;
			}
		}
	}

	virtual void MouseUp(const Point &position, uint8 button_index, uint32 modifiers)
	{
		if (m_moving == -1) return;
		// adjust the mouse position to the views coordinate space
		Point pos = position.ConvertSpace(Frame(), Bounds());
		m_points[m_moving].selected = false;
		m_sibling_points[m_moving].selected = false;
		m_moving = -1;
	}

	virtual void MouseMove(const Point &position)
	{
		// adjust the mouse position to the views coordinate space
		Point pos = position.ConvertSpace(Frame(), Bounds());
		if (m_moving >= 0) m_points[m_moving].point = pos;

		m_pos = pos;
	}

private:

	ListArray<Triangle> &m_mesh;
	ListArray<Node> &m_points;
	ListArray<Node> &m_sibling_points;
	int32 m_moving;
	Color m_wire_color;


	Point m_pos;
};



int32 DwarfMain(Array<String> &commandline)
{
	Window win(Rect(20, 20, 512, 240), TEXT("Window title"), Window::UNRESIZABLE);
	InputServer input;
	win.SetInputServer(&input);

	// create a clear view (that clears the display every frame)
	ClearView clear(win.Bounds(), Color(0, 0, 0));

	// load the 2 images
	Directory dir(TEXT("../data"));
	HTexture from_tex(&dir, TEXT("from.jpg"));
	HTexture to_tex(&dir, TEXT("to.jpg"));

	// create 2 bitmap views that draws the bitmaps
	BitmapView left_bitmap(Rect(0, 0, win.Width()/2.0f, win.Height()), from_tex);
	BitmapView right_bitmap(Rect(win.Width()/2.0f, 0, win.Width()/2.0f, win.Height()), to_tex);

	ListArray<EditorView::Node> from_points(256);
	ListArray<EditorView::Node> to_points(256);
	ListArray<EditorView::Triangle> mesh(256);

	try
	{
		File file(&dir, TEXT("mesh"));

		Buffer buffer;
		file.Read(buffer);

		int32 count;
		// read the number of points
		buffer >> count;
		logger(TEXT("Point: %d\n"), count);

		for (int32 i = 0; i < count; i++)
		{
			EditorView::Node node;
			node.selected = false;

			(InStream&)buffer >> node.point;
			from_points.AddItem(node);
			(InStream&)buffer >> node.point;
			to_points.AddItem(node);
		}

		// read the number of triangles
		buffer >> count;
		logger(TEXT("Triangles: %d\n"), count);

		for (int32 j = 0; j < count; j++)
		{
			EditorView::Triangle tri;
			tri.color = Color(0, 0, 0, 0);
			buffer >> tri.point[0];
			buffer >> tri.point[1];
			buffer >> tri.point[2];
			mesh.AddItem(tri);
		}
	}
	catch(const File::IoError &e) // if the file does not exist, initialize the mesh
	{
		EditorView::Node node;
		node.selected = false;
		
		// initializes the corners, these cannot be removed (the first 4 points)
		from_points.AddItem(Point(0,0));
		from_points.AddItem(Point(1,0));
		from_points.AddItem(Point(1,1));
		from_points.AddItem(Point(0,1));
		from_points.AddItem(Point(.5,.5));

		to_points.AddItem(Point(0,0));
		to_points.AddItem(Point(1,0));
		to_points.AddItem(Point(1,1));
		to_points.AddItem(Point(0,1));
		to_points.AddItem(Point(.5,.5));

		EditorView::Triangle tri;

		tri.point[0] = 0;
		tri.point[1] = 1;
		tri.point[2] = 4;
		tri.color = Color(.3,0,0,.5);
		mesh.AddItem(tri);

		tri.point[0] = 1;
		tri.point[1] = 2;
		tri.point[2] = 4;
		tri.color = Color(0,.4,0,.5);
		mesh.AddItem(tri);

		tri.point[0] = 2;
		tri.point[1] = 3;
		tri.point[2] = 4;
		tri.color = Color(0,0,.7,.5);
		mesh.AddItem(tri);

		tri.point[0] = 3;
		tri.point[1] = 0;
		tri.point[2] = 4;
		tri.color = Color(.5,.5,0,.5);
		mesh.AddItem(tri);

	}

	// create the editor views
	EditorView left_editor(Rect(0, 0, win.Width()/2.0f, win.Height()), mesh, from_points, to_points);
	EditorView right_editor(Rect(win.Width()/2.0f, 0, win.Width()/2.0f, win.Height()), mesh, to_points, from_points);

	// add the views to the window
	win.AddChild(&clear);
	win.AddChild(&left_bitmap);
	win.AddChild(&right_bitmap);

	input.Add(&left_editor);
	input.Add(&right_editor);

	win.AddChild(&left_editor);
	win.AddChild(&right_editor);

	// show and run
	win.Show();
	win.Run();

	// create a buffer and store all data in it
	Buffer buffer;

	buffer << from_points.ItemCount();
	for(int32 i = 0; i < from_points.ItemCount(); i++)
	{
		(OutStream&)buffer << from_points[i].point;
		(OutStream&)buffer << to_points[i].point;
	}

	buffer << mesh.ItemCount();
	for(int32 j = 0; j < mesh.ItemCount(); j++)
	{
		buffer << mesh[j].point[0];
		buffer << mesh[j].point[1];
		buffer << mesh[j].point[2];
	}

	// create a file and save the buffer
	File save_file(&dir, TEXT("mesh"), File::WRITE, File::CREATE);
	save_file.Write(buffer);

	return 0;
}
