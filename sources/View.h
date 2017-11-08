#ifndef D_VIEW_H_
#define D_VIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <list>
#include "mmgr.h"

#include "Defs.h"

#include "MessageReceiver.h"
#include "InputReceiver.h"
#include "Rect.h"
#include "Plane.h"

namespace dwarf
{


	class Object;
	class View;
	class RenderingDevice;

	
	/*!
		This is an abstraction over glViewport.
		\brief divides the screen into smaller viewports, like glViewport() does.
		\todo maybe we should have some flags. e.g. WILL_RECEIVE_INPUT, the view is
		automatically added as an InputReceiver when it's added to the window (and automatically
		removed as an IntputReceiver when it's removed)
	*/
	
	class View : public MessageReceiver, public InputReceiver
	{
	friend class WindowBase;
	friend class Window;
	
	public:

		enum space_mode
		{
			SPACE_2D_RELATIVE = 0,	//!< 2 dimensional space, the size of the width abd height of the view will be 1.0 no matter how you resize it. Origo will be down in the left corner.
			SPACE_2D_ABSOLUTE,		//!< 2 dimensional space, the size of the view will be equal to the size in pixels the view cover of the screen. Origo will be down in the left corner.
			SPACE_2D_CENTER,		//!< 2 dimensional space, the size of the width abd height of the view will be 2.0 no matter how you resize it. Origo will be in center of the view.
			SPACE_3D				//!< 3 dimension space, specify it further with the SetFov() and SetClip() method.
		};
		
		enum flags
		{
			WILL_DRAW			= 1,	//!< The view will receive Draw() calls
		};

		enum resize_modes
		{
			FOLLOW_LEFT			= 0x001, //!< the left position to the parent's left side will remain the same
			FOLLOW_RIGHT		= 0x002, //!< the right position to the parent's right side will remain the same
			KEEP_H_PROPORTIONS	= 0x004, //!< this will keep the proportions along the horizontal axis, it overrides both FOLLOW_LEFT and FOLLOW_RIGHT
			CENTER_H			= 0x008, //!< keeps the horizontal position centered and never resizes the view (the width remains the same)

			FOLLOW_TOP			= 0x100, //!< the top position to the parent's top side will remain the same
			FOLLOW_BOTTOM		= 0x200, //!< the bottom position to the parent's bottom side will remain the same
			KEEP_V_PROPORTIONS	= 0x400, //!< this will keep the proportions along the vertical axis, it overrides both FOLLOW_TOP and FOLLOW_BOTTOM
			CENTER_V			= 0x800, //!< keeps the vertical position centered and never resizes the view (the height remains the same)

			KEEP_PROPORTIONS	= KEEP_H_PROPORTIONS + KEEP_V_PROPORTIONS,	//!< keeps horizontal and vertical proportions
			CENTER				= CENTER_H + CENTER_V,						//!< centers both vertiacally and horizontally
			FOLLOW_ALL_SIDES	= FOLLOW_LEFT + FOLLOW_RIGHT + FOLLOW_TOP + FOLLOW_BOTTOM, //!< Follows all sides (note that if the right edge is moved to the left of the left edge there are undefined consequences)
			FOLLOW_BOTTOM_LEFT	= FOLLOW_LEFT + FOLLOW_BOTTOM,				//!< Follows left and bottom
			FOLLOW_BOTTOM_RIGHT = FOLLOW_RIGHT + FOLLOW_BOTTOM,				//!< Follows right and bottom
			FOLLOW_TOP_LEFT		= FOLLOW_LEFT + FOLLOW_TOP,					//!< Follows left and top
			FOLLOW_TOP_RIGHT	= FOLLOW_TOP + FOLLOW_RIGHT					//!< Follows right and top
		};

		View(Rect frame, uint32 resize_mode = FOLLOW_LEFT + FOLLOW_BOTTOM, space_mode space = SPACE_2D_ABSOLUTE, uint32 flags = WILL_DRAW);
		virtual ~View();
	
		void AddChild(View *view);
	
		/*!
			\brief removes a previously added child-view from this view
			\param view a pointer to the view to remove
			\sa AddChild()
		*/
		void RemoveChild(View *view);

