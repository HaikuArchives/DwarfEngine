/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <list>
#include <queue>

#include "Dwarf.h"
#include "ClearView.h"
#include "DrawFPS.h"
#include "InputHandlers.h"
#include "Scene.h"
#include "Entity.h"
#include "Sector.h"
#include "Quaternion.h"
#include "Shader/ShaderParser.h"
#include "Euler.h"
#include "Portal.h"
#include "CameraView.h"
#include "QuakeSector.h"
#include "StringView.h"
#include "MirrorPortal.h"
#include "GenericPortal.h"
#include "VertexArray.h"

using namespace dwarf;


// a simple sector that only draws a cubic room
// and a portal. It moves the portal's target in
// it's Tick() method. The portal's target is the
// position (and rotation) where the portal looks.
class TestSector : public Sector
{
public:
	TestSector(Scene &scene, HTexture texture):
		m_texture(texture),
		Sector(scene),
		m_time(0.0f)
	{
		// build a portal
		ListArray<Portal::poly_vertex> poly;
		poly.AddItem(Portal::poly_vertex(Vertex<3>(-40, 40, 0), Point(0, 1)));
		poly.AddItem(Portal::poly_vertex(Vertex<3>(40, 40, 0), Point(1, 1)));
		poly.AddItem(Portal::poly_vertex(Vertex<3>(40, -40, 0), Point(1, 0)));
		poly.AddItem(Portal::poly_vertex(Vertex<3>(-40, -40, 0), Point(0, 0)));
	
		// create a portal that points to our own sector
		m_portal = GenericPortal(Object(Vertex<3>(0,0,100), Quaternion(Euler(0,D_PI*0.2,0)), this), poly);
		
		m_portal.SetShader(HShader(TEXT("teleport")));
	}

	void Tick(float frame_time)
	{
		m_time += frame_time;
		m_portal.SetTarget(Object(
//			Vertex<3>(0, 0, 30),
			Vertex<3>(0,0,100 + sin(m_time)*30.0f),
			Quaternion(Euler(0,D_PI*sin(m_time)*0.1,D_PI*cos(m_time/2)*0.1)),
//			Quaternion(Euler(0, 0, D_PI * sin(m_time))),
			this));
	}

	void DrawSector(RenderingDevice &device, const Camera &camera, int32 depth) const
	{
		m_portal.Draw(device, camera, depth);
		Sector::DrawSector(device, camera, depth);

		device.SetDepthFunc(RenderingDevice::LESS);
		device.EnableState(RenderingDevice::TEXTURE_2D);
		device.DisableState(RenderingDevice::BLEND);
		device.EnableState(RenderingDevice::DEPTH_TEST);
		device.DisableState(RenderingDevice::CULL_FACE);

		device.UseTexture(0, m_texture);

		// far
		device.SetColor(Color(0,1,0));
		device.Begin(RenderingDevice::QUADS);
			device.TexCoord(Point(0,1));
			device.AddVertex(Vertex<3>(-150,150,-150));
			device.TexCoord(Point(1,1));
			device.AddVertex(Vertex<3>(150,150,-150));
			device.TexCoord(Point(1,0));
			device.AddVertex(Vertex<3>(150,-150,-150));
			device.TexCoord(Point(0,0));
			device.AddVertex(Vertex<3>(-150,-150,-150));
		device.End();

		// near
		device.SetColor(Color(1,0,0));
		device.Begin(RenderingDevice::QUADS);
			device.TexCoord(Point(0,1));
			device.AddVertex(Vertex<3>(-150,150,150));
			device.TexCoord(Point(1,1));
			device.AddVertex(Vertex<3>(150,150,150));
			device.TexCoord(Point(1,0));
			device.AddVertex(Vertex<3>(150,-150,150));
			device.TexCoord(Point(0,0));
			device.AddVertex(Vertex<3>(-150,-150,150));
		device.End();

		// top
		device.SetColor(Color(1,1,0));
		device.Begin(RenderingDevice::QUADS);
			device.TexCoord(Point(0,1));
			device.AddVertex(Vertex<3>(-150,150,150));
			device.TexCoord(Point(1,1));
			device.AddVertex(Vertex<3>(150,150,150));
			device.TexCoord(Point(1,0));
			device.AddVertex(Vertex<3>(150,150,-150));
			device.TexCoord(Point(0,0));
			device.AddVertex(Vertex<3>(-150,150,-150));
		device.End();

		// bottom
		device.SetColor(Color(0,0,1));
		device.Begin(RenderingDevice::QUADS);
			device.TexCoord(Point(0,1));
			device.AddVertex(Vertex<3>(-150,-150,150));
			device.TexCoord(Point(1,1));
			device.AddVertex(Vertex<3>(150,-150,150));
			device.TexCoord(Point(1,0));
			device.AddVertex(Vertex<3>(150,-150,-150));
			device.TexCoord(Point(0,0));
			device.AddVertex(Vertex<3>(-150,-150,-150));
		device.End();

		// right
		device.SetColor(Color(0,1,1));
		device.Begin(RenderingDevice::QUADS);
			device.TexCoord(Point(0,1));
			device.AddVertex(Vertex<3>(150,-150,150));
			device.TexCoord(Point(1,1));
			device.AddVertex(Vertex<3>(150,150,150));
			device.TexCoord(Point(1,0));
			device.AddVertex(Vertex<3>(150,150,-150));
			device.TexCoord(Point(0,0));
			device.AddVertex(Vertex<3>(150,-150,-150));
		device.End();

		// left
		device.SetColor(Color(1,0,1));
		device.Begin(RenderingDevice::QUADS);
			device.TexCoord(Point(0,1));
			device.AddVertex(Vertex<3>(-150,-150,150));
			device.TexCoord(Point(1,1));
			device.AddVertex(Vertex<3>(-150,150,150));
			device.TexCoord(Point(1,0));
			device.AddVertex(Vertex<3>(-150,150,-150));
			device.TexCoord(Point(0,0));
			device.AddVertex(Vertex<3>(-150,-150,-150));
		device.End();

	}

private:

