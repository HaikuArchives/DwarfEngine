
/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Dwarf.h"
#include "TCBSpline.h"
#include "BSPNode.h"
#include "List.h"

using namespace dwarf;

/*
// the points
const Vertex<3> points[] =
{
	Vertex<3>(.340,.9, 0),
	Vertex<3>(.100,-.9, 0)
};

const int32 num_points = 2;
*/


// the points
const Vertex<3> points[] =
{
	Vertex<3>(250,300, 0),
	Vertex<3>(250,230, 0),
	Vertex<3>(270,270, 0),
	Vertex<3>(330,230, 0),
	Vertex<3>(300,200, 0),
	Vertex<3>(300,170, 0),
	Vertex<3>(350,200, 0),
	Vertex<3>(350,150, 0),
	Vertex<3>(400,150, 0),
	Vertex<3>(400,300, 0),
};

const int32 num_points = 10;


bool collide(const bool &leaf) { return leaf; };


class MyView : public View
{
public:
	MyView():
		View(Rect(0,0,1,1), View::KEEP_PROPORTIONS, View::SPACE_2D_ABSOLUTE),
		m_start(Vertex<3>(250,60,0))
	{
		// build the bsp-tree by adding polygons to it.
		std::list<dwarf::Polygon> poly_list;
		for (int32 i = 0; i < num_points; i++)
		{
			int j = (i + 1) % num_points;
			dwarf::Polygon polygon;
			polygon.AddVertex(points[i]);
			polygon.AddVertex(points[j]);
			polygon.AddVertex((points[i] + points[j]) / 2 + Vertex<3>(0,0, 100));
			poly_list.push_back(polygon);
		}
		
		m_tree.BuildFrom(poly_list, false, true);
		logger(TEXT("depth: %d"), m_tree.Depth());
		logger(TEXT("num nodex: %d"), m_tree.NodeCount());
		
		m_tree.Print();

		Buffer buffer;

		((OutStream&)buffer) << m_tree;

		m_tree = BSPNode<bool>();

		m_tree.Print();

		logger(TEXT("buffer size: %d"), buffer.Size());

		buffer.Rewind();
		((InStream&)buffer) >> m_tree;
		m_tree.Print();
		

		// initialize our state object
		m_state.Enable(State::LINE_SMOOTH + State::BLEND);
		m_state.Disable(State::DEPTH_TEST);
		m_state.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
		m_state.SetColor(Color(1,0,0));
	}



	virtual void MouseMove(const Point &point)
	{
		m_laser.Empty();

		m_stop = Vertex<3>(point.x(), point.y(), 0);

		// a list of all collisions the line make, this list will be filled by the Bounce() method
		ListArray<Vertex<3> > collisions;

		// the direction vector if the ray at the end (after all collisions), will be filled by the Bounce() method
		Vertex<3> direction;

		// Bounce() will cast the ray into the bsp-tree, if the ray hits a solid leaf, it will bounce (the direction will
		// be mirrored around the plane that was hit) off the wall and continue. If the ray hits anouther wall, it will
		// bounce again. All the points where the ray bounced can be found in the collisions list (which is filled by
		// the Bounce() method) the last entry in the collisions list is the end position of the ray.
		// collide is a function that can tell if a leaf i solid or if it's just air, in out case a leaf is represented
		// by a bool (the template parameter) and the collide function is pretty simple.
		if (m_tree.Bounce(m_start, m_stop, collisions, direction, collide))
		{
			// add each line to the lasers list
			Vertex<3> last = m_start;
			for (ListArray<Vertex<3> >::Iterator i = collisions.Begin(); i.IsValid() ;i++)
			{
				laser shot;
				shot.start = last;
				shot.stop = *i;
				last = *i;
				m_laser.AddItem(shot);
			}
		}
		else
		{
			// if Bounce() returns false, it means that the ray didn't hit anything, then we just build the
			// laser beam as is and add it
			laser shot;
			shot.start = m_start;
			shot.stop = m_stop;
			m_laser.AddItem(shot);
		}
	}


	
	virtual void MouseDown(const Point &point, uint8 button, uint32)
	{
		// change the start position of the ray
		m_start = Vertex<3>(point.x(), point.y(), 0);
	}



	virtual void Draw(RenderingDevice &device) const
	{
		// set up the renderer for som lines smoothing
		device.SetStates(m_state);
		
		
		// draw the mesh defined by the points
		device.Begin(RenderingDevice::LINE_LOOP);
			for (int32 i = 0; i < num_points; i++) device.AddVertex(points[i]);
		device.End();


		// then draw the laser rays
		device.SetColor(Color(1,.6,1));
		device.Begin(RenderingDevice::LINES);
		for (ListArray<laser>::ConstIterator i = m_laser.Begin(); i.IsValid(); i++)
		{
			device.AddVertex((*i).start);
			device.AddVertex((*i).stop);
		}
		
		// draw the original ray
		device.SetColor(Color(1,.4, .6));
		device.AddVertex(m_start);
		device.AddVertex(m_stop);

		device.End();
	}

	virtual void OnWindowAdd(RenderingDevice &device)
	{
		// make sure we fill the entire window
		ResizeTo(Win()->Bounds());
	}
				

private:

	State m_state;
	BSPNode<bool> m_tree;

	struct laser
	{
		Vertex<3> start;
		Vertex<3> stop;
	};

	ListArray<laser> m_laser;
	Vertex<3> m_start;
	Vertex<3> m_stop;

};

int32 DwarfMain(Array<String> &commandline)
{
	Window win(Rect(20, 20, 640, 480), TEXT("BSPSample"));
	
	// clear the window with the color black
	ClearView clear(win.Bounds(), Color(0, 0, 0));

	// draws the actual spline
	MyView view;
	InputServer input(&win);
	input.Add(&view);

	// add the views to the window, remember that
	// the order in which you add views matters!
	win.AddChild(&clear);
	win.AddChild(&view);
	
	win.Show();
	win.Run();
	return 0;
}
