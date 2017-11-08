/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Log.h"
#include "List.h"
#include "Rect.h"
#include "RenderingDevice.h"
#include "Window.h"
#include "PolarWipeView.h"

using namespace dwarf;

/*!
	\brief initializes the PolarWipeView with the given frame
	\param frame the position and size of the PolarWipeView,
	the position is always given in window-space coordinate system
	(not in parent PolarWipeView coordinate system as one could have suspected)
*/
PolarWipeView::PolarWipeView(Rect frame):
	m_visible(true),
	m_frame(frame),
	m_parent(0),
	m_window(0)
{
}

/*!
	\brief virtual destructor
*/
PolarWipeView::~PolarWipeView()
{
}

/*!
	if the PolarWipeView is hidden it will simply not draw anything.
	\brief hides or shows the PolarWipeView
	\param visible true will show the PolarWipeView, false will hide it
	\sa IsVisible()
*/
void PolarWipeView::Show(bool visible)
{
	m_visible = visible;
}

/*!
	\brief let's you know if the PolarWipeView is hidden or not
	\return true if visible and false if hidden
	\sa Show()
*/
bool PolarWipeView::IsVisible() const
{
	return m_visible;
}

/*!
	\brief resizes the PolarWipeView
	\param width the new width for the PolarWipeView
	\param height the new height for the PolarWipeView
*/	
void PolarWipeView::ResizeTo(float width, float height)
{
	m_frame.width = width;
	m_frame.height = height;

	// Call OnSize on children
	for(ViewList::Iterator i = m_viewlist.Begin(); i.IsValid(); ++i)
		i.Ref()->OnSize(m_frame);
}

/*!
	\brief returns the PolarWipeView's frame rectangle
	\return the PolarWipeView's frame rectangle in window coordinate space
*/
const Rect &PolarWipeView::Frame() const
{
	return m_frame;
}

/*!
	\brief returns the position of the bottom left corner of the PolarWipeView
	\return the bottom left corner of the PolarWipeView in window coordinate space
*/
Point PolarWipeView::Position() const
{
	return m_frame.LeftBottom();
}

/*!
	\brief moves the PolarWipeView to the given position
	\param position the new position for the bottom left corner of the PolarWipeView, given in window coordinate space
*/
void PolarWipeView::MoveTo(const Point &position)
{
	m_frame.x = position.x;
	m_frame.y = position.y;

	// Call OnSize on children
	for(ViewList::Iterator i = m_viewlist.Begin(); i.IsValid(); ++i)
		i.Ref()->OnSize(m_frame);
}

/*!
	\brief forces the PolarWipeView to redraw
	\sa Draw()
*/
void PolarWipeView::Invalidate(RenderingDevice &dev) const
{
//	logger(TEXT("PolarWipeView::Invalidate())");
//	m_frame.PrintToLog();
	if (!IsVisible()) return;
	dev.Viewport(m_frame);
	Draw(dev);
	for(ViewList::Iterator i = m_viewlist.Begin(); i.IsValid(); ++i)
		i.Ref()->Invalidate(dev);
}

/*!
	\brief virtual hook-fuction inherited from MessageReceiver, will be called every frame
	\param frame_time the time (in seconds) since the method was last called
	\warning you should always call the inherited version of Tick(), in your own version of Tick(), it's not empty.
*/
void PolarWipeView::Tick(float frame_time)
{
	MessageReceiver::Tick(frame_time);

	for(List<PolarWipeView*>::Iterator i = m_viewlist.Begin(); i.IsValid(); ++i)
		i.Ref()->Tick(frame_time);
}

/*!
	You should implement this method to draw the look of your PolarWipeView
	\brief virtual hook-fuction that will be called evey time the PolarWipeView needs to be redrawn
	\param device the rendering device to draw to
	\warning if you want to redraw the PolarWipeView, don't call this method, call Invalidate().
	\sa Invalidate()
*/
void PolarWipeView::Draw(RenderingDevice &dev) const
{
}

/*!
	OnAdd() is always called before OnWindowAdd()
	\brief virtual hook-function that is called each time the PolarWipeView is added as a child to another PolarWipeView or a window
	\param parent_size is the frame rectangle of the new parent
	\warning you should always call the inherited version of OnAdd(), it's not empty
	\todo is this warning correct?
	\sa OnWindowAdd() OnRemove() OnWindowRemove()
*/
void PolarWipeView::OnAdd(const Rect &parent_size)
{
}

/*!
	Directly after this method returns the Parent() pointer is invalidated (set to 0)
	\brief virtual hook-function that is called when the PolarWipeView is removed from it's parent
	\sa OnWindowAdd() OnAdd() OnWindowRemove()
*/
void PolarWipeView::OnRemove()
{
}

/*!
	directly or indirectly. The baseclass' implementation is empty (you don't need to
	call it in your implementation if you derive directly from PolarWipeView). When this method returns
	OnWindowAdd() is called on all the PolarWipeView child-views. You are guaranteed that this method
	will be called before any Tick() or Draw() call.
	\brief virtual hook-function that is called when this PolarWipeView is added to a window
	\param device The rendering device that the window uses. It's here to let you initialize your PolarWipeView,
	create textures etc.
	\sa OnAdd() OnRemove() OnWindowRemove()
*/
void PolarWipeView::OnWindowAdd(RenderingDevice &device)
{
}

/*!
	directly or indirectly.
	\brief virtual-hook function that is called when the PolarWipeView is removed from it's window
	\sa OnWindowAdd() OnRemove() OnAdd()
*/
void PolarWipeView::OnWindowRemove()
{
}

/*!
	\brief virtual hook-function that is called each time the parent is resized
	\param parent_size is the new frame of the parent
	\warning you should always call the inherited version of OnSize(), it's not empty
	\todo is the warning here really true?
*/
void PolarWipeView::OnSize(const Rect &new_parent_size)
{
}

/*!
	The added PolarWipeView's OnAdd() method will be called, to notify the
	PolarWipeView that it has been added to another PolarWipeView. When the PolarWipeView is added
	as a child it's Draw() method and Tick() method will be called every
	frame.
	\brief adds a PolarWipeView as child to this PolarWipeView
	\param PolarWipeView the PolarWipeView to add
	\warning you still own the responsibility over the added PolarWipeView,
	you have to delete it. Also note that as long as the PolarWipeView is added
	to this PolarWipeView it may not be deleted. Use RemoveChild() to remove it from
	the PolarWipeView.
	\sa RemoveChild() Draw() Tick()
*/
void PolarWipeView::AddChild(PolarWipeView *child)
{
	D_ASSERT_MESSAGE(child != 0, TEXT("You're trying to add a null-child to a PolarWipeView."));
//	D_ASSERT_MESSAGE(m_window == 0, TEXT("You're trying to add the same PolarWipeView twice."));

	logger(TEXT("PolarWipeView::AddChild()"));
	m_viewlist.Add(child);
	child->SetParent(this, m_frame);
//	if (m_window != 0) 
	child->SetWindow(m_window);
}

// private member
void PolarWipeView::SetWindow(Window *window)
{
	m_window = window;
	if (m_window == 0) OnWindowRemove();
	else OnWindowAdd(*(window->Device()));

	for (List<PolarWipeView*>::Iterator i = Children(); i.IsValid(); i++)
		i.Ref()->SetWindow(window);
}

// private member
void PolarWipeView::SetParent(MessageReceiver *parent, const Rect &size)
{
	D_ASSERT_MESSAGE(m_parent == 0, TEXT("You're trying to add the same PolarWipeView twice."));
	m_parent = parent;
	if (m_parent == 0) OnRemove();
	else OnAdd(size);
}