	HTexture m_texture;
	GenericPortal m_portal;
	float m_time;

};
















/*
// this view draws the scene through the eyes of the camera.
// it also listens for input and moves the camera accordingly.
class MyView : public CameraView
{

public:

	MyView(const Rect &frame, int32 resize_mode, Camera &camera, Window &window, TextOutStream &stream):
		CameraView(frame, resize_mode, camera),
		m_forward(false),
		m_backwards(false),
		m_left(false),
		m_right(false),
		m_camera(camera),
		m_window(window),
		m_stream(stream)
	{}

	virtual void KeyDown(dchar character, uint32 scancode, uint32 modifiers)
	{
		// toggle relative mouse mode with escape
		if (scancode == InputReceiver::SC_ESCAPE)
			m_window.GetInputServer()->SetRelativeMouseMode(!m_window.GetInputServer()->RelativeMouseMode());

		switch(character)
		{
		case CHAR('w'):
		case CHAR('W'):
			m_forward = true;
			break;
		case CHAR('s'):
		case CHAR('S'):
			m_backwards = true;
			break;
		case CHAR('a'):
		case CHAR('A'):
			m_left = true;
			break;
		case CHAR('d'):
		case CHAR('D'):
			m_right = true;
			break;
		default:
			CameraView::KeyDown(character, scancode, modifiers);
		}
	}

	virtual void KeyUp(dchar character, uint32 scancode, uint32 modifiers)
	{
		switch(character)
		{
		case CHAR('w'):
		case CHAR('W'):
			m_forward = false;
			break;
		case CHAR('s'):
		case CHAR('S'):
			m_backwards = false;
			break;
		case CHAR('a'):
		case CHAR('A'):
			m_left = false;
			break;
		case CHAR('d'):
		case CHAR('D'):
			m_right = false;
			break;
		default:
			CameraView::KeyDown(character, scancode, modifiers);
		}
	}


	virtual void RelativeMouseMove(const Point &position)
	{
		m_stream << TEXT("(") << position.x() << TEXT(", ") << position.y() << TEXT(")\n");
		m_rotation.heading += position.x() * -.003;
		m_rotation.pitch += position.y() * .003;

		if (m_rotation.pitch > D_PI *.5) m_rotation.pitch = D_PI * .5;
		else if (m_rotation.pitch < D_PI *-.5) m_rotation.pitch = D_PI * -.5;
		
		m_camera.RotateTo(Quaternion(m_rotation));
	}
	
	virtual void Tick(float frame_time)
	{
		if (m_forward) m_camera.MoveForward(frame_time * 100.0f);
		if (m_backwards) m_camera.MoveForward(frame_time * -100.0f);
		if (m_right) m_camera.MoveRight(frame_time * 100.0f);
		if (m_left) m_camera.MoveRight(frame_time * -100.0f);
	}
	
private:

	bool m_forward;
	bool m_backwards;
	bool m_left;
	bool m_right;

	Camera &m_camera;
	Window &m_window;

	Euler m_rotation;

	TextOutStream &m_stream;

};
*/

