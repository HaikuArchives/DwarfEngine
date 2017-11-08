/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 *
 *	Win32 OpenGL device.
 *
 */

/*
 * OpenGLDevice_common.h
 *
 * This file contains all the commom members of OpenGLDevice
 * It's primary function is to make it easier to
 * maintain all platforms (so we don't have to cut'n'paste
 * between the header files)
 */


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
