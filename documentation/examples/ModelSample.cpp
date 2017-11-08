 /*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <iostream.h>

#include "Dwarf.h"
#include "ClearView.h"
#include "DrawFPS.h"
#include "InputHandlers.h"
#include "Directory.h"
#include "File.h"
#include "Shader.h"
#include "Shader/ShaderParser.h"

using namespace dwarf;

class MeshDrawer : public View
{
	public:
		MeshDrawer(
				Rect const& frame, HMesh mesh,
				Vertex<3> const& position,
				Vertex<3> const& rot_axis,
				bool rotate = true):
			View(frame, View::KEEP_PROPORTIONS, View::SPACE_3D),
			m_mesh(mesh),
			m_time(0),
			m_position(position),
			m_rotate(rotate),
			m_axis(rot_axis)
		{
/*
			Directory dir(TEXT("../data"));

			HShader shader(new Shader());

			Shader::Layer reflection(HTexture(&dir, TEXT("env.jpg")));
			reflection.m_uvgen = Shader::Layer::ENVMAP;
			reflection.SetColor(Color(1,1,1,.2));
			reflection.m_blend_src = RenderingDevice::SRC_ALPHA;
			reflection.m_blend_dst = RenderingDevice::ONE_MINUS_SRC_ALPHA;
			reflection.m_lighting = false;
			reflection.m_cull_faces = RenderingDevice::BACK;
			shader->AddLayer(reflection);
			m_mesh->SetShader(shader);
*/

			m_mesh->SetShader(HShader(TEXT("bouncy")));
		}

		virtual ~MeshDrawer()
		{
		}

		virtual void Tick(float frame_time)
		{
			m_time += frame_time;
			View::Tick(frame_time);
		}

		virtual void Draw(RenderingDevice &device) const
		{
			DrawModel(device, true);
		}

		void DrawModel(RenderingDevice &device, bool rec) const
		{
			device.PushMatrix();
			
			device.Translate(m_position);
			if (m_rotate) device.Rotate(Quaternion(m_time, m_axis));
			device.Draw(m_mesh);
		
			device.PopMatrix();
		}

	private:
		HMesh m_mesh;

		float m_time;
		bool m_rotate;
		Vertex<3> m_position;
		Vertex<3> m_axis;
};

int32 DwarfMain(Array<String>& cmdLine)
{
	Directory data(TEXT("../data"));

	Shader::ShaderParser()->AddFile(&data, TEXT("shaders/test.shd"));
	Shader::ShaderParser()->AddFile(&data, TEXT("shaders/test2.shd"));

	Texture::SetFileSystem(&data);

	// create the window
	Window window(Rect(0,0,640,480), TEXT("model sample"), Window::CENTERED);
	InputServer input(&window);

	// create a clear view and add it to the window.
	// this view clears the entire window every frame, so
	// it's important to add this view first
	ClearView clear_view(window.Bounds(), Color(0,0,0));
	window.AddChild(&clear_view);

	// create a MeshDrawer view and add it to the window.
	// This view will draw the actual model
	MeshDrawer model_view(
		window.Bounds(), 
		HMesh(new Mesh(&data, TEXT("tris.md2"))),
		Vertex<3>(0, 0, -25),
//		Vertex<3>(.2, 1.0, .7),
		Vertex<3>(0,1.0,0),
		true);
	window.AddChild(&model_view);


	// create the fps drawer view. Draws the fps each frame.
	DrawFPS fps(Rect(10,12,50,50),HTexture(&data, TEXT("text2.tga")), Point(9,16));
	fps.SetColor(Color(1,1,1));
	window.AddChild(&fps);

	window.LimitFPS(60);

	// show and run the window
	window.Show();
	window.Run();

	return 0;
}