class MyView : public CameraView
{

public:

	MyView(const Rect &frame, int32 resize_mode, Camera &camera, Window &window):
		CameraView(frame, resize_mode, camera),
		m_forward(false),
		m_backwards(false),
		m_left(false),
		m_right(false),
		m_control_portal(false),
		m_camera(camera),
		m_window(window)
	{}

	virtual void KeyDown(dchar character, uint32 scancode, uint32 modifiers)
	{
		// toggle relative mouse mode with escape
		if (scancode == InputReceiver::SC_ESCAPE)
			m_window.GetInputServer()->SetRelativeMouseMode(!m_window.GetInputServer()->RelativeMouseMode());

		switch(character)
		{
		case CHAR(' '):
			m_control_portal = !m_control_portal;
			break;
		case CHAR('w'):
		case CHAR('W'):
			m_forward = true;
			break;
		case CHAR('s'):
		case CHAR('S'):
			m_backwards = true;
			break;
		case CHAR('a'):
		case CHAR('A'):
			m_left = true;
			break;
		case CHAR('d'):
		case CHAR('D'):
			m_right = true;
			break;
		default:
			CameraView::KeyDown(character, scancode, modifiers);
		}
	}

	virtual void KeyUp(dchar character, uint32 scancode, uint32 modifiers)
	{
		switch(character)
		{
		case CHAR('w'):
		case CHAR('W'):
			m_forward = false;
			break;
		case CHAR('s'):
		case CHAR('S'):
			m_backwards = false;
			break;
		case CHAR('a'):
		case CHAR('A'):
			m_left = false;
			break;
		case CHAR('d'):
		case CHAR('D'):
			m_right = false;
			break;
		default:
			CameraView::KeyDown(character, scancode, modifiers);
		}
	}


	virtual void RelativeMouseMove(const Point &position)
	{
		if (m_control_portal)
		{
			m_portal_rotation.heading += position.x() * -.003;
			m_portal_rotation.pitch += position.y() * .003;
			
			if (m_portal_rotation.pitch > D_PI *.5) m_portal_rotation.pitch = D_PI * .5;
			else if (m_portal_rotation.pitch < D_PI *-.5) m_portal_rotation.pitch = D_PI * -.5;


			GenericPortal *portal = static_cast<GenericPortal*>(m_camera.GetSector()->Portals().front());
			portal->Target().RotateTo(Quaternion(m_portal_rotation));
			portal->Update();

		}
		else
		{
			m_rotation.heading += position.x() * -.003;
			m_rotation.pitch += position.y() * .003;

			if (m_rotation.pitch > D_PI *.5) m_rotation.pitch = D_PI * .5;
			else if (m_rotation.pitch < D_PI *-.5) m_rotation.pitch = D_PI * -.5;
		
			m_camera.RotateTo(Quaternion(m_rotation));
		}
	}
	
	virtual void Tick(float frame_time)
	{
		if (m_control_portal)
		{

			GenericPortal *portal = static_cast<GenericPortal*>(m_camera.GetSector()->Portals().front());
			if (m_forward) portal->Target().MoveForward(frame_time * 100.0f);
			if (m_backwards) portal->Target().MoveForward(frame_time * -100.0f);
			if (m_right) portal->Target().MoveRight(frame_time * 100.0f);
			if (m_left) portal->Target().MoveRight(frame_time * -100.0f);
			portal->Update();
		}
		else
		{
			if (m_forward) m_camera.MoveForward(frame_time * 100.0f);
			if (m_backwards) m_camera.MoveForward(frame_time * -100.0f);
			if (m_right) m_camera.MoveRight(frame_time * 100.0f);
			if (m_left) m_camera.MoveRight(frame_time * -100.0f);
		}
	}
	
private:

