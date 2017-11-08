#ifndef D_RENDERINGDEVICE_H_
#define D_RENDERINGDEVICE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Array.h"
#include "Texture.h"
#include "Rect.h"
#include "Mesh.h"
#include "Plane.h"

namespace dwarf
{

		class Window;
		class Rect;
		class Object;
		class Face;
		class Quaternion;
		class Color;
		class WindowBase;
		class State;
		class VertexArray;

		/*!
			\brief The base class for a Rendering Device.
			This enables us to support different rendering API's (e.g. OpenGL & DirectX).
			If you're familiar with OpenGL, you will understand most of this API (because it's "based" on OpenGL)
		*/
		class RenderingDevice
		{
			friend class View; // sets 2D or 3D mode, and calls Viewport()
			friend class Texture; // uploads and removes textures
			friend class Window; // calls SwapBuffers(), Lock() and Unlock()
			friend class Camera; // changes fov and near/far-clip when drawn
			friend class Portal; // uses SetClipPlane
			public:

			enum RenderState
			{
				ALPHA_TEST = 0,
				AUTO_NORMAL,
				BLEND,

				COLOR_MATERIAL,
				CULL_FACE,
				DEPTH_TEST,
				DITHER,
	
				FOG,
	
				LIGHTING,
				LINE_SMOOTH,
				LINE_STIPPLE,
				LOGIC_OP,
	
				POINT_SMOOTH,
				POLYGON_SMOOTH,
				POLYGON_STIPPLE,
				SCISSOR_TEST,
				STENCIL_TEST,
				TEXTURE_1D,
				TEXTURE_2D,

				LIGHT0,
				LIGHT1,
				LIGHT2,
				LIGHT3,
				LIGHT4,
				LIGHT5,
				LIGHT6,
				LIGHT7,

				CLIP_PLANE0,
				CLIP_PLANE1,
				CLIP_PLANE2,
				CLIP_PLANE3,
				CLIP_PLANE4,
				CLIP_PLANE5
	
			};
	
			/*!
				This enum defines the different drawing primitives that can be used with for example Begin()
				\brief Drawing primitives
			*/
			enum Primitive
			{
				POINTS = 0,		//!< Draws points, every Vertex<3> represents a single point
				LINES,			//!< Draws lines, every two vertices represents one single line
				LINE_LOOP,		//!< Draws a line loop, every Vertex<3> will be connected to the previous vetrex with a line, and the first Vertex<3> will be connected to the last
				/*!
					\brief Draws a polyline, every Vertex<3> will be connected to the previous vetrtex with a line, except the first Vertex<3>
					\image html linestrip.png
				*/
				LINE_STRIP,
				TRIANGLES,		//!< Draws triangles, every three vertices defines one triangle
				/*!
					\brief Draws a trinangle strip
					\image html trianglestrip.png
				*/
				TRIANGLE_STRIP,
				TRIANGLE_FAN,	//!< Draws a triangle fan
				QUADS,			//!< Draws quads, every four vertices defines one quad
				QUAD_STRIP,		//!< Draws a quadstrip
				POLYGON			//!< Draws polygons
			};
			
			enum ShadeModel
			{
				SMOOTH = 0,
				FLAT
			};
			
			enum Faces
			{
				FRONT = 0,
				BACK,
				FRONT_AND_BACK
			};
	
			enum TestFunction
			{
				NEVER = 0,
				LESS,
				EQUAL,
				LESS_OR_EQUAL,
				GREATER,
				NOT_EQUAL,
				GREATER_OR_EQUAL,
				ALWAYS
			};
	
			enum Blendmode
			{
				ZERO = 0,
				ONE,
				SRC_COLOR,
				ONE_MINUS_SRC_COLOR,
				SRC_ALPHA,
				ONE_MINUS_SRC_ALPHA,
				DST_ALPHA,
				ONE_MINUS_DST_ALPHA,
				DST_COLOR,
				ONE_MINUS_DST_COLOR,
				SRC_ALPHA_SATURATE
			};
			
			enum Fillmode
			{
				POINT = 0,
				LINE,
				FILL
			};
	
			enum BufferProperties
			{
				DEPTH_BUFFER = 1,
				STENCIL_BUFFER = 2,
				ACCUMULATION_BUFFER = 4
			};

