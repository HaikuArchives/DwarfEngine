#ifndef D_SOFTWARE_DEVICE_H_
#define D_SOFTWARE_DEVICE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <GLView.h>
#include <stdlib.h>
#include <new>

#include "RenderingDevice.h"
#include "Vertex.h"
#include "Rect.h"
#include "Window.h"
#include "State.h"
#include "SoftwareWindow.h"

namespace dwarf
{

	/*!
		\todo in beos, the window that is opened do not like Paul Nettle's memory manager, and crashes when closed.
	*/
	class SoftwareDevice : public RenderingDevice
	{
	public:
	
		SoftwareDevice(Window *window, const Rect &frame, uint32 flags, uint32 options, BView *view);
		virtual ~SoftwareDevice();
		virtual void Lock(); // the device must be locked before drawing
		virtual void Unlock(); // and unlocked when finished
		virtual void SwapBuffers();
		virtual WindowBase *RenderWindow();

		virtual void SetFullScreen(bool fullscreen, int32 width, int32 height);
		virtual bool IsFullScreen();
		virtual bool CanSetFullScreen();

		virtual void SetDepthFunc(TestFunction func);
		virtual void SetCullMode(Faces mode);

		virtual void Viewport(const Rect &rect);
		virtual void Scissor(const Rect &rect);

	// view-matrix operations
		virtual void LoadIdentity();
		virtual void Translate(const Vertex<3> &point);
		virtual void Rotate(const Quaternion &rotation);
		virtual void Scale(const Vertex<3> &factor);

		virtual void MultMatrix(const Matrix4x4 &matrix);


		virtual void PushMatrix();
		virtual void PopMatrix();

	// perspective - matrix operations
		virtual void Set2DMode(float width,float height);
		virtual void Set2DMode(const Rect &rect);
		virtual void Set3DMode(float vfov, float hfov, float near_clip, float far_clip);
		virtual void SetPerspective(float fov, float aspect, float near_clip, float far_clip);

		virtual void Draw(HMesh mesh);
		virtual void Draw(const VertexArray &array);

		virtual void Begin(Primitive primitive);
		virtual void End();

		virtual void AddVertex(const Point &vertex);
		virtual void AddVertex(const Vertex<3> &vertex);

		// vertex light operations
		void CreateLight(int32 index, Vertex<3> const& pos, Color const& color);
		void DisableLight(int32 index);

// texture operations
		virtual HTexture NullTexture(); // returns the "none" texture
		virtual int32 TexUnitCount();
		virtual void UseTexture(int32 texture_unit, HTexture texture); // sets the given texture, uploading it if it isn't on the card
		virtual void UploadTexture(HTexture texture); // uploads the given texture to the card
		virtual void RemoveTexture(HTexture texture); // removes the given texture from the card
		void SetTextureFilters(HTexture texture);
		void SetMaterialState(HMaterial material);

		void SetLayerState(const Shader::Layer &layer, int32 flags);
			
		virtual void ResetTexture();
		virtual void TranslateTexture(const Vertex<3> &vertex);
		virtual void RotateTexture(const Quaternion &rotation);

	// state changes

		virtual void SetDepthRange(float min, float max);	


		virtual void TexCoord(const Point &vertex);
		virtual void SetColor(const Color &color);

		virtual void SetColorMask(bool red, bool green, bool blue, bool alpha);

		virtual void ClipAgainst(const Plane<3> &plane);
		virtual void SetPolygonMode(Fillmode f);

		virtual void EnableState(RenderState state);
		virtual void DisableState(RenderState state);

		virtual void SetBlendMode(Blendmode src, Blendmode dst);

		virtual Matrix4x4 GetTextureMatrix() const;
		virtual void SetStates(const State &state);
		virtual const State &CurrentStates() const;

		virtual void ClearDepthBuffer();
		virtual void ClearColorBuffer();
		virtual void SetClearColor(const Color &clr);

		virtual void SetClipPlane(const Plane<3> &plane);

	private:

