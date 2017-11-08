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

#include "View.h"
#include "Log.h"
#include "Rect.h"
#include "RenderingDevice.h"
#include "Window.h"
#include "Math.h"
#include "Matrix4x4.h"

using namespace dwarf;

/*!
	\brief initializes the view with the given frame
	\param frame the position and size of the view,
	the position is always given in window-space coordinate system
	(not in parent view coordinate system as one could have suspected)
	\param resize_mode flags that controls the way the view will follow it's
	parent when it's resized. See resize_modes. The default behaviour (if you set
	resize_mode to 0) is equal to FOLLOW_LEFT + FOLLOW_BOTTOM.
	\param space sets the drawing space of this view. Can be one of
	\arg \c View::SPACE_2D_ABSOLUTE 2 dimensional space, the size of the view will be equal to
	the size in pixels the view cover of the screen. Origo will be down in the left corner.
	\arg \c View::SPACE_2D_RELATIVE 2 dimensional space, the width and height of the coordinate system
	will be 1.0, no matter how you resize it. Origo will be down in the left corner.
	\arg \c View::SPACE_2D_CENTER 2 dimensional space, the size of the width abd height
	of the view will be 2.0 no matter how you resize it. Origo will be in center of the view.
	\arg \c View::SPACE_3D 3 dimension space, specify it further with the SetFov() and SetClip() method.
	\param flags can be any combination of
	\arg \c View::WILL_DRAW The view will call it's Draw() method (this is set by default). Set this to
	zero if you want you view to be a layout view only, that only helps organize child views.
*/
View::View(Rect frame, uint32 resize_mode, space_mode space, uint32 flags):
	m_flags(flags),
	m_resize_mode(resize_mode),
	m_visible(true),
	m_frame(frame),
	m_parent(0),
	m_space(space),
	m_vfov(90.0),
	m_far_clip(1000.0),
	m_near_clip(1.0),
	m_window(0)
{
	SetResizeMode(resize_mode);
}

/*!
	\brief virtual destructor
*/
View::~View()
{
}

/*!
	\todo document and make the method remove flags that are overridden by other flags.
*/
void View::SetResizeMode(uint32 resize_mode)
{
	m_resize_mode = resize_mode;
	if (resize_mode & (KEEP_H_PROPORTIONS + FOLLOW_LEFT + FOLLOW_RIGHT + CENTER_H) == 0)
	{
		// if no horizontal resize mode was set, default to follow left
		m_resize_mode |= FOLLOW_LEFT;
	}

	if (resize_mode & (KEEP_V_PROPORTIONS + FOLLOW_BOTTOM + FOLLOW_TOP + CENTER_H) == 0)
	{
		// if no vertical resize mode was set, default to follow bottom
		m_resize_mode |= FOLLOW_BOTTOM;
	}
}


/*!
	if the view is hidden it will simply not draw anything.
	\brief hides or shows the view
	\param visible true will show the view, false will hide it
	\sa IsVisible()
*/
void View::Show(bool visible)
{
	m_visible = visible;
}

/*!
	\brief let's you know if the view is hidden or not
	\return true if visible and false if hidden
	\sa Show()
*/
bool View::IsVisible() const
{
	return m_visible;
}

/*!
	\brief resizes the view
	\param frame the new frame of the view
*/
void View::ResizeTo(const Rect &frame)
{
	m_frame = frame;

	// Call OnSize on children
	for(std::list<View*>::iterator i = m_viewlist.begin(); i != m_viewlist.end(); i++)
		(*i)->OnSize(m_frame);
}