	bool m_forward;
	bool m_backwards;
	bool m_left;
	bool m_right;

	bool m_control_portal;

	Camera &m_camera;
	Window &m_window;

	Euler m_rotation;
	Euler m_portal_rotation;
};






















// *** THIS IS THE MOST TEMPORARY THING IN THE ENTIRE DWARF PROJECT ! ***

class Ticker : public View
{
public:
	Ticker(Scene &scene):
		View(Rect(0,0,1,1)),
		m_scene(scene)
	{}

	void Tick(float frame_time)
	{
		m_scene.Tick(frame_time);
	}
private:
	Scene &m_scene;
};



class FPSView : public StringView
{
public:
	FPSView(Rect rect, const Font &font, const Color &color = Color(1,1,1,1), uint32 resize_mode = View::FOLLOW_BOTTOM_LEFT):
		StringView(rect,font,15, color, resize_mode)
	{
	}

	void Tick(float frame_time)
	{
		String str;

		str.Format(TEXT("%.1f fps"),1.0f/frame_time);

		SetString(str);
	}

};



class GunView : public View
{
public:
	GunView(Rect rect, HMesh mesh):
		View(rect, View::KEEP_PROPORTIONS, View::SPACE_3D),
		m_mesh(mesh)
	{
		m_mesh->SetShader(HShader(TEXT("bouncy")));

		SetClip(.1,150);
	}

	virtual void Draw(RenderingDevice &device) const
	{

		State state;

//		state.Disable(State::DEPTH_TEST);

		device.LoadIdentity();
		device.SetStates(state);
		device.Scale(Vertex<3>(.1, .1, .1));
		device.Translate(Vertex<3>(5,-18,-16));
		device.Draw(m_mesh);
	}

private:
	HMesh m_mesh;
};

class CrossHair : public View
{
public:
	CrossHair(Rect rect):
		View(rect, View::CENTER, SPACE_2D_CENTER)
	{
	}

	virtual void Draw(RenderingDevice &device) const
	{
		device.LoadIdentity();

		State state;

		state.Enable(State::BLEND);
		state.SetBlendMode(RenderingDevice::SRC_ALPHA, RenderingDevice::ONE_MINUS_SRC_ALPHA);
		state.SetColor(Color(1,0,0,.5));

		device.SetStates(state);

		device.Begin(RenderingDevice::LINES);
		device.AddVertex(Point(-1, 0));
		device.AddVertex(Point(1, 0));
		device.AddVertex(Point(0, -1));
		device.AddVertex(Point(0, 1));
		device.End();

/*		device.Begin(RenderingDevice::QUADS);
		device.AddVertex(Point(-1,-1));
		device.AddVertex(Point(1,-1));
		device.AddVertex(Point(1,1));
		device.AddVertex(Point(-1,1));
		device.End();
*/

/*		device.Begin(RenderingDevice::TRIANGLES);
		device.AddVertex(Point(0,1));
		device.AddVertex(Point(-1,-1));
		device.AddVertex(Point(1,-1));
		device.End();
*/
	}
};