		/*!
			\brief tells you the number of child-views that this view has
			\return the number of child-views
			\sa AddChild() RemoveChild()
		*/
		int32 ChildCount() { return m_viewlist.size(); }
		MessageReceiver *Parent();
		Window *Win();
		std::list<View*> &Children();
	
		virtual void Show(bool visible);
		virtual bool IsVisible() const;
		Rect Bounds() const;
		const Rect &Frame() const;
		virtual Point Position() const;
		virtual void ResizeTo(const Rect &frame);

//			virtual void PreferredSize(float &min_width, float &min_height, float &max_width, float &max_height);

		void Invalidate(RenderingDevice &device) const;

		void SetResizeMode(uint32 resize_mode);
		uint32 ResizeMode() const { return m_resize_mode; }
	
	// Sound interface
	
	//	void Play(const Sound &sound);
	
	//	void Listen(const Listener &listener);
		// plays all sounds relative to the given listener, A camera derives from a listener

		/*!
			\brief sets the vertical field of view
			The horizontal field of view is automatically changed
			to match the views aspect ratio.
			This method only effects views in 3D mode (SPACE_3D).
			\param vertical_foc the vertical field of vision, ginen in radians
			\sa View
		*/
		void SetFov(float vertical_fov) { D_ASSERT_MESSAGE(m_space == SPACE_3D, TEXT("SetFov() will have no effect on a non-3D view.")); m_vfov = vertical_fov; }
		void SetClip(float near_clip, float far_clip) { m_near_clip = near_clip; m_far_clip = far_clip; }

		virtual void Tick(float frame_time);
		virtual void Draw(RenderingDevice &device) const;

		virtual void OnSize(const Rect &new_parent_size); // called when parent is resized

	protected:

		virtual void MessageReceived(Message *message);

		virtual void OnAdd(const Rect &parent_size);
		virtual void OnWindowAdd(RenderingDevice &device);

		virtual void OnRemove();
		virtual void OnWindowRemove();

		const Rect &ParentFrame() {return m_parent_frame; }
	private:


		// called by the parent when added to one. Sets the parent and the parent's size
		// this method will call OnAdd() or OnRemove()
		void SetParent(MessageReceiver *parent, const Rect &size);

		// called by the parent when added to a window. Sets the window pointer
		// this method will call OnWindowAdd() or OnWindowRemove()
		// it will also continue to call SetWindow() on all this view's child-views
		void SetWindow(Window *window);

		uint32 m_flags;	// the flags, currently only WILL_DRAW
		uint32 m_resize_mode; // the view's resize mode
		Rect m_parent_frame; // the parent's size and position
		bool m_visible; // tells if the view is visible or not
		Rect m_frame; // the view's frame rectangle
		MessageReceiver *m_parent; // pointer to parent, may be zero, before view is addded
		std::list<View*> m_viewlist; // list of child views
		space_mode m_space; // The space mode in which this view is drawn

		float m_vfov; // vertical field of view (radians)
		float m_far_clip, m_near_clip; // far and near clip, in 3D-space

		// the window this view is added to (may be 0 before it's added)
		Window *m_window;
	};
	
	// inline definitions

	/*!
		Your parent can be both a window and another view, that's why
		this method returns a MessageReceiver. The returned pointer is 0 if the
		view doesn't have a parent (i.e. has not been added to anyone).
		\brief lets you know how to message your parent
		\return a pointer to the view's parent's message receiver (it may be 0)
		\sa MessageReceiver View Window
	*/
	inline MessageReceiver *View::Parent()
	{
		return m_parent;
	}
	
	/*!
		\brief gives you access to the view children-views
		\return an iterator object that points to the first child
		\sa AddChild() RemoveChild()
	*/
	inline std::list<View*> &View::Children()
	{
		return m_viewlist;
	}

	/*!
		May be 0, if the view hasn't been added to a window.
		\brief returns a pointer to the window this view is added to
		\returns a pointer to the view's window
		\sa OnWindowAdd() OnWindowRemove()
	*/
	inline Window *View::Win()
	{
		return m_window;
	}

	/*!
		\brief returns the view's frame rectangle in the window's coordinate space
		\return the view's frame rectangle in the window's coordinate space
	*/
	inline const Rect &View::Frame() const
	{
		return m_frame;
	}


} // dwarf

#endif // D_VIEW_H_