			struct Constructor
			{
				Window *window;
				Rect frame;
				uint32 flags;
				uint32 options;
			};


			RenderingDevice() {}
			virtual ~RenderingDevice() {}

			virtual void Lock() = 0; // the device must be locked before drawing
			virtual void Unlock() = 0; // and unlocked when finished

			virtual void SetFullScreen(bool fullscreen, int32 width, int32 height) = 0;
			virtual bool IsFullScreen() = 0;
			virtual bool CanSetFullScreen() = 0;

			virtual void Scissor(const Rect &rect) = 0;

			virtual void SetDepthFunc(TestFunction func) = 0;
			virtual void SetCullMode(Faces mode) = 0;

// view matrix operations
			virtual void LoadIdentity() = 0;
			virtual void MultMatrix(const Matrix4x4 &matrix) = 0;
			virtual void Translate(const Vertex<3> &point) = 0;
			virtual void Rotate(const Quaternion &rotation) = 0;
			virtual void Scale(const Vertex<3> &factor) = 0;

			virtual void PushMatrix() = 0;
			virtual void PopMatrix() = 0;

// texture matrix operations
			virtual void ResetTexture() = 0;
			virtual void TranslateTexture(const Vertex<3> &delta) = 0;
			virtual void RotateTexture(const Quaternion &rotation) = 0;
			virtual Matrix4x4 GetTextureMatrix() const = 0;

// draw operations
			virtual void Draw(HMesh mesh) = 0;
			virtual void Draw(const VertexArray &array) = 0;

			virtual void Begin(Primitive primitive) = 0;
			virtual void End() = 0;

			virtual void AddVertex(const Point &point) = 0;
			virtual void AddVertex(const Vertex<3> &point) = 0;

// texture operations
			virtual int32 TexUnitCount() = 0;
			virtual void UseTexture(int32 texture_unit, HTexture texture) = 0;
			
// lighting
			virtual void CreateLight(int32 index, Vertex<3> const& pos, Color const& color) = 0;
			virtual void DisableLight(int32 index) = 0;

// state changes
			virtual void SetDepthRange(float min, float max) = 0;

			virtual void SetColor(const Color &color) = 0;
			virtual void TexCoord(const Point &point) = 0;

			virtual void SetColorMask(bool red, bool green, bool blue, bool alpha) = 0;

			virtual void ClipAgainst(const Plane<3> &plane) = 0;
			virtual void SetPolygonMode(Fillmode f) = 0;

			virtual void EnableState(RenderState state) = 0;
			virtual void DisableState(RenderState state) = 0;

			virtual void SetBlendMode(Blendmode src, Blendmode dst) = 0;

			virtual const State &CurrentStates() const = 0;
			virtual void SetStates(const State &state) = 0;

// clear operations
			virtual void ClearDepthBuffer() = 0;
			virtual void ClearColorBuffer() = 0;
			virtual void SetClearColor(const Color &clr) = 0;

			// used by Camera and Portal
			virtual void SetClipPlane(const Plane<3> &plane) = 0;
			virtual void SetLayerState(const Shader::Layer &layer, int32 flags) = 0;

		protected:

			// used by inherited classes to change textureflags

			void SetChanged(HTexture texture)
			{
			}
			void SetFirstTime(HTexture texture)
			{
			}

			// used only by Texture
			virtual void UploadTexture(HTexture texture) = 0; // uploads the given texture to the card
			virtual void RemoveTexture(HTexture texture) = 0; // removes the given texture from the card
			virtual HTexture NullTexture() = 0; // returns the "none" texture

			// used only by View
			virtual void Set2DMode(float width,float height) = 0;
			virtual void Set2DMode(const Rect &rect) = 0;
			virtual void SetPerspective(float fov, float aspect, float near_clip, float far_clip) = 0;
			virtual void Viewport(const Rect &rect) = 0;

			// used only by View and Camera
			virtual void Set3DMode(float vfov, float hfov, float near_clip, float far_clip) = 0;
			
			// used only by Window
			virtual WindowBase *RenderWindow() = 0;
			virtual void SwapBuffers() = 0;
		};


} // dwarf

#endif // D_RENDERINGDEVICE_H_