int32 DwarfMain(Array<String> &cmdLine)
{
	Directory data(TEXT("../data"));

	Texture::SetFileSystem(&data);

	// create the window
	Window window(Rect(0,0,640,480), TEXT("model sample")/*, Window::FULLSCREEN*/);

	// tell the shader where to find the shader-files
	Shader::ShaderParser()->AddFile(&data, TEXT("shaders/test.shd"));
	Shader::ShaderParser()->AddFile(&data, TEXT("shaders/test2.shd"));
	

	// create a clear view and add it to the window.
	// this view clears the entire window every frame, so
	// it's important to add this view first
	ClearView clear_view(window.Bounds(), Color(0,0,0));

	// first we need a scene to keep track of all entities and sectors
	// (we will of course just use one sector in this sample)
	Scene scene;

	// create an instance of our portal-sector
	TestSector psector(scene, HTexture(&data, TEXT("jesus.jpg")));
	// add the sector to the scene
	scene.AddSector(&psector);

	// create a camera and initialize it
	Camera camera;
	camera.MoveTo(Vertex<3>(0, 0, 100));
	camera.LookAt(Vertex<3>(0, 0, -100));

	// add the camera to the scene and to our sector
	int32 id = scene.AddCamera(&camera);
	psector.AddCamera(id);

	// --------------- quake bsp

	QuakeSector quake_sector(scene,TEXT("maps/dm6.bsp"),data);

	quake_sector.Load(0.0f);
	scene.AddSector(&quake_sector);
	Camera quake_camera;
	quake_camera.MoveTo(Vertex<3>(200, 180, 900));
//	quake_camera.LookAt(Vertex<3>(0, 0, -100));

	// second quake sector

	QuakeSector dm4_sector(scene,TEXT("maps/dm4.bsp"),data);
	quake_sector.AddCamera(scene.AddCamera(&quake_camera));

	dm4_sector.Load(0.0f);
	scene.AddSector(&dm4_sector);

	// =================

	// build a portal
	ListArray<Portal::poly_vertex> poly;

	poly.AddItem(Portal::poly_vertex(Vertex<3>(176, 224, 855), Point(0, 1)));
	poly.AddItem(Portal::poly_vertex(Vertex<3>(224, 224, 855), Point(1, 1)));
	poly.AddItem(Portal::poly_vertex(Vertex<3>(224, 128, 855), Point(1, 0)));
	poly.AddItem(Portal::poly_vertex(Vertex<3>(176, 128, 855), Point(0, 0)));

//	GenericPortal portal(Object(Vertex<3>(0,0,100), Quaternion(), &psector), poly);
	GenericPortal portal(Object(Vertex<3>(384,62,960), Quaternion(), &dm4_sector), poly);
//	MirrorPortal portal(poly);

	portal.SetShader(HShader(TEXT("mirror")));

	quake_sector.AddPortal(&portal);
/*
// ****************************************
	// build another portal
	ListArray<Portal::poly_vertex> poly2;

	poly2.AddItem(Portal::poly_vertex(Vertex<3>(226, 224, 875), Point(0, 1)));
	poly2.AddItem(Portal::poly_vertex(Vertex<3>(274, 224, 875), Point(1, 1)));
	poly2.AddItem(Portal::poly_vertex(Vertex<3>(274, 128, 875), Point(1, 0)));
	poly2.AddItem(Portal::poly_vertex(Vertex<3>(226, 128, 875), Point(0, 0)));
	
	Portal portal2(Object(Vertex<3>(-100,0,100), Quaternion(Euler(0, -.5, 0)), &psector), poly2);
	//	Portal portal(Object(Vertex<3>(384,62,960), Quaternion(), &dm4_sector), poly);
	portal2.SetShader(HShader(TEXT("transparent")));

	quake_sector.AddPortal(portal2);
*/	
// ****************************************
	

	// -------------------------

	// load a font
	Font font(&data, TEXT("fonts/Verdana, Bold Italic.font"));

	// create a view that draws what the camera sees
	MyView camera_view(window.Bounds(), View::FOLLOW_ALL_SIDES, quake_camera, window/*, console*/);


	InputServer input(&window);
	input.Add(&camera_view);

	// create and add a string view with some help-text
	StringView string_view(Rect(0, 0, 50, 50),15, Color(1, .4, .8, 1), View::FOLLOW_BOTTOM_LEFT);
	string_view.SetFont(font);
	string_view.SetString(TEXT("press escape to toggle mouse.\npress space to control portal target.\nmove with wsad-keys."));

	FPSView fps_view(Rect(0, window.Height()-15, 50, 15),font, Color(1,1,1), View::FOLLOW_TOP_LEFT);

	GunView gun_view(window.Bounds(), HMesh(new Mesh(&data, TEXT("weapon.md2"))));

	CrossHair crosshair_view(Rect(0,0,10,10));

	// the order in which views are added is important!
	window.AddChild(&clear_view);
	window.AddChild(&camera_view);
	window.AddChild(&gun_view);
	window.AddChild(&string_view);
	window.AddChild(&fps_view);
	window.AddChild(&crosshair_view);

	// maybe we should make it possible to add a scene to a window
	// so that it is ticked. Now I have to make a dummy view that
	// ticks the scene. (as said earlier, this is a highly temporary solution).
	Ticker dummy(scene);
	window.AddChild(&dummy);

	// show and run the window
	window.Show();
	window.Run();

	return 0;
}