		struct rgba_color
		{
			const rgba_color &operator=(const Color &col)
			{
				r = col.r * 255.0f;
				g = col.g * 255.0f;
				b = col.b * 255.0f;
				a = col.a * 255.0f;
				return *this;
			}
			uint8 r, g, b, a;
		};

		Matrix4x4 &ModelMatrix() { return m_model_matrix[m_model_stack_ptr]; }

		void SetShadeModel(RenderingDevice::ShadeModel model);

		State m_current_state;
		Timer m_timer;
		float m_time;

		// if we are building in debug mode, we define these variables that
		// makes sure you don't do anything illegal (like accessing a locked rendering device)
		#ifdef _DEBUG
		bool m_locked;
		bool m_in_begin;
		#endif
		
		HTexture m_null_texture;
		
		Matrix4x4 m_texture_matrix;
		bool m_identity_texture;

		Matrix4x4 m_model_matrix[22];
		int32 m_model_stack_ptr;
		bool m_identity_model;

		rgba_color m_clear_color;
		
		Rect m_viewport;
		
		ListArray<Bitmap *> m_textures;
		rgba_color *m_current_texture;
		int32 m_current_texture_width;
		int32 m_current_texture_height;

		float m_line_width;

		SoftwareWindow m_window;

		Vertex<2> bias;
		Vertex<2> scale;
		
		int32 m_clip_left;
		int32 m_clip_top;
		int32 m_clip_right;
		int32 m_clip_bottom;
		
		float m_near_z;
		float m_far_z;
	
		// since we only can draw triangles, we just need a 3 vertex-buffer
		Vertex<3> m_vertices[3];
		Vertex<2> m_tex_coord[3];
		rgba_color m_color[3];
		int32 m_current_vertex;
		
		rgba_color *m_buffer;
		int32 m_width;
		int32 m_height;

		void HorizLineTextured(rgba_color *line_pointer,int32 linestart,int32 lineend,rgba_color *texture,int32 u,int32 v,int32 add_u,int32 add_v);
		void HorizLineFlat(rgba_color *line_pointer,int32 linestart,int32 lineend,rgba_color color);
		void DrawCurrentFace();
		void DrawCurrentFace_();


	};



/*

0							linestart					lineend
|							|							|
V							V							V
..................................OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO........................
line_pointer

*/

	inline void SoftwareDevice::HorizLineTextured(rgba_color *line_pointer,int32 linestart,int32 lineend,rgba_color *texture,int32 u,int32 v,int32 add_u,int32 add_v)
	{
		if (linestart < m_clip_left)
		{
			u += (m_clip_left - linestart) * add_u;
			v += (m_clip_left - linestart) * add_v;
			linestart = m_clip_left;
		}
		if (lineend > m_clip_right+1) lineend = m_clip_right+1;
	
		rgba_color *run_pointer = line_pointer + linestart;
		rgba_color *stop_pointer = line_pointer + lineend;
	
		while (run_pointer < stop_pointer)
		{
			*run_pointer = m_current_texture[(((v>>16)&m_current_texture_height)<<9) + ((u>>16)&m_current_texture_width)];
			run_pointer++;
			u += add_u;
			v += add_v;
		}
	}

	inline void SoftwareDevice::HorizLineFlat(rgba_color *line_pointer,int32 linestart,int32 lineend,rgba_color color)
	{
		lineend++;
		if (linestart < m_clip_left) linestart = m_clip_left;
		if (lineend > m_clip_right+1) lineend = m_clip_right+1;
	//	if (lineend > m_clip_right) lineend = m_clip_right;
	
	//	fprintf(stderr,"start: %ld end: %ld\n",linestart,lineend);
		rgba_color *run_pointer = line_pointer + linestart;
		rgba_color *stop_pointer = line_pointer + lineend;
	
		int lim3 = stop_pointer - run_pointer - 1;
		
		if (lim3 > 0)
			while (lim3--)
			{
				*run_pointer = color;
				run_pointer++;
			}
	}


} // dwarf

#endif // D_SOFTWARE_DEVICE_H_
