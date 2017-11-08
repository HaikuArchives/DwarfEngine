
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

bool test(const uint8 a) {return a == 0; }


class MyView : public View
{
public:
	MyView():
		View(Rect(0,0,1,1), View::KEEP_PROPORTIONS, View::SPACE_2D_ABSOLUTE),
		m_time_to_spawn(.2)
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
		m_mouse_pos = Vertex<3>(point.x(), point.y());
	}

	virtual void Tick(float frame_time)
	{
		for (ListArray<particle>::Iterator i = m_particles.Begin(); i.IsValid(); i++)
		{
			// a list of all collisions the line make, this list will be filled by the Bounce() method
			ListArray<Vertex<3> > collisions;
			
			// the direction vector if the ray at the end (after all collisions), will be filled by the Bounce() method
			Vector direction;
			
			particle &p = *i;

			if (m_tree.Bounce(p.pos, p.pos + p.direction * p.speed * frame_time, collisions, direction, collide) && collisions.ItemCount() > 0)
			{
				p.pos = *collisions.Last();
				p.direction = direction;
			}
			else
			{
				p.pos = p.pos + p.direction * p.speed * frame_time;
			}

			p.time_left -= frame_time;
			if (p.time_left <= 0) m_particles.RemoveItemAt(i);
		}

		m_time_to_spawn -= frame_time;
		if (m_time_to_spawn <= 0)
		{
			while (m_time_to_spawn <=0 )
			{
				m_time_to_spawn += .01;
				particle p;
				p.pos = m_mouse_pos;
				p.speed = 60;
				float a = (float)rand() / 1000;
				p.direction = Vertex<3>(sin(a), cos(a), 0);
				p.time_left = 5;
			
				m_particles.AddItem(p);
			}
		}
	}

	virtual void Draw(RenderingDevice &device) const
	{
		// set up the renderer for som lines smoothing
		device.SetStates(m_state);
		
		
		// first draw the mesh defined by the points
		device.Begin(RenderingDevice::LINE_LOOP);
			for (int32 i = 0; i < num_points; i++) device.AddVertex(points[i]);
		device.End();


		// then draw the particle
		device.SetColor(Color(1,.8,1));
		device.Begin(RenderingDevice::POINTS);
		for (ListArray<particle>::ConstIterator i = m_particles.Begin(); i.IsValid(); i++)
		{
			const particle &p = *i;
			float a = p.time_left / 5.0f;
			device.SetColor(Color(a,a,a));
			device.AddVertex(p.pos);
		}
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

	struct particle
	{
		Vertex<3> pos;
		Vector direction;
		float speed;
		float time_left;
	};

	ListArray<particle> m_particles;

	Vertex<3> m_mouse_pos;

	float m_time_to_spawn;

};

int32 DwarfMain(Array<String> &commandline)
{
	Window win(Rect(20, 20, 640, 480), TEXT("BSPSample2"));
	
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