/*!
	This means that there is no directly logical definition of this method if
	the view is in 3D-mode. The definition is that the returned rectangle is
	the frustum projected on the near clip plane.
	\brief returns the view's frame rectangle in it's own coordinate space
	\return the view's frame rectangle in it's own coordinate space
*/
Rect View::Bounds() const
{
	switch (m_space)
	{
	case SPACE_2D_RELATIVE:
		return Rect(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case SPACE_2D_ABSOLUTE:
		return Rect(0.0f, 0.0f, Frame().Width(), Frame().Height());
		break;
	case SPACE_2D_CENTER:
		return Rect(-1, -1, 2, 2);
		break;
	case SPACE_3D:
	{
		const float x = tan(m_vfov/2.0);
		const float y = tan(m_vfov/2.0);
		return Rect(-x, -y, 2*x, 2*y);
	}break;
	}		
	D_ASSERT_MESSAGE(false, TEXT("Internal error: The view has an invalid space-state."));
	return Rect(0,0,1,1);
}

/*!
	\brief returns the position of the bottom left corner of the view
	\return the bottom left corner of the view in window coordinate space
*/
Point View::Position() const
{
	return m_frame.LeftBottom();
}

/*!
	\brief forces the view to redraw
	\sa Draw()
*/
void View::Invalidate(RenderingDevice &dev) const
{
//	logger(TEXT("View::Invalidate())");
//	m_frame.PrintToLog();
	if (!IsVisible()) return;
	if (m_flags & WILL_DRAW)
	{
		dev.Viewport(m_frame);
		switch (m_space)
		{
		case SPACE_2D_RELATIVE:
			dev.Set2DMode(1.0, 1.0);
			break;
		case SPACE_2D_ABSOLUTE:
			dev.Set2DMode(Frame().Width(), Frame().Height());
			break;
		case SPACE_2D_CENTER:
			dev.Set2DMode(Rect(-1, -1, 2, 2));
			break;
		case SPACE_3D:
			dev.SetPerspective(RadToDeg(m_vfov), Frame().Width() / Frame().Height() , m_near_clip, m_far_clip);
			break;
		default:
			D_ASSERT_MESSAGE(false, TEXT("Internal error: The view has an invalid space-state."));
		}
		Draw(dev);
		D_ASSERT_MESSAGE(dev.GetTextureMatrix() == IdentityMatrix(), TEXT("Your View did not reset the texture matrix to identity, you have to do that"));
	}
	for(std::list<View*>::const_iterator i = m_viewlist.begin(); i != m_viewlist.end(); ++i)
		(*i)->Invalidate(dev);
}

/*!
	\brief virtual hook-fuction inherited from MessageReceiver, will be called every frame
	\param frame_time the time (in seconds) since the method was last called
	\warning you should always call the inherited version of Tick(), in your own version of Tick(), it's not empty.
*/
void View::Tick(float frame_time)
{
	MessageReceiver::Tick(frame_time);

	for(std::list<View*>::iterator i = m_viewlist.begin(); i != m_viewlist.end(); ++i)
		(*i)->Tick(frame_time);
}

void View::MessageReceived(Message *message)
{
	switch(message->id)
	{
	case D_HIDE:
		Show(false);
		break;
	case D_SHOW:
		Show(true);
		break;
	case D_REMOVE:
		View *view;
		(*message) >> (int32&)view;
		RemoveChild(view);
		break;
	default:
		MessageReceiver::MessageReceived(message);
	}
}

/*!
	You should implement this method to draw the look of your view
	\brief virtual hook-fuction that will be called evey time the view needs to be redrawn
	\param device the rendering device to draw to
	\warning if you want to redraw the view, don't call this method, call Invalidate().
	\sa Invalidate()
*/
void View::Draw(RenderingDevice &dev) const
{
}

/*!
	You are supposed to fill parameters with the size this view
	looks best in. This hook-function is only used by HGroup and VGroup
	GUI layout classes. So, if you doesn't use the layout classes, you don't
	need to implement this method. The size cannot be 0 or less.
	\brief virtual hook function that is should return the prefered size of the view
	\param min_width fill this parameter with the minimal width of the view
	\param min_height fill this parameter with the minimal height of the view
	\param max_width fill this parameter with the maximum width of the view
	\param max_height fill this parameter with the maximum height of the view
*/
/*
void View::PreferredSize(float &min_width, float &min_height, float &max_width, float &max_height)
{
	D_ASSERT_MESSAGE(false, TEXT("You have a view that hasn't implemented PreferedSize(), but the PreferredSize() method is used by some class (HGroup and VGroup uses it). As long as you don't use layout classes to manage your GUI this method usually isn't needed."));
}
*/

/*!
	OnAdd() is always called before OnWindowAdd()
	\brief virtual hook-function that is called each time the view is added as a child to another view or a window
	\param parent_size is the frame rectangle of the new parent
	\warning you should always call the inherited version of OnAdd(), it's not empty
	\todo is this warning correct?
	\sa OnWindowAdd() OnRemove() OnWindowRemove()
*/
void View::OnAdd(const Rect &parent_size)
{
}

/*!
	Directly after this method returns the Parent() pointer is invalidated (set to 0)
	\brief virtual hook-function that is called when the view is removed from it's parent
	\sa OnWindowAdd() OnAdd() OnWindowRemove()
*/
void View::OnRemove()
{
}

/*!
	directly or indirectly. The baseclass' implementation is empty (you don't need to
	call it in your implementation if you derive directly from View). When this method returns
	OnWindowAdd() is called on all the view child-views. You are guaranteed that this method
	will be called before any Tick() or Draw() call.
	\brief virtual hook-function that is called when this view is added to a window
	\param device The rendering device that the window uses. It's here to let you initialize your view,
	create textures etc.
	\sa OnAdd() OnRemove() OnWindowRemove()
*/
void View::OnWindowAdd(RenderingDevice &device)
{
}

/*!
	directly or indirectly.
	\brief virtual-hook function that is called when the view is removed from it's window
	\sa OnWindowAdd() OnRemove() OnAdd()
*/
void View::OnWindowRemove()
{
}

/*!
	\brief virtual hook-function that is called each time the parent is resized
	\param parent_size is the new frame of the parent
	\warning The original implementation resizes and moves the view the way the resizing mode of the view are set.
	You only needs to implement this method if the resizing modes are not enough for you.
	\todo make sure this method behaves correctly if someone reszes a view that has not been added to another view or window.
*/
void View::OnSize(const Rect &new_parent_size)
{
	Rect new_frame = Frame();

	// ********************** Horizontal resizing ***************

	if (m_resize_mode & KEEP_H_PROPORTIONS)
	{
		float ratio = new_parent_size.Width() / m_parent_frame.Width();
		new_frame.x -= m_parent_frame.Left();
		new_frame.x *= ratio;
		new_frame.x += new_parent_size.Left();
		new_frame.width *= ratio;
	}
	else if (m_resize_mode & CENTER_H)
	{
		new_frame.x = new_parent_size.Width() / 2.0f - Frame().Width()/2.0f + new_parent_size.Left();
	}
	else
	{
		// if both flags are set
		if (m_resize_mode & FOLLOW_LEFT && m_resize_mode & FOLLOW_RIGHT)
		{
			new_frame.x += new_parent_size.x - m_parent_frame.x;
			new_frame.width += -m_parent_frame.Width() + new_parent_size.Width();
		}
		// if only FOLLOW_LEFT are set
		else if (m_resize_mode & FOLLOW_LEFT)
		{
			new_frame.x += new_parent_size.x - m_parent_frame.x;
		}
		// if only FOLLOW_RIGHT are set
		else if (m_resize_mode & FOLLOW_RIGHT)
		{
			new_frame.x += new_parent_size.x - m_parent_frame.x - m_parent_frame.Width() + new_parent_size.Width();
		}
	}

	// ********************** Vertical resizing ***************

	if (m_resize_mode & KEEP_V_PROPORTIONS)
	{
		float ratio = new_parent_size.Height() / m_parent_frame.Height();
		new_frame.y -= m_parent_frame.Bottom();
		new_frame.y *= ratio;
		new_frame.y += new_parent_size.Bottom();
		new_frame.height *= ratio;
	}
	else if (m_resize_mode & CENTER_V)
	{
		new_frame.y = new_parent_size.Height() / 2.0f - Frame().Height()/2.0f + new_parent_size.Bottom();
	}
	else
	{
		// if both flags are set
		if (m_resize_mode & FOLLOW_BOTTOM && m_resize_mode & FOLLOW_TOP)
		{
			new_frame.y += new_parent_size.y - m_parent_frame.y;
			new_frame.height += -m_parent_frame.Height() + new_parent_size.Height();
		}
		// if only FOLLOW_BOTTOM are set
		else if (m_resize_mode & FOLLOW_BOTTOM)
		{
			new_frame.y += new_parent_size.y - m_parent_frame.y;
		}
		// if only FOLLOW_TOP are set
		else if (m_resize_mode & FOLLOW_TOP)
		{
			new_frame.y += new_parent_size.y - m_parent_frame.y - m_parent_frame.Height() + new_parent_size.Height();
		}
	}


	if (new_frame != Frame()) ResizeTo(new_frame);
	m_parent_frame = new_parent_size;
}

/*!
	The added view's OnAdd() method will be called, to notify the
	view that it has been added to another view. When the view is added
	as a child it's Draw() method and Tick() method will be called every
	frame.
	\brief adds a view as child to this view
	\param view the view to add
	\warning you still own the responsibility over the added view,
	you have to delete it. Also note that as long as the view is added
	to this view it may not be deleted. Use RemoveChild() to remove it from
	the view.
	\sa RemoveChild() Draw() Tick()
*/
void View::AddChild(View *child)
{
	D_ASSERT_MESSAGE(child != 0, TEXT("null-pointer passed to View::AddChild()"));

	logger(TEXT("View::AddChild()"));
	m_viewlist.push_back(child);
	child->SetParent(this, m_frame);
//	if (m_window != 0) 
	child->SetWindow(m_window);
}

void View::RemoveChild(View *child)
{
	D_ASSERT_MESSAGE(child != 0, TEXT("null-pointer passed to View::RemoveChild()"));

	logger(TEXT("View::RemoveChild()"));
	m_viewlist.remove(child);
}

// private member
void View::SetWindow(Window *window)
{
	m_window = window;
	if (m_window == 0) OnWindowRemove();
	else OnWindowAdd(*(window->Device()));

	std::list<View*> &views = Children();
	for (std::list<View*>::iterator i = views.begin(); i != views.end(); i++)
		(*i)->SetWindow(window);
}

// private member
void View::SetParent(MessageReceiver *parent, const Rect &size)
{
	D_ASSERT_MESSAGE(m_parent == 0, TEXT("You're trying to add the same view twice."));

	m_parent = parent;
	m_parent_frame = size;
	if (m_parent == 0) OnRemove();
	else
	{
		OnAdd(size);
		OnSize(size);
	}
}
